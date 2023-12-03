#include <cctype>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "util/FileUtils.h"

bool is_valid_pos(int x, int y, std::vector<std::string>& lines) {
    if (x < 0 || y < 0) {
        return false;
    }
    if (y >= lines.size() || x >= lines[y].length()) {
        return false;
    }
    return true;
}

auto extract_digit_near(int y, int x, std::vector<std::string>& lines,
                        std::set<std::pair<int, int>>& already_considered) {
    std::vector<int> neighbours = {};

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            auto px = x + i;
            auto py = y + j;

            if (!is_valid_pos(px, py, lines)) {
                continue;
            }

            if (!std::isdigit(lines[py][px])) {
                continue;
            }

            // search left for start of number
            bool isValid = true;

            auto curr_x = px;
            auto last_start = px;
            while (is_valid_pos(curr_x, py, lines) && std::isdigit(lines[py][curr_x])) {
                if (already_considered.contains(std::make_pair(py, curr_x))) {
                    isValid = false;
                    break;
                }
                last_start = curr_x;
                curr_x--;
            }

            // right for end of number
            curr_x = px;
            auto last_end = px;
            while (is_valid_pos(curr_x, py, lines) && std::isdigit(lines[py][last_end])) {
                if (already_considered.contains(std::make_pair(py, curr_x))) {
                    isValid = false;
                    break;
                }
                last_end = curr_x;
                curr_x++;
            }

            if (!isValid) {
                continue;
            }

            for (auto i = last_start; i <= last_end; i++) {
                already_considered.insert(std::make_pair(py, i));
            }

            auto number_string = lines[py].substr(last_start, last_end - last_start + 1);
            auto number = std::stoi(number_string);

            neighbours.push_back(number);
        }
    }

    int neighgbour_sum = std::accumulate(neighbours.begin(), neighbours.end(), 0);

    int gear_ratio = 0;
    if (neighbours.size() == 2 && lines[y][x] == '*') {
        gear_ratio = std::accumulate(neighbours.begin(), neighbours.end(), 1, std::multiplies<>());
    }

    return std::make_pair(neighgbour_sum, gear_ratio);
}

int main() {
    auto input = FileUtils::loadFile("day03/part_numbers.txt");

    std::set<std::pair<int, int>> symbol_pos;

    std::vector<std::string> lines = {};

    int y = 0;
    for (std::string line; std::getline(input, line);) {
        for (size_t x = 0; x < line.length(); x++) {
            auto chr = line[x];

            if (std::isdigit(chr) || chr == '.') {
                continue;
            };

            symbol_pos.insert(std::make_pair(y, x));
        }

        y++;
        lines.push_back(line);
    }

    std::set<std::pair<int, int>> already_considered;

    auto sum = 0;
    auto gear_ratio = 0;
    for (auto& [y, x] : symbol_pos) {
        auto [n_sum, n_gear_ratio] = extract_digit_near(y, x, lines, already_considered);

        sum += n_sum;
        gear_ratio += n_gear_ratio;
    }

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Gear ratio: " << gear_ratio << std::endl;

    return 0;
}