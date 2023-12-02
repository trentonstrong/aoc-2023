#include <iostream>
#include <fstream>
#include <regex>
#include <map>

namespace {
    std::map<std::string, std::string> word_to_digit_map = {
            {"one", "1"},
            {"two", "2"},
            {"three", "3"},
            {"four", "4"},
            {"five", "5"},
            {"six", "6"},
            {"seven", "7"},
            {"eight", "8"},
            {"nine", "9"}
    };

    std::string word_to_digit(std::string &s) {
        return word_to_digit_map[s];
    }

    int parse_calibration_values(std::string &s) {
        std::regex digit_re("\\d");
        std::regex word_re("(?=(one|two|three|four|five|six|seven|eight|nine)).");
        std::string first = "";
        int first_pos = INT_MAX;
        std::string last = "";
        int last_pos = -1;

        auto digits_begin = std::sregex_iterator(s.begin(), s.end(), digit_re);
        auto digits_end = std::sregex_iterator();
        for (auto it = digits_begin; it != digits_end; it++) {
            std::smatch m = *it;
            std::string s = m.str();
            int pos = m.position();
            if (pos< first_pos) {
                first = s;
                first_pos = pos;
            }
            if (m.position() > last_pos) {
                last = s;
                last_pos = pos;
            }
        }

        auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_re);
        auto words_end = std::sregex_iterator();
        for (auto it = words_begin; it != words_end; it++) {
            std::smatch m = *it;
            std::string s = m.str(1);
            int pos = m.position();
            if (pos < first_pos) {
                first = word_to_digit(s);
                first_pos = pos;
            }
            if (pos > last_pos) {
                last = word_to_digit(s);
                last_pos = pos;
            }
        }

        std::string value = first + last;
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
