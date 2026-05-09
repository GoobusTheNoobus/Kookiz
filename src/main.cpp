#include <iostream>
#include <string>

#include "lexer/lexer.hpp"

/*
int main(int argc, char** argv) {
    
    // Command line shell
    if (argc == 1) {
        while (true) {
            std::cout << ">> ";
            
            std::string command;
            std::getline(std::cin, command);

            // TODO: parse line
        }

        return 0;
    }

    if (argc > 1) {
        std::string filename = argv[1];

        // Add .kkz extension if not provided
        if ((filename.length() >= 4) && (filename.substr(filename.length() - 4, 4) != ".kkz")) {
            filename.append(".kkz");
        }

        std::cout << "Running " << filename << std::endl;
    }

}

*/

using namespace Kookiz;

int main() {
    Token token;
    token.string = "3";
    token.token_type = TokenType::NumberLiteral;

    std::cout << token.repr() << std::endl;
}