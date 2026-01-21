#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>
#include <stdexcept>

namespace toon {

class Value {
public:
    // ---- Type aliases ----
    using Array  = std::vector<Value>;
    using Object = std::map<std::string, Value>;

    // ---- Constructors ----
    Value();                          // null
    Value(std::nullptr_t);
    Value(bool b);
    Value(double n);
    Value(const std::string& s);
    Value(std::string&& s);
    Value(const char* s);
    Value(const Array& a);
    Value(Array&& a);
    Value(const Object& o);
    Value(Object&& o);

    // ---- Type checks ----
    bool isNull()   const;
    bool isBool()   const;
    bool isNumber() const;
    bool isString() const;
    bool isArray()  const;
    bool isObject() const;

    // ---- Accessors (throw if wrong type) ----
    bool&              asBool();
    double&            asNumber();
    std::string&       asString();
    Array&             asArray();
    Object&            asObject();

    const bool&        asBool()   const;
    const double&      asNumber() const;
    const std::string& asString() const;
    const Array&       asArray()  const;
    const Object&      asObject() const;

    // ---- Array/Object operators ----
    Value& operator[](size_t index);                    // Array access (throws if out of bounds)
    const Value& operator[](size_t index) const;  

    Value& operator[](const std::string& key);      // Object access (creates key if not found)
    const Value& operator[](const std::string& key) const;

    // ---- Size methods ----
    size_t size() const; 

private:
    using Storage = std::variant<
        std::nullptr_t,
        bool,
        double,
        std::string,
        Array,
        Object
    >;

    Storage data_;
};

} // namespace toon
