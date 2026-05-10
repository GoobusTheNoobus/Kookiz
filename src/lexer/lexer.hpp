#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <sstream>

namespace Kookiz {

enum class TokenType: int8_t {
    // Literals
    Identifier,
    IntegerLiteral,
    FloatLiteral,
    StringLiteral,

    KwdReturn,
    KwdFn,
    KwdVar,
    KwdTrue,
    KwdFalse,
    KwdNull,

    OperAdd,
    OperMinus,
    OperStar,
    OperSlash,
    OperEqual,
    OperSemicolon,
    OperDot,
    OperColon,

    OBracketRound,
    CBracketRound,
    OBracketSquare,
    CBracketSquare,
    OBracketBrace,
    CBracketBrace,

    // Special
    EndOfFile,
    Error,
    None

};

inline const char* to_string(TokenType type) {
    switch (type) {
        case TokenType::Identifier: return "Identifier";
        case TokenType::IntegerLiteral: return "IntegerLiteral";
        case TokenType::FloatLiteral: return "FloatLiteral";
        case TokenType::StringLiteral: return "StringLiteral";

        case TokenType::KwdReturn: return "KwdReturn";
        case TokenType::KwdFn: return "KwdFn";
        case TokenType::KwdVar: return "KwdVar";
        case TokenType::KwdTrue: return "KwdTrue";
        case TokenType::KwdFalse: return "KwdFalse";
        case TokenType::KwdNull: return "KwdNull";

        case TokenType::OperAdd: return "OperAdd";
        case TokenType::OperMinus: return "OperMinus";
        case TokenType::OperStar: return "OperStar";
        case TokenType::OperSlash: return "OperSlash";
        case TokenType::OperEqual: return "OperEqual";
        case TokenType::OperSemicolon: return "OperSemicolon";
        case TokenType::OperColon: return "OperColon";
        case TokenType::OperDot: return "OperDot";
        case TokenType::OBracketRound: return "OBracketRound";
        case TokenType::OBracketSquare: return "OBracketSquare";
        case TokenType::OBracketBrace: return "OBracketBrace";
        case TokenType::CBracketRound: return "CBracketRound";
        case TokenType::CBracketSquare: return "CBracketSquare";
        case TokenType::CBracketBrace: return "CBracketBrace";
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

    static Token end_of_file() {
        return {"", TokenType::EndOfFile};
    }
};

using Tokens = std::vector<Token>;

inline std::ostream& operator<<(std::ostream& os, const Tokens& tokens) {
    for (const Token& t: tokens) {
        os << t.repr() << "\n";
    }

    return os;
}

class Lexer {

    public:
    Tokens tokenize(const std::string&);

    private:
    std::string src;
    size_t at = 0;
    int line = 1;

    inline bool end() { return at >= src.length(); }
    inline char peek() { return src[at]; }
    inline char peek(size_t off = 0) const {
        return (at + off < src.size()) ? src[at + off] : '\0';
    }
    inline void skip_spaces() {
        while (at < src.length() && (src[at] == ' ' || src[at] == '\t')) ++at;
    }
    inline void next() { ++at; }

    Token make_identifier_or_keyword();
    Token make_number();
    Token make_string();
    Token make_operator();

};
}