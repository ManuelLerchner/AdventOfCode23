#include <algorithm>
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

struct Range {
    long start_output;
    long start_input;
    long length;
};

class Map {
   public:
    std::vector<Range> ranges;

    int binary_search_interval(long input, int start, int end) {
        if (start == end) return start;

        int mid = (start + end) / 2;

        auto& range = ranges[mid];

        if (input >= range.start_input && input < range.start_input + range.length) {
            return mid;
        } else if (input < range.start_input) {
            return binary_search_interval(input, start, mid);
        } else {
            return binary_search_interval(input, mid + 1, end);
        }
    }

    long map(long input) {
        auto index = binary_search_interval(input, 0, ranges.size());

        auto& range = ranges[index];

        if (input >= range.start_input && input < range.start_input + range.length) {
            return range.start_output + (input - range.start_input);
        }
        return input;
    }
};

auto parse(std::stringstream& s) {
    auto file_string = s.str();

    auto parts = splitString(file_string, "\n\n");
    auto seeds_line = parts[0];

    auto seeds = splitString(seeds_line, " ");
    std::vector<long> seed_nums = {};
    for (int i = 1; i < seeds.size(); i++) {
        seed_nums.push_back(std::stol(seeds[i]));
    }

    std::vector<std::string> seed_lines = {parts.begin() + 1, parts.end()};

    std::vector<Map> maps = {};

    for (auto& seed : seed_lines) {
        std::stringstream ss(seed);

        std::string line;
        // skip name
        std::getline(ss, line);

        Map m = {};

        while (!ss.eof()) {
            std::getline(ss, line);

            auto nums = splitString(line, " ");

            Range r = {
                .start_output = std::stol(nums[0]),
                .start_input = std::stol(nums[1]),
                .length = std::stol(nums[2]),
            };

            m.ranges.push_back(r);
        }

        std::ranges::sort(m.ranges, {}, [](auto&& r) { return r.start_input; });
        maps.push_back(m);
    }

    return std::make_pair(seed_nums, maps);
}

void part1() {
    std::stringstream file = FileUtils::loadFile("day05/maps.txt");

    auto [seed_nums, maps] = parse(file);

    std::vector<long> results = {};
    for (auto seed : seed_nums) {
        auto curr_seed = seed;
        for (auto& map : maps) {
            curr_seed = map.map(curr_seed);
        }
        results.push_back(curr_seed);
    }

    for (int i = 0; i < results.size(); i++) {
        std::cout << "Seed " << seed_nums[i] << " -> " << results[i] << std::endl;
    }

    auto min = std::min_element(results.begin(), results.end());

    std::cout << "Min seed: " << seed_nums[std::distance(results.begin(), min)] << " -> " << *min << std::endl;
}

void part2() {
    std::stringstream file = FileUtils::loadFile("day05/maps.txt");

    auto [seed_nums, maps] = parse(file);

    std::vector<std::pair<long, long>> ranges = {};

    long min = (1L << 32);
    for (int i = 0; i < seed_nums.size(); i += 2) {
        auto start = seed_nums[i];
        auto end = start + seed_nums[i + 1];

        std::cout << "Range " << start << " -> " << end << std::endl;

        for (long j = start; j < end; j++) {
            auto curr_seed = j;
            for (auto& map : maps) {
                curr_seed = map.map(curr_seed);
            }

            if (curr_seed < min) {
                min = curr_seed;
                std::cout << "Found new min " << min << std::endl;
            }

            if (min <= 0) {
                std::cout << "Found seed " << j << std::endl;
                return;
            }
        }
    }

    std::cout << std::endl;

    std::cout << "Min " << min << std::endl;
}

int main() {
    std ::cout << "P1: " << std::endl;
    part1();

    std::cout << "P2:" << std::endl;
    part2();
}