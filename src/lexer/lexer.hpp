#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace Kookiz {

enum class TokenType: int8_t {
    // Literals
    Identifier,
    NumberLiteral,
    StringLiteral,

    Keyword,
    Operator,

    BracketRound,
    BracketSquare,
    BracketBrace,

    // Special
    EndOfFile,
    Error,
    None

};

inline const char* to_string(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "Identifier";
        case TokenType::NumberLiteral: return "NumberLiteral";
        case TokenType::StringLiteral: return "StringLiteral";
        case TokenType::Keyword: return "Keyword";
        case TokenType::Operator: return "Operator";
        case TokenType::BracketRound: return "BracketRound";
        case TokenType::BracketSquare: return "BracketSquare";
        case TokenType::BracketBrace: return "BracketBrace";
        case TokenType::EndOfFile: return "EOF";
        case TokenType::Error: return "Error";
        case TokenType::None: return "None";
    }
    return "Unknown";
}

struct Token {
    std::string string;
    TokenType token_type = TokenType::None;
    
    std::string repr() const {
        return "{ Literal: \"" + string + "\", Type: \"" + to_string(token_type) + "\" }";
    }
};

using Tokens = std::vector<Token>;

class Lexer {

    public:
    Tokens tokenize(const std::string&);

    private:
    std::string src;
    size_t at;

    inline bool end() { return at >= src.length(); }
    inline char peek() { return src[at]; }
    inline void skip_spaces() {
        do {
            ++at;
        } while (src[at] == ' ');
    }

};
}