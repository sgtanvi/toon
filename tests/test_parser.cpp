#include <catch2/catch_test_macros.hpp>
#include "toon/json.hpp"
#include "toon/error.hpp"

using namespace toon;

// ---- Primitives ----

TEST_CASE("Parse null", "[parser]") {
    REQUIRE(parse("null").isNull());
}

TEST_CASE("Parse booleans", "[parser]") {
    REQUIRE(parse("true").asBool()  == true);
    REQUIRE(parse("false").asBool() == false);
}

TEST_CASE("Parse integers and decimals", "[parser]") {
    REQUIRE(parse("0").asNumber()     == 0.0);
    REQUIRE(parse("42").asNumber()    == 42.0);
    REQUIRE(parse("-7").asNumber()    == -7.0);
    REQUIRE(parse("3.14").asNumber()  == 3.14);
}

TEST_CASE("Parse scientific notation", "[parser]") {
    REQUIRE(parse("1e3").asNumber()   == 1000.0);
    REQUIRE(parse("1.5E2").asNumber() == 150.0);
    REQUIRE(parse("2e-1").asNumber()  == 0.2);
}

TEST_CASE("Parse strings", "[parser]") {
    REQUIRE(parse(R"("hello")").asString() == "hello");
    REQUIRE(parse(R"("")").asString()      == "");
}

TEST_CASE("Parse string escape sequences", "[parser]") {
    REQUIRE(parse(R"("line\nbreak")").asString()  == "line\nbreak");
    REQUIRE(parse(R"("tab\there")").asString()    == "tab\there");
    REQUIRE(parse(R"("quote\"hi\"")").asString()  == "quote\"hi\"");
    REQUIRE(parse(R"("back\\slash")").asString()  == "back\\slash");
}

// ---- Arrays ----

TEST_CASE("Parse empty array", "[parser]") {
    auto v = parse("[]");
    REQUIRE(v.isArray());
    REQUIRE(v.size() == 0);
}

TEST_CASE("Parse flat array", "[parser]") {
    auto v = parse("[1, 2, 3]");
    REQUIRE(v.size() == 3);
    REQUIRE(v[0].asNumber() == 1.0);
    REQUIRE(v[2].asNumber() == 3.0);
}

TEST_CASE("Parse array with trailing comma", "[parser]") {
    auto v = parse("[1, 2, 3,]");
    REQUIRE(v.size() == 3);
}

TEST_CASE("Parse array of mixed types", "[parser]") {
    auto v = parse(R"([null, true, 1, "hi"])");
    REQUIRE(v[0].isNull());
    REQUIRE(v[1].isBool());
    REQUIRE(v[2].isNumber());
    REQUIRE(v[3].isString());
}

// ---- Objects ----

TEST_CASE("Parse empty object", "[parser]") {
    auto v = parse("{}");
    REQUIRE(v.isObject());
    REQUIRE(v.size() == 0);
}

TEST_CASE("Parse object", "[parser]") {
    auto v = parse(R"({"name": "Alice", "age": 30})");
    REQUIRE(v["name"].asString() == "Alice");
    REQUIRE(v["age"].asNumber()  == 30.0);
}

TEST_CASE("Parse object with trailing comma", "[parser]") {
    auto v = parse(R"({"a": 1,})");
    REQUIRE(v["a"].asNumber() == 1.0);
}

// ---- Nesting ----

TEST_CASE("Parse nested array in object", "[parser]") {
    auto v = parse(R"({"scores": [98, 100, 87]})");
    REQUIRE(v["scores"][1].asNumber() == 100.0);
}

TEST_CASE("Parse deeply nested structure", "[parser]") {
    auto v = parse(R"({"a": {"b": {"c": 42}}})");
    REQUIRE(v["a"]["b"]["c"].asNumber() == 42.0);
}

// ---- Error cases ----

TEST_CASE("Empty input throws ParseError", "[parser][error]") {
    REQUIRE_THROWS_AS(parse(""), ParseError);
}

TEST_CASE("Duplicate object key throws ParseError", "[parser][error]") {
    REQUIRE_THROWS_AS(parse(R"({"a": 1, "a": 2})"), ParseError);
}

TEST_CASE("Trailing token throws ParseError", "[parser][error]") {
    REQUIRE_THROWS_AS(parse("null null"), ParseError);
}

TEST_CASE("Unterminated string throws ParseError", "[parser][error]") {
    REQUIRE_THROWS_AS(parse(R"("not closed)"), ParseError);
}

TEST_CASE("Unknown keyword throws ParseError", "[parser][error]") {
    REQUIRE_THROWS_AS(parse("nul"), ParseError);
}

TEST_CASE("ParseError carries line and column info", "[parser][error]") {
    try {
        parse("{\n  \"key\": @\n}");
        FAIL("expected ParseError");
    } catch (const ParseError& e) {
        REQUIRE(e.line()   == 2);
        REQUIRE(e.column() > 0);
    }
}
