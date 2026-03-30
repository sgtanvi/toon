#pragma once
#include <stdexcept>
#include <string>

namespace toon {

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message, size_t line, size_t column)
        : std::runtime_error(
              "Parse error at line " + std::to_string(line) +
              ", column " + std::to_string(column) + ": " + message)
        , line_(line)
        , column_(column)
    {}

    size_t line()   const { return line_; }
    size_t column() const { return column_; }

private:
    size_t line_;
    size_t column_;
};

class TypeError : public std::runtime_error {
public:
    explicit TypeError(const std::string& message)
        : std::runtime_error(message)
    {}
};

} // namespace toon
