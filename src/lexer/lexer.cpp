#include "lexer/lexer.hpp"

namespace Kookiz {

Tokens Lexer::tokenize(const std::string& code) {
    // Copy the string into internal state
    src = code;
    at = 0;

    Tokens tokens;

    while (!end()) {
        
    }

}

}