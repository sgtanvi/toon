#include "toon/parser.hpp"
#include "toon/error.hpp"
#include <string>

namespace toon {

Parser::Parser(Lexer& lexer)
    : lexer_(lexer)
    , current_(lexer_.nextToken())
    , previous_(TokenType::Invalid)
{}

void Parser::advance() {
    previous_ = current_;
    current_  = lexer_.nextToken();
}

bool Parser::check(TokenType type) const {
    return current_.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) { advance(); return true; }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        Token t = current_;
        advance();
        return t;
    }
    throw ParseError(message + ", got '" + current_.lexeme + "'",
                     current_.line, current_.column);
}

Value Parser::parse() {
    Value result = parseValue();
    if (!check(TokenType::EndOfFile))
        throw ParseError("Unexpected token after value: '" + current_.lexeme + "'",
                         current_.line, current_.column);
    return result;
}

Value Parser::parseValue() {
    switch (current_.type) {
        case TokenType::Null:  advance(); return Value(nullptr);
        case TokenType::True:  advance(); return Value(true);
        case TokenType::False: advance(); return Value(false);

        case TokenType::Number: {
            double n = std::stod(current_.lexeme);
            advance();
            return Value(n);
        }
        case TokenType::String: {
            std::string s = current_.lexeme;
            advance();
            return Value(std::move(s));
        }

        case TokenType::LeftBracket: return parseArray();
        case TokenType::LeftBrace:   return parseObject();

        case TokenType::EndOfFile:
            throw ParseError("Unexpected end of input, expected a value",
                             current_.line, current_.column);
        default:
            throw ParseError("Expected a value, got '" + current_.lexeme + "'",
                             current_.line, current_.column);
    }
}

Value Parser::parseArray() {
    consume(TokenType::LeftBracket, "Expected '['");

    Value::Array elements;

    if (check(TokenType::RightBracket)) {
        advance();
        return Value(std::move(elements));
    }

    elements.push_back(parseValue());

    while (match(TokenType::Comma)) {
        if (check(TokenType::RightBracket)) break;
        elements.push_back(parseValue());
    }

    consume(TokenType::RightBracket, "Expected ']' after array elements");
    return Value(std::move(elements));
}

Value Parser::parseObject() {
    consume(TokenType::LeftBrace, "Expected '{'");

    Value::Object members;

    if (check(TokenType::RightBrace)) {
        advance();
        return Value(std::move(members));
    }

    auto parsePair = [this, &members]() {
        Token key = consume(TokenType::String, "Expected string key");
        if (members.count(key.lexeme))
            throw ParseError("Duplicate key '" + key.lexeme + "'", key.line, key.column);
        consume(TokenType::Colon, "Expected ':' after object key");
        members[key.lexeme] = parseValue();
    };

    parsePair();
    while (match(TokenType::Comma)) {
        if (check(TokenType::RightBrace)) break;
        parsePair();
    }

    consume(TokenType::RightBrace, "Expected '}' after object members");
    return Value(std::move(members));
}

} // namespace toon
