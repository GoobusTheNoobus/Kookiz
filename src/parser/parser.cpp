#include "parser.hpp"

namespace Kookiz {

AST Parser::parse_primary() {
    if (match(TokenType::IntegerLiteral)) {
        
        return std::make_unique<Integer>(
            std::stoll(prev().string)
        );
    }
}

}