/*
 * Copyright 2017-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <thrift/lib/cpp2/transport/http2/client/H2ClientConnection.h>

#include <folly/portability/GFlags.h>
#include <glog/logging.h>

#include <folly/Likely.h>
#include <proxygen/lib/http/codec/HTTP1xCodec.h>
#include <proxygen/lib/http/codec/HTTP2Codec.h>
#include <proxygen/lib/http/codec/TransportDirection.h>
#include <proxygen/lib/http/session/HTTPTransaction.h>
#include <proxygen/lib/utils/WheelTimerInstance.h>
#include <thrift/lib/cpp/transport/TTransportException.h>
#include <thrift/lib/cpp2/transport/core/ThriftClient.h>
#include <thrift/lib/cpp2/transport/http2/client/ThriftTransactionHandler.h>
#include <thrift/lib/cpp2/transport/http2/common/SingleRpcChannel.h>
#include <wangle/acceptor/TransportInfo.h>

#include <algorithm>

namespace apache {
namespace thrift {

using apache::thrift::async::TAsyncTransport;
using apache::thrift::transport::TTransportException;
using folly::EventBase;
using proxygen::HTTPSessionBase;
using proxygen::HTTPTransaction;
using proxygen::HTTPUpstreamSession;
using proxygen::SettingsList;
using proxygen::WheelTimerInstance;
using std::string;

std::unique_ptr<ClientConnectionIf> H2ClientConnection::newHTTP2Connection(
    TAsyncTransport::UniquePtr transport) {
  std::unique_ptr<H2ClientConnection> connection(new H2ClientConnection(
      std::move(transport),
      std::make_unique<proxygen::HTTP2Codec>(
          proxygen::TransportDirection::UPSTREAM)));
  return std::move(connection);
}

H2ClientConnection::H2ClientConnection(
    TAsyncTransport::UniquePtr transport,
    std::unique_ptr<proxygen::HTTPCodec> codec)
    : evb_(transport->getEventBase()) {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  auto localAddress = transport->getLocalAddress();
  auto peerAddress = transport->getPeerAddress();
  httpSession_ = new HTTPUpstreamSession(
      WheelTimerInstance(timeout_, evb_),
      std::move(transport),
      localAddress,
      peerAddress,
      std::move(codec),
      wangle::TransportInfo(),
      this);
  // TODO: Improve the way max outging streams is set
  setMaxPendingRequests(100000);
  httpSession_->startNow();
}

H2ClientConnection::~H2ClientConnection() {
  closeNow();
}

std::shared_ptr<ThriftChannelIf> H2ClientConnection::getChannel() {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  return std::make_shared<SingleRpcChannel>(
      *evb_, [this](auto* self) { return newTransaction(self); });
}

void H2ClientConnection::setMaxPendingRequests(uint32_t num) {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  if (httpSession_) {
    httpSession_->setMaxConcurrentOutgoingStreams(num);
  }
}

void H2ClientConnection::setCloseCallback(
    ThriftClient* client,
    CloseCallback* cb) {
  if (cb == nullptr) {
    closeCallbacks_.erase(client);
  } else {
    closeCallbacks_[client] = cb;
  }
}

EventBase* H2ClientConnection::getEventBase() const {
  return evb_;
}

HTTPTransaction* H2ClientConnection::newTransaction(H2Channel* channel) {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  if (!httpSession_) {
    throw TTransportException(
        TTransportException::NOT_OPEN, "HTTPSession is not open");
  }
  // These objects destroy themselves when done.
  auto handler = new ThriftTransactionHandler();
  auto txn = httpSession_->newTransaction(handler);
  if (!txn) {
    delete handler;
    TTransportException ex(
        TTransportException::NETWORK_ERROR,
        "Too many active requests on connection");
    // Might be able to create another transaction soon
    ex.setOptions(TTransportException::CHANNEL_IS_VALID);
    throw ex;
  }
  handler->setChannel(
      std::dynamic_pointer_cast<H2Channel>(channel->shared_from_this()));
  return txn;
}

TAsyncTransport* H2ClientConnection::getTransport() {
  DCHECK(!evb_ || evb_->isInEventBaseThread());
  if (httpSession_) {
    return dynamic_cast<TAsyncTransport*>(httpSession_->getTransport());
  } else {
    return nullptr;
  }
}

bool H2ClientConnection::good() {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  auto transport = httpSession_ ? httpSession_->getTransport() : nullptr;
  return transport && transport->good();
}

ClientChannel::SaturationStatus H2ClientConnection::getSaturationStatus() {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  if (httpSession_) {
    return ClientChannel::SaturationStatus(
        httpSession_->getNumOutgoingStreams(),
        httpSession_->getMaxConcurrentOutgoingStreams());
  } else {
    return ClientChannel::SaturationStatus();
  }
}

void H2ClientConnection::attachEventBase(EventBase* evb) {
  DCHECK(evb && evb->isInEventBaseThread());
  if (httpSession_) {
    httpSession_->attachThreadLocals(
        evb,
        nullptr,
        WheelTimerInstance(timeout_, evb),
        nullptr,
        [](proxygen::HTTPCodecFilter*) {},
        nullptr,
        nullptr);
  }
  evb_ = evb;
}

void H2ClientConnection::detachEventBase() {
  DCHECK(evb_->isInEventBaseThread());
  if (httpSession_) {
    httpSession_->detachTransactions();
    httpSession_->detachThreadLocals();
  }
  evb_ = nullptr;
}

bool H2ClientConnection::isDetachable() {
  // MultiRpcChannel will always have at least one open stream.
  // This is used to leverage multiple rpcs in one stream. We should
  // still enable detaching if MultiRpc doesn't have any outstanding
  // rpcs and the number of streams is <= 1.
  // SingleRpcChannel should only detach if the number of outgoing
  // streams == 0. That's how we know there are no pending rpcs to
  // be fulfilled.
  auto session_isDetachable =
      !httpSession_ || httpSession_->getNumOutgoingStreams() == 0;
  auto transport = getTransport();
  auto transport_isDetachable = !transport || transport->isDetachable();
  return transport_isDetachable && session_isDetachable;
}

uint32_t H2ClientConnection::getTimeout() {
  return timeout_.count();
}

void H2ClientConnection::setTimeout(uint32_t ms) {
  timeout_ = std::chrono::milliseconds(ms);
  // TODO: need to change timeout in httpSession_.  This functionality
  // is also missing in JiaJie's HTTPClientChannel.
}

void H2ClientConnection::closeNow() {
  DCHECK(!evb_ || evb_->isInEventBaseThread());
  if (httpSession_) {
    if (!evb_) {
      attachEventBase(folly::EventBaseManager::get()->getEventBase());
    }
    httpSession_->dropConnection();
    httpSession_ = nullptr;
  }
}

CLIENT_TYPE H2ClientConnection::getClientType() {
  return THRIFT_HTTP_CLIENT_TYPE;
}

void H2ClientConnection::onDestroy(const HTTPSessionBase&) {
  DCHECK(evb_ && evb_->isInEventBaseThread());
  for (auto& cb : closeCallbacks_) {
    cb.second->channelClosed();
  }
  closeCallbacks_.clear();
  httpSession_ = nullptr;
}

} // namespace thrift
} // namespace apache
