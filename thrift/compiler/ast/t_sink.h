/*
 * Copyright 2017-present Facebook, Inc.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#pragma once

#include <string>

#include <thrift/compiler/ast/t_type.h>

namespace apache {
namespace thrift {
namespace compiler {

/**
 * A sink is a lightweight object type that just wraps another data type.
 *
 */

class t_sink : public t_type {
 public:
  explicit t_sink(
      t_type* sink_type,
      t_type* final_response_type,
      t_type* first_response_type = nullptr)
      : sink_type_(sink_type),
        final_response_type_(final_response_type),
        first_response_type_(first_response_type) {}

  t_type* get_sink_type() const {
    return sink_type_;
  }

  t_type* get_final_response_type() const {
    return final_response_type_;
  }

  bool is_sink() const override {
    return true;
  }

  t_type* get_first_response_type() const {
    return first_response_type_;
  }

  bool sink_has_first_response() const {
    return (bool)(first_response_type_);
  }

  std::string get_full_name() const override {
    return "sink<" + sink_type_->get_full_name() + ", " +
        final_response_type_->get_full_name() + ">" +
        (sink_has_first_response()
             ? (", " + first_response_type_->get_full_name())
             : "");
  }

  std::string get_impl_full_name() const override {
    return "sink<" + sink_type_->get_impl_full_name() + ", " +
        final_response_type_->get_impl_full_name() + ">" +
        (sink_has_first_response()
             ? (", " + first_response_type_->get_impl_full_name())
             : "");
  }

  TypeValue get_type_value() const override {
    return TypeValue::TYPE_SINK;
  }

 private:
  t_type* sink_type_;
  t_type* final_response_type_;
  t_type* first_response_type_;
};

} // namespace compiler
} // namespace thrift
} // namespace apache
