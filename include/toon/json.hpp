#pragma once
#include "toon/value.hpp"
#include "toon/error.hpp"
#include "toon/lexer.hpp"
#include "toon/parser.hpp"
#include "toon/serializer.hpp"
#include <string>
#include <string_view>

namespace toon {

inline Value parse(std::string_view input) {
    Lexer  lexer(input);
    Parser parser(lexer);
    return parser.parse();
}

Value parseFile(const std::string& path);

inline std::string stringify(const Value& value) {
    return Serializer{}.serialize(value);
}

inline std::string stringifyPretty(const Value& value, size_t indent = 2) {
    return Serializer{}.serializePretty(value, indent);
}

} // namespace toon
