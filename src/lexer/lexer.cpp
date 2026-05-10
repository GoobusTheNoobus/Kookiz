#include "lexer/lexer.hpp"
#include "diagnostics/diagnostics.hpp"

#include <iostream>

namespace Kookiz {

Tokens Lexer::tokenize(const std::string& code) {
    // Copy the string into internal state
    src = code;
    at = 0;

    Tokens tokens;

    while (!end()) {
        char c = peek();

        if (c == ' ' || c == '\t') 
            skip_spaces();

        else if (c == '\n') {
            ++line;
            next();
        }
            
        // Technically, numbers are allowed in identifier names,
        // however this is looking at the first character of the 
        // identifier, which does NOT allow a number
        else if (std::isalpha(c) || c == '_') {
            tokens.push_back(make_identifier_or_keyword());
        }
            

        else if (std::isdigit(c)) 
            tokens.push_back(make_number());
        
        else if (c == '"') 
            tokens.push_back(make_string());
        
        else {
            tokens.push_back(make_operator());
        }

    }

    tokens.push_back(Token::end_of_file());

    return tokens;

}

Token Lexer::make_identifier_or_keyword() {
    // An identifier can include: 
    // - Numbers
    // - Letters
    // - Underscores

    Token token{"", TokenType::Identifier};

    while (!end()) {
        char c = peek();

        
        if (std::isalnum(c) || c == '_') {
            token.string.push_back(c);
            next();
        }

        else break;
    }

    if (token.string == "ret") token.token_type = TokenType::KwdReturn;
    if (token.string == "fn") token.token_type = TokenType::KwdFn;
    if (token.string == "var") token.token_type = TokenType::KwdVar;
    if (token.string == "true") token.token_type = TokenType::KwdTrue;
    if (token.string == "false") token.token_type = TokenType::KwdFalse;
    if (token.string == "null") token.token_type = TokenType::KwdNull;


    return token;
}

Token Lexer::make_number() {
    Token token{"", TokenType::IntegerLiteral};

    while (!end()) {
        char c = peek();

        next();

        // We handle minus sign as operator
        if (std::isdigit(c))
        token.string.push_back(c);

        else if (c == '.') {
            token.token_type = TokenType::FloatLiteral;
            token.string.push_back(c);
        }
        else break;

    }

    return token;
}

Token Lexer::make_string() {
    Token token{"", TokenType::StringLiteral};

    // Skip first quote
    next();

    while (!end()) {
        char c = peek();

        next();

        if (c == '"') break;

        // We don't allow multi-line strings
        else if (c == '\n') {
            std::cout << "Error at line " << line << "\n";
            std::cout << "Syntax Error: Unterminated string\n";
        }

        token.string.push_back(c);
    }

    return token;
}


// All operators are single character for now
Token Lexer::make_operator() {

    char c = peek();

    Token token{std::string(1, c), TokenType::None};

    if (c == '(') token.token_type = TokenType::OBracketRound;
    else if (c == ')') token.token_type = TokenType::CBracketRound;
    else if (c == '[') token.token_type = TokenType::OBracketSquare;
    else if (c == ']') token.token_type = TokenType::CBracketSquare;
    else if (c == '{') token.token_type = TokenType::OBracketBrace;
    else if (c == '}') token.token_type = TokenType::CBracketBrace;
    else if (c == '+') token.token_type = TokenType::OperAdd;
    else if (c == '-') token.token_type = TokenType::OperMinus;
    else if (c == '*') token.token_type = TokenType::OperStar;
    else if (c == '/') token.token_type = TokenType::OperSlash;
    else if (c == '=') token.token_type = TokenType::OperEqual;
    else if (c == ';') token.token_type = TokenType::OperSemicolon;
    else if (c == '.') token.token_type = TokenType::OperDot;
    else if (c == ':') token.token_type = TokenType::OperColon;
    else {
        std::string m = "Unknown symbol: ";
        m.push_back(c);
        error(line, Error::SyntaxError, m);
    }
    

    next();

    return token;
}

}