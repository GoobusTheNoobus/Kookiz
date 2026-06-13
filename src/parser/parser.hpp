#pragma once

#include "diagnostics/diagnostics.hpp"
#include "lexer/lexer.hpp"

#include <initializer_list>
#include <memory>
#include <vector>
#include <string>

namespace Kookiz {

enum class NodeType: int8_t {
    Root,
    Block,

    Identifier,
    Keyword,
    Integer,
    Float,
    String,

    ExpressionStatement,
    ReturnStatement,
    VariableDeclaration,
    FunctionDeclaration,

    UnaryExpression,
    BinaryExpression,
    CallExpression,
    MemberAccessExpression,
    
    None
};

struct ASTNode {
    int line = 1;

    ASTNode() = default;
    explicit ASTNode(int line_) : line(line_) {}
    virtual ~ASTNode() = default;

    virtual NodeType kind() const { return NodeType::None; }
};

using AST = std::unique_ptr<ASTNode>;

struct UnaryExpression : ASTNode {
    TokenType op;
    AST child;

    NodeType kind() const override { return NodeType::UnaryExpression; }
};

struct BinaryExpression : ASTNode {
    AST child1;
    AST child2;
    TokenType op;

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

struct ExpressionStatement : ASTNode {
    AST expression;

    NodeType kind() const override { return NodeType::ExpressionStatement; }
};

struct ReturnStatement : ASTNode {
    AST value;

    NodeType kind() const override { return NodeType::ReturnStatement; }
};

struct VariableDeclaration : ASTNode {
    std::string name;
    AST initializer;

    NodeType kind() const override { return NodeType::VariableDeclaration; }
};

struct FunctionDeclaration : ASTNode {
    std::string name;
    std::vector<std::string> parameters;
    AST body;

    NodeType kind() const override { return NodeType::FunctionDeclaration; }
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
    TokenType token_type = TokenType::None;
    
    NodeType kind() const override { return NodeType::Keyword; }
};

struct Integer : ASTNode {
    int64_t data = 0;

    explicit Integer(int64_t d) : data(d) {}
    NodeType kind() const override { return NodeType::Integer; }
};

struct Float : ASTNode {
    double data = 0.0;

    explicit Float(double d) : data(d) {}
    NodeType kind() const override { return NodeType::Float; }
};

struct String : ASTNode {
    std::string data;

    explicit String(std::string d) : data(std::move(d)) {}
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
    inline bool check(TokenType t) { return !end() && peek().token_type == t; }
    inline bool match(TokenType t) {
        if (is(t)) {
            next();
            return true;
        }

        return false;
    }
    inline bool match_any(std::initializer_list<TokenType> types) {
        for (TokenType type : types) {
            if (is(type)) {
                next();
                return true;
            }
        }
        return false;
    }
    inline void consume(TokenType t, const std::string& message) {
        if (!match(t)) {
            error(peek().line, Error::SyntaxError, message);
        }
    }

    AST parse_program();
    AST parse_declaration();
    AST parse_statement();
    AST parse_block();
    AST parse_function_declaration();
    AST parse_variable_declaration();
    AST parse_return_statement();
    AST parse_expression_statement();

    AST parse_expression();
    AST parse_assignment();
    AST parse_term();
    AST parse_factor();
    AST parse_unary();
    AST parse_call_member_access();
    AST parse_primary();
};

}