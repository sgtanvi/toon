#include "toon/json.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace toon {

Value parseFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file: " + path);
    std::ostringstream buf;
    buf << file.rdbuf();
    return parse(buf.str());
}

} // namespace toon
