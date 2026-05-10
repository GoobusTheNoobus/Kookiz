#include <iostream>
#include <string>
#include <fstream>

#include "lexer/lexer.hpp"
using namespace Kookiz;

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

        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cout << "[31mFailed to open file " << filename << "[0m]";
            return 1; 
        }

        Lexer lexer;

        std::string code = std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );

        std::cout << "Contents of " << filename << std::endl;
        std::cout << "======================================\n";
        std::cout << code << std::endl << std::endl;
 
        Tokens tokens = lexer.tokenize(code);

        std::cout << "Tokens: \n";
        std::cout << tokens;
    }

}



/*
int main() {
    std::string code = "print(\"Hello World\")345 654.432 @#";

    Lexer lexer;

    std::cout << lexer.tokenize(code);
}
*/

