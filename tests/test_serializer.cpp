#include <catch2/catch_test_macros.hpp>
#include "toon/json.hpp"

using namespace toon;

// ---- Primitive output ----

TEST_CASE("Serialize null", "[serializer]") {
    REQUIRE(stringify(Value(nullptr)) == "null");
}

TEST_CASE("Serialize booleans", "[serializer]") {
    REQUIRE(stringify(Value(true))  == "true");
    REQUIRE(stringify(Value(false)) == "false");
}

TEST_CASE("Serialize whole number omits decimal point", "[serializer]") {
    REQUIRE(stringify(Value(42.0)) == "42");
    REQUIRE(stringify(Value(0.0))  == "0");
}

TEST_CASE("Serialize decimal number", "[serializer]") {
    REQUIRE(stringify(Value(3.14)) == "3.1400000000000001");
}

// ---- String escaping ----

TEST_CASE("Serialize plain string", "[serializer]") {
    REQUIRE(stringify(Value("hello")) == "\"hello\"");
}

TEST_CASE("Serialize string with special characters", "[serializer]") {
    REQUIRE(stringify(Value("line\nbreak")) == R"("line\nbreak")");
    REQUIRE(stringify(Value("tab\there"))   == R"("tab\there")");
    REQUIRE(stringify(Value("say \"hi\"")) == R"("say \"hi\"")");
    REQUIRE(stringify(Value("back\\slash")) == R"("back\\slash")");
}

// ---- Collections ----

TEST_CASE("Serialize empty array and object", "[serializer]") {
    REQUIRE(stringify(Value(Value::Array{}))  == "[]");
    REQUIRE(stringify(Value(Value::Object{})) == "{}");
}

TEST_CASE("Serialize compact array", "[serializer]") {
    Value v(Value::Array{ Value(1.0), Value(2.0), Value(3.0) });
    REQUIRE(stringify(v) == "[1,2,3]");
}

// ---- Round-trips ----

TEST_CASE("Round-trip primitives", "[serializer]") {
    for (auto input : { "null", "true", "false" }) {
        REQUIRE(stringify(parse(input)) == input);
    }
}

TEST_CASE("Round-trip empty collections", "[serializer]") {
    REQUIRE(stringify(parse("[]")) == "[]");
    REQUIRE(stringify(parse("{}")) == "{}");
}

TEST_CASE("Round-trip nested structure", "[serializer]") {
    std::string input = R"({"active":true,"name":"Alice","scores":[98,100,87]})";
    REQUIRE(stringify(parse(input)) == input);
}

// ---- Pretty print ----

TEST_CASE("Pretty-print array indents elements", "[serializer]") {
    auto result = stringifyPretty(parse("[1,2]"));
    REQUIRE(result.find("  ") != std::string::npos);
    REQUIRE(result.find('\n') != std::string::npos);
}

TEST_CASE("Pretty-print object indents key-value pairs", "[serializer]") {
    auto result = stringifyPretty(parse(R"({"a":1})"));
    REQUIRE(result.find("  \"a\": 1") != std::string::npos);
}
