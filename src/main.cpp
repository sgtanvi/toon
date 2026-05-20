#include "toon/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[]) {
    try {
        std::string input;

        if (argc > 1) {
            std::ifstream file(argv[1]);
            if (!file.is_open()) {
                std::cerr << "Error: cannot open '" << argv[1] << "'\n";
                return 1;
            }
            std::ostringstream buf;
            buf << file.rdbuf();
            input = buf.str();
        } else {
            std::ostringstream buf;
            buf << std::cin.rdbuf();
            input = buf.str();
        }

        toon::Value value = toon::parse(input);
        std::cout << toon::stringifyPretty(value) << '\n';
        return 0;

    } catch (const toon::ParseError& e) {
        std::cerr << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
