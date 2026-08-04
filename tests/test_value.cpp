#include <catch2/catch_test_macros.hpp>
#include "toon/value.hpp"
#include "toon/error.hpp"

using namespace toon;

TEST_CASE("Default Value is null", "[value]") {
    Value v;
    REQUIRE(v.isNull());
}

TEST_CASE("Each constructor sets the correct type", "[value]") {
    REQUIRE(Value(nullptr).isNull());
    REQUIRE(Value(true).isBool());
    REQUIRE(Value(false).isBool());
    REQUIRE(Value(42.0).isNumber());
    REQUIRE(Value("hello").isString());
    REQUIRE(Value(std::string("hi")).isString());
    REQUIRE(Value(Value::Array{}).isArray());
    REQUIRE(Value(Value::Object{}).isObject());
}

TEST_CASE("Accessors return the stored value", "[value]") {
    REQUIRE(Value(true).asBool()      == true);
    REQUIRE(Value(false).asBool()     == false);
    REQUIRE(Value(3.14).asNumber()    == 3.14);
    REQUIRE(Value("hi").asString()    == "hi");
}

TEST_CASE("Wrong-type accessor throws TypeError", "[value]") {
    Value n(42.0);
    REQUIRE_THROWS_AS(n.asBool(),   TypeError);
    REQUIRE_THROWS_AS(n.asString(), TypeError);
    REQUIRE_THROWS_AS(n.asArray(),  TypeError);
    REQUIRE_THROWS_AS(n.asObject(), TypeError);
}

TEST_CASE("Array index operator", "[value]") {
    Value v(Value::Array{ Value(10.0), Value(20.0), Value(30.0) });

    REQUIRE(v[0].asNumber() == 10.0);
    REQUIRE(v[2].asNumber() == 30.0);
    REQUIRE_THROWS_AS(v[3], std::out_of_range);
}

TEST_CASE("Object key operator (const) throws on missing key", "[value]") {
    Value::Object obj;
    obj["x"] = Value(1.0);
    const Value v(std::move(obj));

    REQUIRE(v["x"].asNumber() == 1.0);
    REQUIRE_THROWS_AS(v["missing"], std::out_of_range);
}

TEST_CASE("size() works for array, object, and string", "[value]") {
    Value arr(Value::Array{ Value(1.0), Value(2.0) });
    REQUIRE(arr.size() == 2);

    Value str("hello");
    REQUIRE(str.size() == 5);

    Value obj(Value::Object{});
    REQUIRE(obj.size() == 0);
}

TEST_CASE("size() throws on non-container type", "[value]") {
    REQUIRE_THROWS_AS(Value(42.0).size(), TypeError);
    REQUIRE_THROWS_AS(Value(true).size(), TypeError);
    REQUIRE_THROWS_AS(Value(nullptr).size(), TypeError);
}
