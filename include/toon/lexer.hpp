#pragma once
#include "toon/token.hpp"
#include <optional>
#include <string_view>

namespace toon {

class Lexer {
public:
    explicit Lexer(std::string_view source);

    Token nextToken();
    Token peekToken();

private:
    std::string_view   source_;
    size_t             pos_  = 0;
    size_t             line_ = 1;
    size_t             col_  = 1;
    std::optional<Token> peeked_;

    char peekChar() const;
    char getChar();
    bool isAtEnd() const;

    void  skipWhitespace();
    Token scanToken();
    Token scanString();
    Token scanNumber();
    Token scanKeyword();

    std::string scanUnicodeEscape();
    std::string codePointToUTF8(unsigned int cp);

    bool isDigit(char c)    const;
    bool isAlpha(char c)    const;
    bool isHexDigit(char c) const;
};

} // namespace toon
