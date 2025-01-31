<?hh // strict
/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

/**
 * Original thrift enum:-
 * Animal
 */
enum Animal: int {
  DOG = 1;
  CAT = 2;
  TARANTULA = 3;
}

type PersonID = int;
/**
 * Original thrift struct:-
 * Color
 */
class Color implements \IThriftStruct {
  use \ThriftSerializationTrait;

  const dict<int, this::TFieldSpec> SPEC = dict[
    1 => shape(
      'var' => 'red',
      'type' => \TType::DOUBLE,
    ),
    2 => shape(
      'var' => 'green',
      'type' => \TType::DOUBLE,
    ),
    3 => shape(
      'var' => 'blue',
      'type' => \TType::DOUBLE,
    ),
    4 => shape(
      'var' => 'alpha',
      'type' => \TType::DOUBLE,
    ),
  ];
  const dict<string, int> FIELDMAP = dict[
    'red' => 1,
    'green' => 2,
    'blue' => 3,
    'alpha' => 4,
  ];
  const int STRUCTURAL_ID = 5495510740722957663;
  /**
   * Original thrift field:-
   * 1: double red
   */
  public float $red;
  /**
   * Original thrift field:-
   * 2: double green
   */
  public float $green;
  /**
   * Original thrift field:-
   * 3: double blue
   */
  public float $blue;
  /**
   * Original thrift field:-
   * 4: double alpha
   */
  public float $alpha;

  <<__Rx>>
  public function __construct(?float $red = null, ?float $green = null, ?float $blue = null, ?float $alpha = null  ) {
    if ($red === null) {
      $this->red = 0.0;
    } else {
      $this->red = $red;
    }
    if ($green === null) {
      $this->green = 0.0;
    } else {
      $this->green = $green;
    }
    if ($blue === null) {
      $this->blue = 0.0;
    } else {
      $this->blue = $blue;
    }
    if ($alpha === null) {
      $this->alpha = 0.0;
    } else {
      $this->alpha = $alpha;
    }
  }

  public function getName(): string {
    return 'Color';
  }

}

/**
 * Original thrift struct:-
 * Vehicle
 */
class Vehicle implements \IThriftStruct {
  use \ThriftSerializationTrait;

  const dict<int, this::TFieldSpec> SPEC = dict[
    1 => shape(
      'var' => 'color',
      'type' => \TType::STRUCT,
      'class' => Color::class,
    ),
    2 => shape(
      'var' => 'licensePlate',
      'type' => \TType::STRING,
    ),
    3 => shape(
      'var' => 'description',
      'type' => \TType::STRING,
    ),
    4 => shape(
      'var' => 'name',
      'type' => \TType::STRING,
    ),
    5 => shape(
      'var' => 'hasAC',
      'type' => \TType::BOOL,
    ),
  ];
  const dict<string, int> FIELDMAP = dict[
    'color' => 1,
    'licensePlate' => 2,
    'description' => 3,
    'name' => 4,
    'hasAC' => 5,
  ];
  const int STRUCTURAL_ID = 2222890000100114781;
  /**
   * Original thrift field:-
   * 1: struct module.Color color
   */
  public ?Color $color;
  /**
   * Original thrift field:-
   * 2: string licensePlate
   */
  public ?string $licensePlate;
  /**
   * Original thrift field:-
   * 3: string description
   */
  public ?string $description;
  /**
   * Original thrift field:-
   * 4: string name
   */
  public ?string $name;
  /**
   * Original thrift field:-
   * 5: bool hasAC
   */
  public bool $hasAC;

  <<__Rx>>
  public function __construct(?Color $color = null, ?string $licensePlate = null, ?string $description = null, ?string $name = null, ?bool $hasAC = null  ) {
    $this->color = $color;
    $this->licensePlate = $licensePlate;
    $this->description = $description;
    $this->name = $name;
    if ($hasAC === null) {
      $this->hasAC = false;
    } else {
      $this->hasAC = $hasAC;
    }
  }

  public function getName(): string {
    return 'Vehicle';
  }

}

/**
 * Original thrift struct:-
 * Person
 */
class Person implements \IThriftStruct {
  use \ThriftSerializationTrait;

