#include "toon/token.hpp"

namespace toon {

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Null:         return "Null";
        case TokenType::True:         return "True";
        case TokenType::False:        return "False";
        case TokenType::Number:       return "Number";
        case TokenType::String:       return "String";
        case TokenType::Colon:        return "Colon";
        case TokenType::Comma:        return "Comma";
        case TokenType::LeftBracket:  return "LeftBracket";
        case TokenType::RightBracket: return "RightBracket";
        case TokenType::LeftBrace:    return "LeftBrace";
        case TokenType::RightBrace:   return "RightBrace";
        case TokenType::EndOfFile:    return "EndOfFile";
        case TokenType::Invalid:      return "Invalid";
    }
    return "Unknown";
}

std::string Token::toString() const {
    return "Token(" + tokenTypeToString(type) + ", \"" + lexeme +
           "\", " + std::to_string(line) + ":" + std::to_string(column) + ")";
}

} // namespace toon
