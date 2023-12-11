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
    std::stringstream f1 = FileUtils::loadFile("day10/sample.txt");

    std::vector<std::vector<char>> maze;

    std::pair<int, int> start;

    while (!f1.eof()) {
        std::string line;
        std::getline(f1, line);

        std::vector<char> row;
        for (auto& c : line) {
            row.push_back(c);
            if (c == 'S') {
                start = {maze.size(), row.size() - 1};
            }
        }

        maze.push_back(row);
    }

    return std::pair(maze, start);
}

bool isInside(std::vector<std::vector<char>>& maze, std::pair<int, int> pos) {
    auto [x, y] = pos;

    return x >= 0 && y >= 0 && x < maze.size() && y < maze[0].size();
}

auto go_first_step(std::vector<std::vector<char>>& maze, std::pair<int, int> curr) {
    bool can_go_left = isInside(maze, {curr.first, curr.second - 1});
    bool can_go_right = isInside(maze, {curr.first + 1, curr.second});
    bool can_go_top = isInside(maze, {curr.first, curr.second - 1});
    bool can_go_bottom = isInside(maze, {curr.first, curr.second + 1});

    if (can_go_left) {
        std::set<char> allowed_chars = {'-', 'L', 'F'};
        if (allowed_chars.contains(maze[curr.first][curr.second - 1])) {
            return std::pair(curr.first, curr.second - 1);
        }
    }

    if (can_go_right) {
        std::set<char> allowed_chars = {'-', 'J', '7'};
        if (allowed_chars.contains(maze[curr.first][curr.second + 1])) {
            return std::pair(curr.first, curr.second + 1);
        }
    }

    if (can_go_top) {
        std::set<char> allowed_chars = {'|', '7', 'F'};
        if (allowed_chars.contains(maze[curr.first - 1][curr.second])) {
            return std::pair(curr.first - 1, curr.second);
        }
    }

    if (can_go_bottom) {
        std::set<char> allowed_chars = {'|', 'L', 'J'};
        if (allowed_chars.contains(maze[curr.first + 1][curr.second])) {
            return std::pair(curr.first + 1, curr.second);
        }
    }

    return std::pair(-1, -1);
}

auto validSteps(char c) {
    auto TOP = std::pair(-1, 0);
    auto BOTTOM = std::pair(1, 0);
    auto LEFT = std::pair(0, -1);
    auto RIGHT = std::pair(0, 1);

    if (c == '|') {
        return std::vector{TOP, BOTTOM};
    } else if (c == '-') {
        return std::vector{LEFT, RIGHT};
    } else if (c == 'L') {
        return std::vector{TOP, RIGHT};
    } else if (c == 'J') {
        return std::vector{LEFT, TOP};
    } else if (c == '7') {
        return std::vector{BOTTOM, LEFT};
    } else if (c == 'F') {
        return std::vector{BOTTOM, RIGHT};
    }

    return std::vector<std::pair<int, int>>{};
}

int find_loop(std::vector<std::vector<char>>& maze, std::pair<int, int> curr, std::set<std::pair<int, int>>& visited) {
    for (auto& dir : validSteps(maze[curr.first][curr.second])) {
        auto next = std::pair(curr.first + dir.first, curr.second + dir.second);

        if (!isInside(maze, next)) continue;

        visited.insert(next);

        if (maze[next.first][next.second] == 'S') {
            std::cout << "Found loop with depth " << visited.size() << std::endl;
            return visited.size();
        }

        return find_loop(maze, next, visited);
    }

    return -1;
}

void part1() {
    auto [maze, start] = loadMaze();

    auto first_step = go_first_step(maze, start);

    std::set<std::pair<int, int>> visited = {start, first_step};

    auto loop = find_loop(maze, first_step, 1);

    std::cout << "Loop size: " << loop << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();
}