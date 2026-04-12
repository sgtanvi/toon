#include "toon/lexer.hpp"
#include "toon/error.hpp"

namespace toon {

Lexer::Lexer(std::string_view source) : source_(source) {}

char Lexer::peekChar() const {
    return pos_ < source_.size() ? source_[pos_] : '\0';
}

char Lexer::getChar() {
    if (pos_ >= source_.size()) return '\0';
    char c = source_[pos_++];
    if (c == '\n') { ++line_; col_ = 1; } else { ++col_; }
    return c;
}

bool Lexer::isAtEnd() const { return pos_ >= source_.size(); }

void Lexer::skipWhitespace() {
    while (true) {
        char c = peekChar();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            getChar();
        else
            return;
    }
}

bool Lexer::isDigit(char c)    const { return c >= '0' && c <= '9'; }
bool Lexer::isAlpha(char c)    const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool Lexer::isHexDigit(char c) const { return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

Token Lexer::nextToken() {
    if (peeked_) {
        Token t = *peeked_;
        peeked_.reset();
        return t;
    }
    return scanToken();
}

Token Lexer::peekToken() {
    if (!peeked_) peeked_ = scanToken();
    return *peeked_;
}

Token Lexer::scanToken() {
    skipWhitespace();

    size_t startLine = line_;
    size_t startCol  = col_;
    char   c         = peekChar();

    if (c == '\0') return Token(TokenType::EndOfFile, "", startLine, startCol);

    switch (c) {
        case ':': getChar(); return Token(TokenType::Colon,        ":",  startLine, startCol);
        case ',': getChar(); return Token(TokenType::Comma,        ",",  startLine, startCol);
        case '[': getChar(); return Token(TokenType::LeftBracket,  "[",  startLine, startCol);
        case ']': getChar(); return Token(TokenType::RightBracket, "]",  startLine, startCol);
        case '{': getChar(); return Token(TokenType::LeftBrace,    "{",  startLine, startCol);
        case '}': getChar(); return Token(TokenType::RightBrace,   "}",  startLine, startCol);
    }

    if (c == '"')                  return scanString();
    if (c == '-' || isDigit(c))    return scanNumber();
    if (isAlpha(c))                return scanKeyword();

    getChar();
    throw ParseError("Unexpected character '" + std::string(1, c) + "'", startLine, startCol);
}

Token Lexer::scanString() {
    size_t startLine = line_, startCol = col_;
    getChar(); // consume opening '"'

    std::string value;
    while (true) {
        char c = peekChar();

        if (c == '\0') throw ParseError("Unterminated string", startLine, startCol);
        if (c == '\n') throw ParseError("Newline in string (use \\n instead)", line_, col_);

        if (c == '"') {
            getChar();
            return Token(TokenType::String, value, startLine, startCol);
        }

        if (c == '\\') {
            getChar();
            char esc = getChar();
            switch (esc) {
                case '"':  value += '"';  break;
                case '\\': value += '\\'; break;
                case '/':  value += '/';  break;
                case 'b':  value += '\b'; break;
                case 'f':  value += '\f'; break;
                case 'n':  value += '\n'; break;
                case 'r':  value += '\r'; break;
                case 't':  value += '\t'; break;
                case 'u':  value += scanUnicodeEscape(); break;
                default:
                    throw ParseError("Invalid escape sequence '\\" + std::string(1, esc) + "'",
                                     line_, col_);
            }
        } else {
            value += getChar();
        }
    }
}

std::string Lexer::scanUnicodeEscape() {
    std::string hex;
    for (int i = 0; i < 4; i++) {
        char c = peekChar();
        if (!isHexDigit(c))
            throw ParseError("Expected 4 hex digits in unicode escape", line_, col_);
        hex += getChar();
    }
    return codePointToUTF8(std::stoul(hex, nullptr, 16));
}

std::string Lexer::codePointToUTF8(unsigned int cp) {
    std::string r;
    if (cp < 0x80) {
        r += static_cast<char>(cp);
    } else if (cp < 0x800) {
        r += static_cast<char>(0xC0 | (cp >> 6));
        r += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        r += static_cast<char>(0xE0 | (cp >> 12));
        r += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        r += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        r += static_cast<char>(0xF0 | (cp >> 18));
        r += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        r += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        r += static_cast<char>(0x80 | (cp & 0x3F));
    }
    return r;
}

Token Lexer::scanNumber() {
    size_t startLine = line_, startCol = col_, startPos = pos_;

    if (peekChar() == '-') getChar();

    if (peekChar() == '0') {
        getChar();
    } else if (isDigit(peekChar())) {
        while (isDigit(peekChar())) getChar();
    } else {
        throw ParseError("Invalid number", startLine, startCol);
    }

    if (peekChar() == '.') {
        getChar();
        if (!isDigit(peekChar()))
            throw ParseError("Expected digit after decimal point", line_, col_);
        while (isDigit(peekChar())) getChar();
    }

    if (peekChar() == 'e' || peekChar() == 'E') {
        getChar();
        if (peekChar() == '+' || peekChar() == '-') getChar();
        if (!isDigit(peekChar()))
            throw ParseError("Expected digit in exponent", line_, col_);
        while (isDigit(peekChar())) getChar();
    }

    return Token(TokenType::Number,
                 std::string(source_.substr(startPos, pos_ - startPos)),
                 startLine, startCol);
}

Token Lexer::scanKeyword() {
    size_t startLine = line_, startCol = col_, startPos = pos_;
    while (isAlpha(peekChar())) getChar();

    std::string word(source_.substr(startPos, pos_ - startPos));

    if (word == "null")  return Token(TokenType::Null,  word, startLine, startCol);
    if (word == "true")  return Token(TokenType::True,  word, startLine, startCol);
    if (word == "false") return Token(TokenType::False, word, startLine, startCol);

    throw ParseError("Unknown keyword '" + word + "'", startLine, startCol);
}

} // namespace toon
