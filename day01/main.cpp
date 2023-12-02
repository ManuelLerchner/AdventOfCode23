#include <cctype>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>

#include "util/FileUtils.h"

bool substring_starts_at(const std::string& str, const std::string& substr, int pos) {
    if (pos + substr.size() > str.size()) {
        return false;
    }

    for (auto i = 0; i < substr.size(); i++) {
        if (str[pos + i] != substr[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    auto input = FileUtils::loadFile("day01/calibration_data.txt");

    std::unordered_map<std::string, int> digits = {{"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
                                                   {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

    auto find_combination = [&digits](std::string& line) {
        int res = 0;

        auto first = -1;
        auto last = -1;

        for (auto i = 0; i < line.size(); i++) {
            if (std::isdigit(line[i])) {
                first = line[i] - '0';
                break;
            }
            for (auto& [key, value] : digits) {
                if (substring_starts_at(line, key, i)) {
                    first = value;
                    goto end1;
                }
            }
        }
    end1:

        for (auto i = line.size() - 1; i >= 0; i--) {
            if (std::isdigit(line[i])) {
                last = line[i] - '0';
                break;
            }
            for (auto& [key, value] : digits) {
                if (substring_starts_at(line, key, i)) {
                    last = value;
                    goto end2;
                }
            }
        }
    end2:

        return 10 * first + last;
    };

    auto sum = 0;

    for (std::string line; std::getline(input, line);) {
        auto combination = find_combination(line);

        std::cout << combination << "\t|\t" << line << std::endl;
        sum += combination;
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}