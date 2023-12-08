#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
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

struct Game {
    long time;
    long distance;
};

std::string trim(std::string& str) {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);

    return str;
}

auto parse(std::stringstream& s) {
    std::string line1;
    std::getline(s, line1);

    std::string line2;
    std::getline(s, line2);

    auto elems1 = splitString(line1, " ");
    auto elems2 = splitString(line2, " ");

    std::vector<Game> games = {};

    // filter out empty strings
    elems1.erase(std::remove_if(elems1.begin(), elems1.end(), [](std::string& s) { return s.length() == 0; }),
                 elems1.end());
    elems2.erase(std::remove_if(elems2.begin(), elems2.end(), [](std::string& s) { return s.length() == 0; }),
                 elems2.end());

    for (int i = 1; i < elems1.size(); i++) {
        auto& elem1 = elems1[i];
        auto& elem2 = elems2[i];

        auto time = std::stol(elem1);
        auto distance = std::stol(elem2);

        games.push_back({time, distance});
    }

    return games;
}

void part1(std::string filename) {
    std::stringstream file = FileUtils::loadFile(filename);

    auto games = parse(file);

    auto prod = 1;

    for (auto& game : games) {
        auto lower = (-game.time - std::sqrt(game.time * game.time - 4 * game.distance)) / 2;
        auto upper = (-game.time + std::sqrt(game.time * game.time - 4 * game.distance)) / 2;

        auto nums_between = std::floor(upper) - std::ceil(lower) + 1;

        // exclude the first and last number
        if (std::floor(upper) == upper) nums_between--;
        if (std::ceil(lower) == lower) nums_between--;

        std::cout << nums_between << std::endl;

        prod *= nums_between;
    }

    std::cout << "prod: " << prod << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1("day06/game1.txt");

    std::cout << "P2: " << std::endl;
    part1("day06/game2.txt");
}