#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "util/FileUtils.h"

auto loadMaze() {
    std::stringstream data = FileUtils::loadFile("day11/galaxy.txt");

    std::vector<std::pair<int, int>> occupied = {};

    std::string line;
    int row = 0;
    while (std::getline(data, line)) {
        int col = 0;
        for (auto& c : line) {
            if (c == '#') {
                occupied.emplace_back(row, col);
            }
            col++;
        }
        row++;
    }

    std::set<int> emptyRows = {};
    std::set<int> emptyCols = {};

    auto height = row;
    auto width = line.size();

    for (int i = 0; i < height; i++) {
        if (std::ranges::find(occupied, i, [](auto& p) { return p.first; }) == occupied.end()) {
            emptyRows.insert(i);
        }
    }

    for (int i = 0; i < width; i++) {
        if (std::ranges::find(occupied, i, [](auto& p) { return p.second; }) == occupied.end()) {
            emptyCols.insert(i);
        }
    }

    return std::tuple(occupied, emptyRows, emptyCols);
}

auto calculteMinDistance(std::vector<std::pair<int, int>>& occupied, std::set<int>& emptyRows, std::set<int>& emptyCols,
                         int expansion) {
    long long sum_min_distances = 0;

    for (auto it1 = occupied.begin(); it1 != occupied.end(); ++it1) {
        for (auto it2 = (it1 + 1); it2 != occupied.end(); ++it2) {
            auto [y1, x1] = *it1;
            auto [y2, x2] = *it2;

            auto upper_left = std::pair(std::min(y1, y2), std::min(x1, x2));
            auto lower_right = std::pair(std::max(y1, y2), std::max(x1, x2));

            // count empoty rows between x1 and x2
            long long emptyRowsBetween = std::count_if(
                emptyRows.begin(), emptyRows.end(),
                [upper_left, lower_right](auto& row) { return row > upper_left.first && row < lower_right.first; });

            // count empoty cols between y1 and y2
            long long emptyColsBetween = std::count_if(
                emptyCols.begin(), emptyCols.end(),
                [upper_left, lower_right](auto& col) { return col > upper_left.second && col < lower_right.second; });

            long long distance = std::abs(x1 - x2) + std::abs(y1 - y2);

            distance += (emptyRowsBetween + emptyColsBetween) * (expansion - 1);

            sum_min_distances += distance;
        }
    }

    return sum_min_distances;
}

void part1() {
    auto [occupied, emptyRows, emptyCols] = loadMaze();

    auto minDistance = calculteMinDistance(occupied, emptyRows, emptyCols, 2);

    std::cout << "Min distance: " << minDistance << std::endl;
}

void part2() {
    auto [occupied, emptyRows, emptyCols] = loadMaze();

    auto minDistance = calculteMinDistance(occupied, emptyRows, emptyCols, 1000000);

    std::cout << "Min distance: " << minDistance << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();

    std ::cout << "P2: " << std::endl;
    part2();
}