  const dict<int, this::TFieldSpec> SPEC = dict[
    1 => shape(
      'var' => 'id',
      'type' => \TType::I64,
    ),
    2 => shape(
      'var' => 'name',
      'type' => \TType::STRING,
    ),
    3 => shape(
      'var' => 'age',
      'type' => \TType::I16,
    ),
    4 => shape(
      'var' => 'address',
      'type' => \TType::STRING,
    ),
    5 => shape(
      'var' => 'favoriteColor',
      'type' => \TType::STRUCT,
      'class' => Color::class,
    ),
    6 => shape(
      'var' => 'friends',
      'type' => \TType::SET,
      'etype' => \TType::I64,
      'elem' => shape(
        'type' => \TType::I64,
      ),
      'format' => 'collection',
    ),
    7 => shape(
      'var' => 'bestFriend',
      'type' => \TType::I64,
    ),
    8 => shape(
      'var' => 'petNames',
      'type' => \TType::MAP,
      'ktype' => \TType::I32,
      'vtype' => \TType::STRING,
      'key' => shape(
        'type' => \TType::I32,
        'enum' => Animal::class,
      ),
      'val' => shape(
        'type' => \TType::STRING,
      ),
      'format' => 'collection',
    ),
    9 => shape(
      'var' => 'afraidOfAnimal',
      'type' => \TType::I32,
      'enum' => Animal::class,
    ),
    10 => shape(
      'var' => 'vehicles',
      'type' => \TType::LST,
      'etype' => \TType::STRUCT,
      'elem' => shape(
        'type' => \TType::STRUCT,
        'class' => Vehicle::class,
      ),
      'format' => 'collection',
    ),
  ];
  const dict<string, int> FIELDMAP = dict[
    'id' => 1,
    'name' => 2,
    'age' => 3,
    'address' => 4,
    'favoriteColor' => 5,
    'friends' => 6,
    'bestFriend' => 7,
    'petNames' => 8,
    'afraidOfAnimal' => 9,
    'vehicles' => 10,
  ];
  const int STRUCTURAL_ID = 5615342512964403351;
  /**
   * Original thrift field:-
   * 1: i64 id
   */
  public int $id;
  /**
   * Original thrift field:-
   * 2: string name
   */
  public string $name;
  /**
   * Original thrift field:-
   * 3: i16 age
   */
  public ?int $age;
  /**
   * Original thrift field:-
   * 4: string address
   */
  public ?string $address;
  /**
   * Original thrift field:-
   * 5: struct module.Color favoriteColor
   */
  public ?Color $favoriteColor;
  /**
   * Original thrift field:-
   * 6: set<i64> friends
   */
  public ?Set<int> $friends;
  /**
   * Original thrift field:-
   * 7: i64 bestFriend
   */
  public ?int $bestFriend;
  /**
   * Original thrift field:-
   * 8: map<enum module.Animal, string> petNames
   */
  public ?Map<Animal, string> $petNames;
  /**
   * Original thrift field:-
   * 9: enum module.Animal afraidOfAnimal
   */
  public ?Animal $afraidOfAnimal;
  /**
   * Original thrift field:-
   * 10: list<struct module.Vehicle> vehicles
   */
  public ?Vector<Vehicle> $vehicles;

  <<__Rx>>
  public function __construct(?int $id = null, ?string $name = null, ?int $age = null, ?string $address = null, ?Color $favoriteColor = null, ?Set<int> $friends = null, ?int $bestFriend = null, ?Map<Animal, string> $petNames = null, ?Animal $afraidOfAnimal = null, ?Vector<Vehicle> $vehicles = null  ) {
    if ($id === null) {
      $this->id = 0;
    } else {
      $this->id = $id;
    }
    if ($name === null) {
      $this->name = '';
    } else {
      $this->name = $name;
    }
    $this->age = $age;
    $this->address = $address;
    $this->favoriteColor = $favoriteColor;
    $this->friends = $friends;
    $this->bestFriend = $bestFriend;
    $this->petNames = $petNames;
    $this->afraidOfAnimal = $afraidOfAnimal;
    $this->vehicles = $vehicles;
  }

  public function getName(): string {
    return 'Person';
  }

}

