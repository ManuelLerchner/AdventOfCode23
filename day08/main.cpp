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

struct Vertice {
    std::string name;
    std::vector<std::string> connections;
};

auto parse(std::stringstream& s) {
    auto lines = splitString(s.str(), "\n");

    auto sequence = lines[0];

    std::map<std::string, std::vector<std::string>> graph;
    for (int i = 2; i < lines.size(); i++) {
        auto& line = lines[i];

        auto elems = splitString(line, " = ");

        auto seq = elems[1].substr(1, elems[1].size() - 2);

        auto seq_elems = splitString(seq, ", ");

        graph[elems[0]] = seq_elems;
    }

    return std::pair(sequence, graph);
}

int dir2index(char dir) {
    if (dir == 'L') {
        return 0;
    } else if (dir == 'R') {
        return 1;
    }
    return -1;
}

auto step(const std::string& curr, const std::map<std::string, std::vector<std::string>>& graph, const std::string& seq,
          long iter) {
    auto& connections = graph.at(curr);

    auto cur_dir = seq[iter % seq.size()];
    auto dir_index = dir2index(cur_dir);

    auto next = connections[dir_index];

    return next;
}

void part1() {
    std::stringstream file = FileUtils::loadFile("day08/graph.txt");

    auto [seq, graph] = parse(file);

    auto start = "AAA";
    auto end = "ZZZ";

    std::string curr = start;

    auto iter = 0;

    while (true) {
        curr = step(curr, graph, seq, iter++);
        if (curr == end) {
            break;
        }
    }

    std::cout << "Total steps: " << iter << std::endl;
}

void part2() {
    std::stringstream file = FileUtils::loadFile("day08/graph.txt");

    auto [seq, graph] = parse(file);

    std::set<std::string> ends_with_a = {};
    std::set<std::string> ends_with_z = {};

    for (auto& [key, value] : graph) {
        if (key[key.size() - 1] == 'A') {
            ends_with_a.insert(key);
        } else if (key[key.size() - 1] == 'Z') {
            ends_with_z.insert(key);
        }
    }

    std::set<std::string> curr = ends_with_a;

    auto iter = 0;

    while (true) {
        auto cur_dir = seq[iter % seq.size()];
        auto dir_index = dir2index(cur_dir);

        std::set<std::string> next = {};

        for (auto& c : curr) {
            auto& connections = graph[c];

            auto next_p = connections[dir_index];

            next.insert(next_p);
        }

        curr = next;
        iter++;

        bool allEnd = true;
        for (auto& c : curr) {
            if (!ends_with_z.contains(c)) {
                allEnd = false;
                break;
            }
        }

        if (allEnd) {
            break;
        }
    }

    std::cout << "Total steps: " << iter << std::endl;
}

auto calculate_path_length(const std::string& start, const std::map<std::string, std::vector<std::string>>& graph,
                           const std::string& seq) {
    auto curr = start;

    long iter_z = 0;
    while (curr[2] != 'Z') {
        curr = step(curr, graph, seq, iter_z++);
    }

    auto end = curr;

    long steps = 0;
    while (curr != end || steps == 0) {
        curr = step(curr, graph, seq, steps++);
    }

    return steps;
}

auto lcm(const std::vector<long>& numbers) {
    if (numbers.size() < 2) return 0l;
    long lcm = std::lcm(numbers[0], numbers[1]);
    for (std::size_t i{2}; i < numbers.size(); ++i) lcm = std::lcm(lcm, numbers[i]);
    return lcm;
}

void part2_lcm() {
    std::stringstream file = FileUtils::loadFile("day08/graph.txt");

    auto [seq, graph] = parse(file);

    std::set<std::string> ends_with_a = {};

    for (auto& [key, value] : graph) {
        if (key[key.size() - 1] == 'A') {
            ends_with_a.insert(key);
        }
    }

    std::vector<long> lengths = {};
    for (auto& c : ends_with_a) {
        auto length = calculate_path_length(c, graph, seq);
        lengths.push_back(length);
    }

    auto l = lcm(lengths);

    std::cout << "Total steps: " << l << std::endl;
}

auto solvePart2() {
    std::stringstream file = FileUtils::loadFile("day08/graph.txt");

    auto [seq, graph] = parse(file);

    std::set<std::string> ends_with_a = {};

    for (auto& [key, value] : graph) {
        if (key[key.size() - 1] == 'A') {
            ends_with_a.insert(key);
        }
    }

    auto currentNodes = ends_with_a;
    std::vector<long> cycleLengths;
    cycleLengths.reserve(currentNodes.size());
    for (const auto& node : currentNodes) cycleLengths.emplace_back(calculate_path_length(node, graph, seq));

    return lcm(cycleLengths);
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();

    solvePart2();

    std::cout << "P2_fast:" << std::endl;
    part2_lcm();

    std::cout << "P2:" << std::endl;
    part2();
}