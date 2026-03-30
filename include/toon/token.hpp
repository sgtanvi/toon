#pragma once

#include <string>

namespace toon {

enum class TokenType {
    // Literals
    Null,
    True,
    False,
    Number,
    String,

    // Structural
    Colon,
    Comma,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,

    // Special
    EndOfFile,
    Invalid
};

struct Token {
    TokenType type;
    std::string lexeme;
    size_t line;
    size_t column;

    Token(TokenType t, std::string lex, size_t ln, size_t col)
        : type(t), lexeme(std::move(lex)), line(ln), column(col) {}

    Token(TokenType t)
        : type(t), lexeme(""), line(0), column(0) {}

    std::string toString() const;
};

std::string tokenTypeToString(TokenType type);

} // namespace toon
