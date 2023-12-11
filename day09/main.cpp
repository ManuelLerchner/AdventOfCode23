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

auto splitString(std::string_view sv, std::string_view sep) {
    auto parts = std::views::split(sv, sep);

    auto vec = std::views::transform(parts, [](auto&& part) { return std::string(part.begin(), part.end()); });

    return std::vector(vec.begin(), vec.end());
}

auto parse(std::stringstream& s) {
    auto lines = splitString(s.str(), "\n");

    std::vector<std::vector<long>> nums = {};

    for (auto& line : lines) {
        auto elems = splitString(line, " ");

        if (elems.empty()) continue;

        std::vector<long> row = {};

        for (auto& elem : elems) {
            row.push_back(std::stol(elem));
        }

        nums.push_back(row);
    }

    return nums;
}

auto calculateDifferences(std::vector<long>& nums) {
    std::vector<std::vector<long>> differences = {};

    differences.push_back(nums);

    for (int i = 1; i < nums.size(); i++) {
        std::vector<long> row = {};
        bool all_zero = true;
        for (int j = 0; j < differences[0].size() - i; ++j) {
            auto diff = differences[i - 1][j + 1] - differences[i - 1][j];
            if (diff != 0) all_zero = false;

            row.push_back(diff);
        }
        differences.push_back(row);
        if (all_zero) break;
    }

    return differences;
}

auto extrapolateBack(std::vector<std::vector<long>>& differences) {
    differences[differences.size() - 1].push_back(0);

    for (long i = differences.size() - 2; i >= 0; i--) {
        auto& row = differences[i];
        auto& next_row = differences[i + 1];

        auto append = row[row.size() - 1] + next_row[next_row.size() - 1];
        row.push_back(append);
    }

    return differences[0][differences[0].size() - 1];
}

auto extrapolateFront(std::vector<std::vector<long>>& differences) {
    differences[differences.size() - 1].insert(differences[differences.size() - 1].begin(), 0);

    for (long i = differences.size() - 2; i >= 0; i--) {
        auto& row = differences[i];
        auto& next_row = differences[i + 1];

        auto append = row[0] - next_row[0];
        row.insert(row.begin(), append);
    }

    return differences[0][0];
}

void part1() {
    std::stringstream file = FileUtils::loadFile("day09/history.txt");

    auto data = parse(file);

    long sum = 0;
    for (auto& row : data) {
        auto diff = calculateDifferences(row);
        sum += extrapolateBack(diff);
    }

    std::cout << "Sum: " << sum << std::endl;
}

void part2() {
    std::stringstream file = FileUtils::loadFile("day09/history.txt");

    auto data = parse(file);

    auto sum = 0;
    for (auto& row : data) {
        auto diff = calculateDifferences(row);
        sum += extrapolateFront(diff);
    }

    std::cout << "Sum: " << sum << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();

    std::cout << "P2: " << std::endl;
    part2();
}