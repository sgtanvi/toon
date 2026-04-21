#pragma once
#include "toon/lexer.hpp"
#include "toon/value.hpp"
#include <string>

namespace toon {

class Parser {
public:
    explicit Parser(Lexer& lexer);
    Value parse();

private:
    Lexer& lexer_;
    Token  current_;
    Token  previous_;

    void  advance();
    bool  check(TokenType type) const;
    bool  match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    Value parseValue();
    Value parseArray();
    Value parseObject();
};

} // namespace toon
