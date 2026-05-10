#pragma once

#include <cstdint>
#include <iostream>

namespace Kookiz {

enum class Error: int8_t {
    SyntaxError,
    MathError
};

inline std::string error_message(Error err) {
    switch (err) {
        case Error::SyntaxError: return "SyntaxError";
        case Error::MathError: return "MathError";
        default: return "Errors";
    }
}

void error(int line, Error err, const std::string& message);
}