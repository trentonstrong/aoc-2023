#include <iostream>
#include <fstream>

namespace {
    int parse_calibration_values(std::string &s) {
        std::pair<char, char> digits;
        digits.first = -1;
        digits.second = -1;
        for (char c : s) {
            if (isdigit(c)) {
                if (digits.first == -1) {
                    digits.first = c;
                }
                digits.second = c;
            }
        }
        std::string value{digits.first, digits.second};
        return atoi(value.c_str());
    }
}

int main() {
    std::fstream fs = std::fstream("../inputs/01_calibration.txt", std::ios::in);
    std::string s;
    if (!fs.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        exit(-1);
    }

    int sum = 0;
    while (fs >> s) {
        int value = parse_calibration_values(s);
        std::cout << s << "->" << value << std::endl;
        sum += value;
    }
    std::cout << "Total: " << sum << std::endl;
    return 0;
}
