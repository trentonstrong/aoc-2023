//
// Created by trent on 12/2/2023.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


namespace {
    struct Result {
        int n_red;
        int n_blue;
        int n_green;
    };

    struct Game {
        int game_id;
        std::vector<Result> results;
    };

    std::vector<std::string> split_string(const std::string& s, const char delimiter) {
        auto result = std::vector<std::string>();

        std::string current;
        for (const char c: s) {
            if (c == delimiter) {
                if (!current.empty()) {
                    if (current[0] == ' ') {
                        current = current.substr(1);
                    }
                    result.push_back(current);
                    current = "";
                }
                continue;
            }
            current += c;
        }

        if (!current.empty()) {
            if (current[0] == ' ') {
                current = current.substr(1);
            }
            result.push_back(current);
        }

        return result;
    }

    Game parse_game(const std::string& line) {
        auto ss = std::stringstream(line);
        std::string dummy, rest;
        int game_id;
        ss >> dummy >> game_id >> dummy;
        std::getline(ss, rest);

        std::vector<Result> all_results;
        const auto rounds = split_string(rest, ';');
        for (const std::string& round: rounds) {
            auto results = split_string(round, ',');
            Result current_result{};
            for (const std::string& result: results) {
                int n_cubes;
                std::string cube_type;
                std::stringstream(result) >> n_cubes >> cube_type;
                if (cube_type == "red") { current_result.n_red = n_cubes; } else if (cube_type == "blue") {
                    current_result.n_blue = n_cubes;
                } else if (cube_type == "green") { current_result.n_green = n_cubes; }
            }

            all_results.push_back(current_result);
        }

        return Game{game_id, all_results};
    }
}

int main() {
    std::fstream fs = std::fstream("../inputs/02_games.txt", std::ios::in);
    if (!fs.is_open()) {
        std::cerr << "Unable to open input file." << std::endl;
        exit(-1);
    }

    constexpr int max_red = 12;
    constexpr int max_blue = 14;
    constexpr int max_green = 13;

    std::vector<Game> games;
    std::string line;
    while (std::getline(fs, line)) {
        Game game = parse_game(line);
        std::cout << "Parsed Game: " << game.game_id << std::endl;
        games.push_back(game);
    }

    std::vector<Game> possible_games;
    std::copy_if(games.begin(), games.end(), std::back_inserter(possible_games), [](const Game& g) {
        return std::all_of(g.results.begin(), g.results.end(), [](Result r) {
            return r.n_red <= max_red && r.n_blue <= max_blue && r.n_green <= max_green;
        });
    });

    std::cout << "n_games: " << games.size() << " n_possible: " << possible_games.size() << std::endl;

    int sum = 0;
    for (const Game &g : possible_games) {
        sum += g.game_id;
        std::cout << "Possible Game ID: " << g.game_id << std::endl;
    }
    std::cout << "Sum of Game IDs: " << sum << std::endl;

    int cube_sum = 0;
    for (const Game &g : games) {
        int min_red_needed = 0;
        int min_blue_needed = 0;
        int min_green_needed = 0;
        for (auto result : g.results) {
            min_red_needed = result.n_red > min_red_needed ? result.n_red : min_red_needed;
            min_blue_needed = result.n_blue > min_blue_needed ? result.n_blue : min_blue_needed;
            min_green_needed = result.n_green > min_green_needed ? result.n_green : min_green_needed;
        }

        cube_sum += min_red_needed * min_blue_needed * min_green_needed;
    }

    std::cout << "Cube sum: " << cube_sum << std::endl;
}
