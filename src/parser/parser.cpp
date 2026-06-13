#include "parser.hpp"
#include "diagnostics/diagnostics.hpp"

namespace Kookiz {

AST Parser::parse(const Tokens& t) {
    tokens = t;
    at = 0;
    return parse_program();
}

AST Parser::parse_program() {
    auto root = std::make_unique<Root>(0);

    while (!end()) {
        if (peek().token_type == TokenType::EndOfFile) break;
        root->children.push_back(parse_declaration());
    }

    return root;
}

AST Parser::parse_declaration() {
    if (match(TokenType::KwdFn)) return parse_function_declaration();
    if (match(TokenType::KwdVar)) return parse_variable_declaration();
    return parse_statement();
}

AST Parser::parse_statement() {
    if (match(TokenType::KwdReturn)) return parse_return_statement();
    return parse_expression_statement();
}

AST Parser::parse_function_declaration() {
    if (!match(TokenType::Identifier)) {
        error(peek().line, Error::SyntaxError, "Expected function name after fn");
    }

    std::string name = prev().string;
    consume(TokenType::OBracketRound, "Expected '(' after function name");

    std::vector<std::string> parameters;
    if (!check(TokenType::CBracketRound)) {
        do {
            if (!match(TokenType::Identifier)) {
                error(peek().line, Error::SyntaxError, "Expected parameter name in function declaration");
            }
            parameters.push_back(prev().string);
        } while (match(TokenType::OperComma));
    }

    consume(TokenType::CBracketRound, "Expected ')' after function parameters");
    AST body = parse_block();

    auto functionDecl = std::make_unique<FunctionDeclaration>(prev().line);
    functionDecl->name = std::move(name);
    functionDecl->parameters = std::move(parameters);
    functionDecl->body = std::move(body);
    return functionDecl;
}

AST Parser::parse_variable_declaration() {
    if (!match(TokenType::Identifier)) {
        error(peek().line, Error::SyntaxError, "Expected variable name after var");
    }

    std::string name = prev().string;
    consume(TokenType::OperEqual, "Expected '=' after variable name");
    AST initializer = parse_expression();
    consume(TokenType::OperSemicolon, "Expected ';' after variable declaration");

    auto decl = std::make_unique<VariableDeclaration>(prev().line);
    decl->name = std::move(name);
    decl->initializer = std::move(initializer);
    return decl;
}

AST Parser::parse_return_statement() {
    AST value = nullptr;
    if (!match(TokenType::OperSemicolon)) {
        value = parse_expression();
        consume(TokenType::OperSemicolon, "Expected ';' after return statement");
    }

    auto ret = std::make_unique<ReturnStatement>(prev().line);
    ret->value = std::move(value);
    return ret;
}

AST Parser::parse_expression_statement() {
    AST expr = parse_expression();
    consume(TokenType::OperSemicolon, "Expected ';' after expression");

    auto stmt = std::make_unique<ExpressionStatement>(prev().line);
    stmt->expression = std::move(expr);
    return stmt;
}

AST Parser::parse_block() {
    consume(TokenType::OBracketBrace, "Expected '{' to begin block");

    auto block = std::make_unique<Block>(prev().line);
    while (!check(TokenType::CBracketBrace) && !end()) {
        block->children.push_back(parse_declaration());
    }

    consume(TokenType::CBracketBrace, "Expected '}' to close block");
    return block;
}

AST Parser::parse_expression() {
    return parse_assignment();
}

AST Parser::parse_assignment() {
    AST expr = parse_term();

    if (match(TokenType::OperEqual)) {
        AST value = parse_expression();

        if (expr->kind() == NodeType::Identifier) {
            auto assignment = std::make_unique<BinaryExpression>(prev().line);
            assignment->child1 = std::move(expr);
            assignment->child2 = std::move(value);
            assignment->op = TokenType::OperEqual;
            return assignment;
        }

        error(prev().line, Error::SyntaxError, "Invalid assignment target");
    }

    return expr;
}

AST Parser::parse_term() {
    AST expr = parse_factor();

    while (match_any({TokenType::OperAdd, TokenType::OperMinus})) {
        TokenType op = prev().token_type;
        AST right = parse_factor();

        auto binary = std::make_unique<BinaryExpression>(prev().line);
        binary->child1 = std::move(expr);
        binary->child2 = std::move(right);
        binary->op = op;
        expr = std::move(binary);
    }

    return expr;
}

AST Parser::parse_factor() {
    AST expr = parse_unary();

    while (match_any({TokenType::OperStar, TokenType::OperSlash})) {
        TokenType op = prev().token_type;
        AST right = parse_unary();

        auto binary = std::make_unique<BinaryExpression>(prev().line);
        binary->child1 = std::move(expr);
        binary->child2 = std::move(right);
        binary->op = op;
        expr = std::move(binary);
    }

    return expr;
}

AST Parser::parse_unary() {
    if (match_any({TokenType::OperMinus, TokenType::OperAdd})) {
        TokenType op = prev().token_type;
        AST right = parse_unary();

        auto unary = std::make_unique<UnaryExpression>(prev().line);
        unary->op = op;
        unary->child = std::move(right);
        return unary;
    }

    return parse_call_member_access();
}

AST Parser::parse_call_member_access() {
    AST expr = parse_primary();

    while (true) {
        if (match(TokenType::OBracketRound)) {
            auto call = std::make_unique<CallExpression>(prev().line);
            call->func = std::move(expr);
            if (!check(TokenType::CBracketRound)) {
                do {
                    call->params.push_back(parse_expression());
                } while (match(TokenType::OperComma));
            }
            consume(TokenType::CBracketRound, "Expected ')' after call arguments");
            expr = std::move(call);
        } else if (match(TokenType::OperDot)) {
            if (!match(TokenType::Identifier)) {
                error(peek().line, Error::SyntaxError, "Expected property name after '.'");
            }
            auto member = std::make_unique<MemberAccessExpression>(prev().line);
            member->obj = std::move(expr);
            member->member = prev().string;
            expr = std::move(member);
        } else {
            break;
        }
    }

    return expr;
}

AST Parser::parse_primary() {
    if (match(TokenType::IntegerLiteral)) {
        return std::make_unique<Integer>(std::stoll(prev().string));
    }

    if (match(TokenType::FloatLiteral)) {
        return std::make_unique<Float>(std::stod(prev().string));
    }

    if (match(TokenType::StringLiteral)) {
        return std::make_unique<String>(prev().string);
    }

    if (match(TokenType::Identifier)) {
        auto id = std::make_unique<Identifier>(prev().line);
        id->data = prev().string;
        return id;
    }

    if (match(TokenType::KwdTrue)) {
        auto lit = std::make_unique<Identifier>(prev().line);
        lit->data = "true";
        return lit;
    }

    if (match(TokenType::KwdFalse)) {
        auto lit = std::make_unique<Identifier>(prev().line);
        lit->data = "false";
        return lit;
    }

    if (match(TokenType::KwdNull)) {
        auto lit = std::make_unique<Identifier>(prev().line);
        lit->data = "null";
        return lit;
    }

    if (match(TokenType::OBracketRound)) {
        AST expr = parse_expression();
        consume(TokenType::CBracketRound, "Expected ')' after expression");
        return expr;
    }

    error(peek().line, Error::SyntaxError, "Expected expression");
    return nullptr;
}

}