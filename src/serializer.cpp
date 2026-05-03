#include "toon/serializer.hpp"
#include <cstdio>
#include <stdexcept>

namespace toon {

std::string Serializer::serialize(const Value& value) const {
    return serializeImpl(value, 0, false, 0);
}

std::string Serializer::serializePretty(const Value& value, size_t indentSize) const {
    return serializeImpl(value, 0, true, indentSize);
}

std::string Serializer::indent(size_t depth, size_t indentSize) const {
    return std::string(depth * indentSize, ' ');
}

std::string Serializer::escapeString(const std::string& s) const {
    std::string r;
    r.reserve(s.size() + 2);
    r += '"';
    for (unsigned char c : s) {
        switch (c) {
            case '"':  r += "\\\""; break;
            case '\\': r += "\\\\"; break;
            case '\b': r += "\\b";  break;
            case '\f': r += "\\f";  break;
            case '\n': r += "\\n";  break;
            case '\r': r += "\\r";  break;
            case '\t': r += "\\t";  break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                    r += buf;
                } else {
                    r += static_cast<char>(c);
                }
        }
    }
    r += '"';
    return r;
}

std::string Serializer::serializeImpl(const Value& value, size_t depth,
                                       bool pretty, size_t indentSize) const {
    if (value.isNull())   return "null";
    if (value.isBool())   return value.asBool() ? "true" : "false";

    if (value.isNumber()) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.17g", value.asNumber());
        return buf;
    }

    if (value.isString()) return escapeString(value.asString());

    if (value.isArray()) {
        const auto& arr = value.asArray();
        if (arr.empty()) return "[]";

        std::string r = "[";
        if (pretty) r += '\n';

        for (size_t i = 0; i < arr.size(); ++i) {
            if (pretty) r += indent(depth + 1, indentSize);
            r += serializeImpl(arr[i], depth + 1, pretty, indentSize);
            if (i + 1 < arr.size()) r += ',';
            if (pretty) r += '\n';
        }

        if (pretty) r += indent(depth, indentSize);
        r += ']';
        return r;
    }

    if (value.isObject()) {
        const auto& obj = value.asObject();
        if (obj.empty()) return "{}";

        std::string r = "{";
        if (pretty) r += '\n';

        size_t i = 0;
        for (const auto& [key, val] : obj) {
            if (pretty) r += indent(depth + 1, indentSize);
            r += escapeString(key);
            r += pretty ? ": " : ":";
            r += serializeImpl(val, depth + 1, pretty, indentSize);
            if (++i < obj.size()) r += ',';
            if (pretty) r += '\n';
        }

        if (pretty) r += indent(depth, indentSize);
        r += '}';
        return r;
    }

    throw std::runtime_error("Unknown value type");
}

} // namespace toon
