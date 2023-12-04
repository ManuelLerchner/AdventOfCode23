#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "util/FileUtils.h"

void part1() {
    auto ss = FileUtils::loadFile("day04/cards.txt");

    int total = 0;

    while (!ss.eof()) {
        std::string line;
        std::getline(ss, line);

        std::stringstream ss(line);

        std::string card;
        std::getline(ss, card, ':');

        std::string winning_nums;
        std::getline(ss, winning_nums, '|');
        std::stringstream winning_nums_s(winning_nums);

        std::string draws;
        std::getline(ss, draws);
        std::stringstream draws_s(draws);

        std::string num;

        std::set<int> set;
        while (!winning_nums_s.eof()) {
            std::string num;
            std::getline(winning_nums_s, num, ' ');

            if (num.length() < 1) continue;

            auto val = std::stoi(num);
            set.insert(val);
        }

        std::set<int> your_nums;
        int points = 1;
        while (!draws_s.eof()) {
            std::string num;
            std::getline(draws_s, num, ' ');

            if (num.length() < 1) continue;

            int val = std::stoi(num.substr(0, num.length()));

            if (set.contains(val)) {
                your_nums.insert(val);
                points *= 2;
            }
        }

        total += points / 2;
    }

    std::cout << total << std::endl;
}

void part2() {
    std::map<int, int> copies;

    auto temp = FileUtils::loadFile("day04/cards.txt");

    int line_num = 1;
    while (!temp.eof()) {
        std::string line;
        std::getline(temp, line);

        copies.insert(std::make_pair(line_num, 1));
        line_num++;
    }

    auto ss = FileUtils::loadFile("day04/cards.txt");
    line_num = 1;
    while (!ss.eof()) {
        std::string line;
        std::getline(ss, line);

        std::stringstream ss(line);

        std::string card;
        std::getline(ss, card, ':');

        std::string winning_nums;
        std::getline(ss, winning_nums, '|');
        std::stringstream winning_nums_s(winning_nums);

        std::string draws;
        std::getline(ss, draws);
        std::stringstream draws_s(draws);

        std::string num;

        std::set<int> set;
        while (!winning_nums_s.eof()) {
            std::string num;
            std::getline(winning_nums_s, num, ' ');

            if (num.length() < 1) continue;

            auto val = std::stoi(num);
            set.insert(val);
        }

        int count = 0;

        while (!draws_s.eof()) {
            std::string num;
            std::getline(draws_s, num, ' ');

            if (num.length() < 1) continue;

            int val = std::stoi(num.substr(0, num.length()));

            if (set.contains(val)) {
                count++;
            }
        }

        for (int i = line_num + 1; i <= line_num + count; i++) {
            copies[i] += copies[line_num];
        }

        line_num++;
    }

    auto sum_cards = 0;

    for (auto& [key, value] : copies) {
        sum_cards += value;
    }

    std::cout << sum_cards << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();

    std::cout << "P2:" << std::endl;
    part2();
}