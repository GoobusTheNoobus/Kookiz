#include "diagnostics/diagnostics.hpp"

namespace Kookiz {

void error(int line, Error err, const std::string& message) {
    std::cerr << "\033[31m" << error_message(err) << "\033[0m" << " at line " << line << std::endl;
    std::cerr << message << std::endl;
    std::abort();
}
}