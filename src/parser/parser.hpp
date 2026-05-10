#pragma once

#include "diagnostics/diagnostics.hpp"
#include "lexer/lexer.hpp"

#include <memory>
#include <vector>
#include <string>

namespace Kookiz {

enum class OperatorEnum: int8_t {
    OperAdd,
    OperMinus,
    OperStar,
    OperSlash,
    OperEqual,
    OperSemicolon,
    OperDot,
    OperColon,
};

enum class KeywordEnum: int8_t {
    KwdReturn,
    KwdFn,
    KwdVar,
    KwdTrue,
    KwdFalse,
    KwdNull,
};

enum class NodeType: int8_t {
    Root,
    Block,

    Identifier,
    Keyword,
    Integer,
    Float,
    String,

    UnaryExpression,
    BinaryExpression,
    CallExpression,
    MemberAccessExpression,
    
    None
};

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual NodeType kind() const { return NodeType::None; }
    ASTNode() = default;
};

using AST = std::unique_ptr<ASTNode>;

struct UnaryExpression : ASTNode {
    AST child;
    AST op;

    NodeType kind() const override { return NodeType::UnaryExpression; }
};

struct BinaryExpression : ASTNode {
    AST child1;
    AST child2;
    OperatorEnum op;

    NodeType kind() const override { return NodeType::BinaryExpression; }
};

struct CallExpression : ASTNode {
    AST func;
    std::vector<AST> params;

    NodeType kind() const override { return NodeType::CallExpression; }
};

struct MemberAccessExpression : ASTNode {
    AST obj;
    std::string member;

    NodeType kind() const override { return NodeType::MemberAccessExpression; }
};

// Contains the entire program
struct Root : ASTNode {
    std::vector<AST> children;

    NodeType kind() const override { return NodeType::Root; }
};

// Works similarly to root
struct Block : ASTNode {
    std::vector<AST> children;

    NodeType kind() const override { return NodeType::Block; }
};

struct Identifier : ASTNode {
    std::string data;

    NodeType kind() const override { return NodeType::Identifier; }
};

struct Keyword : ASTNode {
    KeywordEnum kwd;
    
    NodeType kind() const override { return NodeType::Keyword; }
};

struct Integer : ASTNode {
    int64_t data;

    Integer(int64_t d) : data(d) {}
    NodeType kind() const override { return NodeType::Integer; }
};

struct Float : ASTNode {
    double data;

    NodeType kind() const override { return NodeType::Float; }
};

struct String : ASTNode {
    std::string data;

    NodeType kind() const override { return NodeType::String; }
};


class Parser {
    public:
    AST parse(const Tokens& t);

    private:
    Tokens tokens;
    size_t at = 0;

    // Same designs as Lexer
    inline Token& peek() { return tokens[at]; }
    inline Token& prev() { return tokens[at - 1]; }

    inline bool end() { return tokens[at].token_type == TokenType::EndOfFile; }
    inline Token& next() { ++at; return prev(); }

    inline bool is(TokenType t) { return peek().token_type == t; }
    inline bool match(TokenType t) {
        if (is(t)) {
            next();
            return true;
        }

        return false;
    }

    AST parse_expression();
    AST parse_term();
    AST parse_factor();
    AST parse_primary(); 
};

}