//
// Created by trent on 12/3/2023.
//


#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace {
}

int main() {
    auto fs = std::fstream("../inputs/03_parts.txt", std::ios::in);
    if (!fs.is_open()) {
        std::cerr << "Failed to open input." << std::endl;
        return -1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fs, line)) {
        lines.push_back(line);
    }

    int n_rows = lines.size();
    int n_cols = line.length();
    // we allocate a matrix with '.' padding to make bounds checking simple
    std::vector schematic(n_rows + 2, std::vector<char>(n_cols + 2, '.'));
    for (int i = 1; i < n_rows + 1; i++) {
        std::string current_line = lines[i - 1];
        for (int j = 1; j < n_cols + 1; j++) {
            schematic[i][j] = current_line[j - 1];
        }
    }

    std::vector<std::pair<int, int>> gear_coords;
    std::vector<std::tuple<int, int, int>> part_bounds;
    for (int i = 1; i < n_rows + 1; i++) {
        for (int j = 1; j < n_cols + 1; j++) {
            char current_char = schematic[i][j];

            if (!isdigit(current_char)) {
                if (current_char == '*') {
                    gear_coords.emplace_back(i, j);
                }
                continue;
            }

            // if we find a digit we need to find its bounds [j, k)
            // start seeking right boundary k from j
            int k = j;
            while (true) {
                k++;
                if (!isdigit(schematic[i][k])) {
                    // found boundary k
                    break;
                }
            }

            part_bounds.emplace_back(i, j, k);
            j = k - 1; // back up to k - 1 to ensure we process the entry (it could be a gear!);
        }
    }

    std::vector<int> part_nums;
    std::vector<std::pair<int, std::tuple<int, int, int>>> parts;
    std::vector schematic_part_num_map(n_rows + 2, std::vector(n_cols + 2, -1));
    for (auto part_bound : part_bounds) {
        int m = std::get<0>(part_bound);
        int n = std::get<1>(part_bound);
        int k = std::get<2>(part_bound);

        // search the area for symbols (who gaf about skipping the digits)
        bool is_part = false;
        for (int i = m - 1; i <= m + 1; i++) {
            for (int j = n - 1; j <= k; j++) {
                if (char c = schematic[i][j]; !isdigit(c) && c != '.') {
                    is_part = true;
                }
            }
        }

        // if we found a part, parse the number , sum it and map it
        if (is_part) {
            std::string part_str;
            for (int j = n; j < k; j++) {
                char c = schematic[m][j];
                part_str += c;
            }

            int part_num = std::stoi(part_str);
            part_nums.push_back(part_num);

            for (int j = n; j < k; j++) {
                schematic_part_num_map[m][j] = part_num;
            }
        }
    }

    int sum = 0;
    for (int part_num : part_nums) {
        sum += part_num;
    }

    std::cout << "Sum of part numbers: " << sum << std::endl;

    int gear_sum = 0;
    for (auto gear_coord : gear_coords) {
        int i = gear_coord.first;
        int j = gear_coord.second;
        std::set<int> gear_parts{
            schematic_part_num_map[i - 1][j - 1],
            schematic_part_num_map[i - 1][j],
            schematic_part_num_map[i - 1][j + 1],
            schematic_part_num_map[i][j - 1],
            // skip da gear
            schematic_part_num_map[i][j + 1],
            schematic_part_num_map[i + 1][j - 1],
            schematic_part_num_map[i + 1][j],
            schematic_part_num_map[i + 1][j + 1]
        };

        // remove "null" part_num
        gear_parts.erase(-1);

        // if we have found a gear, compute the ratio and sum
        if (gear_parts.size() == 2) {
            int gear_ratio = 1;
            for (int part : gear_parts) {
                gear_ratio *= part;
            }

            gear_sum += gear_ratio;
        }
    }

    std::cout << "Sum of gear ratios: " << gear_sum << std::endl;

    return 0;
}