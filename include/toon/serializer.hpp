#pragma once
#include "toon/value.hpp"
#include <string>

namespace toon {

class Serializer {
public:
    std::string serialize(const Value& value) const;
    std::string serializePretty(const Value& value, size_t indentSize = 2) const;

private:
    std::string serializeImpl(const Value& value, size_t depth,
                              bool pretty, size_t indentSize) const;
    std::string escapeString(const std::string& s) const;
    std::string indent(size_t depth, size_t indentSize) const;
};

} // namespace toon
