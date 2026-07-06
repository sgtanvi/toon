# toon

A lightweight C++ parser for JSON-like structured data. Parses text into a typed `Value` tree you can traverse, modify, and serialize back out.

## Integration

Add toon to your CMake project with `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    toon
    GIT_REPOSITORY https://github.com/sgtanvi/toon.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(toon)

target_link_libraries(your_target PRIVATE toon_lib)
```

Then include the header:

```cpp
#include "toon/json.hpp"
```

That's it — CMake automatically handles the include path because `toon_lib` declares it publicly.

## Build

Requires CMake 3.20+ and a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build
```

The binary is at `build/toon`.

## CLI

Read from a file:

```bash
./build/toon data.json
```

Read from stdin:

```bash
echo '{"name": "Alice", "age": 30}' | ./build/toon
```

Output is always pretty-printed:

```json
{
  "age": 30,
  "name": "Alice"
}
```

## C++ API

```cpp
#include "toon/json.hpp"

// Parse
toon::Value v = toon::parse(R"({"name": "Alice", "scores": [98, 100]})");

// Access
std::string name  = v["name"].asString();   // "Alice"
double      first = v["scores"][0].asNumber(); // 98.0

// Type checks
v["name"].isString();  // true
v["scores"].isArray(); // true

// Serialize
std::string compact = toon::stringify(v);
std::string pretty  = toon::stringifyPretty(v);

// Parse from file
toon::Value config = toon::parseFile("config.json");
```

### Value types

| Type | C++ accessor |
|------|-------------|
| `null` | `isNull()` |
| `bool` | `asBool()` |
| `number` | `asNumber()` → `double` |
| `string` | `asString()` |
| `array` | `asArray()` → `std::vector<Value>` |
| `object` | `asObject()` → `std::map<std::string, Value>` |

Accessors throw `toon::TypeError` if the value is the wrong type.

### Error handling

```cpp
try {
    toon::Value v = toon::parse(input);
} catch (const toon::ParseError& e) {
    std::cerr << e.what();    // includes line and column
    std::cerr << e.line();
    std::cerr << e.column();
}
```

## Format notes

- Identical to JSON with two relaxations: **trailing commas** are allowed in arrays and objects, and **duplicate keys** are rejected as errors.
- Strings support the full JSON escape set including `\uXXXX` unicode escapes.

## Project structure

```
include/toon/   public headers (token, lexer, parser, value, serializer, json)
src/            implementation
build/          cmake output
```
