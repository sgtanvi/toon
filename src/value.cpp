#include "toon/value.hpp"
#include "toon/error.hpp"

namespace toon {

namespace {
    std::string typeName(const Value& v) {
        if (v.isNull())   return "null";
        if (v.isBool())   return "bool";
        if (v.isNumber()) return "number";
        if (v.isString()) return "string";
        if (v.isArray())  return "array";
        if (v.isObject()) return "object";
        return "unknown";
    }
}

Value::Value()                     : data_(nullptr) {}
Value::Value(std::nullptr_t)       : data_(nullptr) {}
Value::Value(bool b)               : data_(b) {}
Value::Value(double n)             : data_(n) {}
Value::Value(const std::string& s) : data_(s) {}
Value::Value(std::string&& s)      : data_(std::move(s)) {}
Value::Value(const char* s)        : data_(std::string(s)) {}
Value::Value(const Array& a)       : data_(a) {}
Value::Value(Array&& a)            : data_(std::move(a)) {}
Value::Value(const Object& o)      : data_(o) {}
Value::Value(Object&& o)           : data_(std::move(o)) {}

bool Value::isNull()   const { return std::holds_alternative<std::nullptr_t>(data_); }
bool Value::isBool()   const { return std::holds_alternative<bool>(data_); }
bool Value::isNumber() const { return std::holds_alternative<double>(data_); }
bool Value::isString() const { return std::holds_alternative<std::string>(data_); }
bool Value::isArray()  const { return std::holds_alternative<Array>(data_); }
bool Value::isObject() const { return std::holds_alternative<Object>(data_); }

bool& Value::asBool() {
    if (!isBool()) throw TypeError("Expected bool, got " + typeName(*this));
    return std::get<bool>(data_);
}
const bool& Value::asBool() const {
    if (!isBool()) throw TypeError("Expected bool, got " + typeName(*this));
    return std::get<bool>(data_);
}

double& Value::asNumber() {
    if (!isNumber()) throw TypeError("Expected number, got " + typeName(*this));
    return std::get<double>(data_);
}
const double& Value::asNumber() const {
    if (!isNumber()) throw TypeError("Expected number, got " + typeName(*this));
    return std::get<double>(data_);
}

std::string& Value::asString() {
    if (!isString()) throw TypeError("Expected string, got " + typeName(*this));
    return std::get<std::string>(data_);
}
const std::string& Value::asString() const {
    if (!isString()) throw TypeError("Expected string, got " + typeName(*this));
    return std::get<std::string>(data_);
}

Value::Array& Value::asArray() {
    if (!isArray()) throw TypeError("Expected array, got " + typeName(*this));
    return std::get<Array>(data_);
}
const Value::Array& Value::asArray() const {
    if (!isArray()) throw TypeError("Expected array, got " + typeName(*this));
    return std::get<Array>(data_);
}

Value::Object& Value::asObject() {
    if (!isObject()) throw TypeError("Expected object, got " + typeName(*this));
    return std::get<Object>(data_);
}
const Value::Object& Value::asObject() const {
    if (!isObject()) throw TypeError("Expected object, got " + typeName(*this));
    return std::get<Object>(data_);
}

Value& Value::operator[](size_t index) {
    Array& arr = asArray();
    if (index >= arr.size())
        throw std::out_of_range("Array index " + std::to_string(index) +
                                " out of bounds (size: " + std::to_string(arr.size()) + ")");
    return arr[index];
}
const Value& Value::operator[](size_t index) const {
    const Array& arr = asArray();
    if (index >= arr.size())
        throw std::out_of_range("Array index " + std::to_string(index) +
                                " out of bounds (size: " + std::to_string(arr.size()) + ")");
    return arr[index];
}

Value& Value::operator[](const std::string& key) {
    return asObject()[key];
}
const Value& Value::operator[](const std::string& key) const {
    const Object& obj = asObject();
    auto it = obj.find(key);
    if (it == obj.end())
        throw std::out_of_range("Key '" + key + "' not found in object");
    return it->second;
}

size_t Value::size() const {
    if (isArray())  return asArray().size();
    if (isObject()) return asObject().size();
    if (isString()) return asString().size();
    throw TypeError("size() called on " + typeName(*this) + " (expected array, object, or string)");
}

} // namespace toon
