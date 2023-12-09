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

struct Hands {
    std::string cards;
    long bid;
};

std::string trim(std::string& str) {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);

    return str;
}

auto parse(std::stringstream& s) {
    auto lines = splitString(s.str(), "\n");

    std::vector<Hands> hands = {};
    for (auto& line : lines) {
        auto elems = splitString(line, " ");

        if (elems.size() == 0) continue;

        hands.push_back({elems[0], std::stol(elems[1])});
    }

    return hands;
}

auto handValue(std::string hand) {
    auto card_strengths = std::map<std::string, long>();
    card_strengths["2"] = 1;
    card_strengths["3"] = 2;
    card_strengths["4"] = 3;
    card_strengths["5"] = 4;
    card_strengths["6"] = 5;
    card_strengths["7"] = 6;
    card_strengths["8"] = 7;
    card_strengths["9"] = 8;
    card_strengths["T"] = 9;
    card_strengths["J"] = 10;
    card_strengths["Q"] = 11;
    card_strengths["K"] = 12;
    card_strengths["A"] = 13;

    long value = 0;
    for (int i = 0; i < hand.length(); i++) {
        auto card = hand[i];
        value += card_strengths[std::string(1, card)] * long(std::pow(10, card_strengths.size() - i - 1));
    }

    return value;
}

auto determineValue(std::string hand) {
    auto hand_strengths = std::map<std::string, int>();
    hand_strengths["Five of a Kind"] = 6;
    hand_strengths["Four of a Kind"] = 6;
    hand_strengths["Full House"] = 5;
    hand_strengths["Three of a Kind"] = 4;
    hand_strengths["Two Pair"] = 3;
    hand_strengths["One Pair"] = 2;
    hand_strengths["High Card"] = 1;

    auto occurences = std::map<char, int>();

    for (auto& card : hand) {
        occurences[card]++;
    }

    // five of a kind
    for (auto& [card, count] : occurences) {
        if (count == 5) {
            return std::make_pair(hand_strengths["Five of a Kind"], handValue(hand));
        }
    }

    // four of a kind
    for (auto& [card, count] : occurences) {
        if (count == 4) {
            return std::make_pair(hand_strengths["Four of a Kind"], handValue(hand));
        }
    }

    // full house
    bool has_three = false;
    bool has_two = false;
    for (auto& [card, count] : occurences) {
        if (count == 3) {
            has_three = true;
        }
        if (count == 2) {
            has_two = true;
        }
    }

    if (has_three && has_two) {
        return std::make_pair(hand_strengths["Full House"], handValue(hand));
    }

    // three of a kind
    for (auto& [card, count] : occurences) {
        if (count == 3) {
            return std::make_pair(hand_strengths["Three of a Kind"], handValue(hand));
        }
    }

    // two pair
    int num_pairs = 0;
    for (auto& [card, count] : occurences) {
        if (count == 2) {
            num_pairs++;
        }
    }

    if (num_pairs == 2) {
        return std::make_pair(hand_strengths["Two Pair"], handValue(hand));
    }

    // one pair
    for (auto& [card, count] : occurences) {
        if (count == 2) {
            return std::make_pair(hand_strengths["One Pair"], handValue(hand));
        }
    }

    // high card
    return std::make_pair(hand_strengths["High Card"], handValue(hand));
}

void part1() {
    std::stringstream file = FileUtils::loadFile("day07/graph.txt");

    auto hands = parse(file);

    std::vector<std::pair<double, Hands>> combined;

    for (auto& hand : hands) {
        auto value = determineValue(hand.cards);

        auto v = value.first * std::pow(10, 5) + value.second;

        auto p = std::make_pair(v, hand);

        combined.push_back(p);
    }

    std::sort(combined.begin(), combined.end(), [](auto& a, auto& b) { return a.first < b.first; });

    auto total_win = 0;
    for (int i = 0; i < combined.size(); i++) {
        std::cout << "Rank " << i + 1 << ": " << combined[i].second.bid << " -> " << combined[i].second.cards
                  << " value " << combined[i].first << std::endl;

        total_win += (i + 1) * combined[i].second.bid;
    }

    std::cout << total_win << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();
}