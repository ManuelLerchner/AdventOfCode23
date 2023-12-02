#include <cctype>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>

#include "util/FileUtils.h"

auto parseDraw(std::string& draw) {
    std::stringstream choice_s(draw);
    std::string choice;

    std::getline(choice_s, choice, ' ');
    std::getline(choice_s, choice, ' ');

    auto value = choice;

    std::getline(choice_s, choice);

    auto key = choice;

    return std::make_pair(key, std::stoi(value));
}

auto parseLine(std::string& line) {
    std::stringstream s(line);

    std::string game;
    std::getline(s, game, ':');

    std::stringstream game_s(game);
    std::string id;
    std::getline(game_s, id, ' ');
    std::getline(game_s, id);

    std::string run;

    std::map<std::string, int> maximum_map;

    while (!s.eof()) {
        std::getline(s, run, ';');

        std::stringstream draw_s(run);
        std::string draw;

        std::map<std::string, int> draw_map;

        while (!draw_s.eof()) {
            std::getline(draw_s, draw, ',');

            auto d = parseDraw(draw);
            draw_map.insert(d);
        }

        for (auto& [key, value] : draw_map) {
            if (!maximum_map.contains(key)) {
                maximum_map.insert(std::make_pair(key, value));
            } else {
                auto old_value = *maximum_map.find(key);
                maximum_map[key] = std::max(value, old_value.second);
            }
        }
    }

    return std::make_pair(std::stoi(id), maximum_map);
}

auto isValidSubset(std::map<std::string, int> max_ocurences, std::map<std::string, int> to_search) {
    for (auto& [key, value] : max_ocurences) {
        auto p = to_search.find(key);

        if (p != to_search.end()) {
            if (p->second < value) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    auto input = FileUtils::loadFile("day02/records.txt");

    std::map<std::string, int> to_search = {{"red", 12}, {"green", 13}, {"blue", 14}};

    auto sum = 0;
    auto sum_power = 0;

    for (std::string line; std::getline(input, line);) {
        auto [id, max_ocurences] = parseLine(line);

        if (isValidSubset(max_ocurences, to_search)) {
            std::cout << line << std::endl;
            sum += id;
        }

        auto pow = std::transform_reduce(max_ocurences.begin(), max_ocurences.end(), 1, std::multiplies<>(),
                                         [](auto& p) { return p.second; });

        std::cout << "Pow: " << pow << std::endl;

        sum_power += pow;
    }

    std::cout << "Sum of ids: " << sum << std::endl;
    std::cout << "Sum of powers: " << sum_power << std::endl;

    return 0;
}