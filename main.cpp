#include <cstddef>
#include <functional>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <array>
#include <queue>
#include <string>
#include <string_view>
#include <fstream>
#include <iostream>
#include <argp.h>
#include "utils.h"

namespace ra = std::ranges;
namespace rv = ra::views;

auto find_path(std::vector<std::vector<int>>& matrix, int target, std::size_t row, std::size_t col) -> std::vector<std::pair<int, int>> {
    if (row >= matrix.size() || col >= matrix[0].size() || matrix[row][col] == -1) return {}; // Wall or out of bounds

    if (matrix[row][col] == target) return {{row, col}}; // Path found!

    const auto cur_elem = matrix[row][col];
    matrix[row][col] = -1; // Mark as visited

    const std::vector dr = {1, -1, 0, 0};
    const std::vector dc = {0, 0, 1, -1};
    for (const auto& i : rv::iota(0, 4)) { // C++20 std::ranges::views::iota
        auto path = find_path(matrix, target, row + dr[i], col + dc[i]);
        if (!path.empty()) {
            path.push_back({row, col});
            return path;
        }
    }

    matrix[row][col] = cur_elem; // Unmark visited if no valid path found
    return {};
}

auto flood_fill(std::vector<std::vector<int>> maze, std::size_t start_row, std::size_t start_col) -> std::vector<std::pair<int, int>> {
    const auto rows = maze.size();
    const auto cols = maze[0].size();

    std::vector<std::vector<int>> solved_maze(rows, std::vector<int>(cols, -1));  // Initialize matrix with -1
    solved_maze[start_row][start_col] = 0;  // Starting point

    std::queue<std::pair<int, int>> q;
    q.push({start_row, start_col});

    while (!q.empty()) {
        auto [cur_row, cur_col] = q.front();  // C++17 Structured bindings
        q.pop();

        const std::vector dr = {-1, 1, 0, 0};
        const std::vector dc = {0, 0, -1, 1};

        for (const auto& i : rv::iota(0, 4)) { // C++20 std::ranges::views::iota
            std::size_t new_row = cur_row + dr[i];
            std::size_t new_col = cur_col + dc[i];

            if (new_row < rows && new_col < cols && maze[new_row][new_col] == 0 && solved_maze[new_row][new_col] == -1) {
                solved_maze[new_row][new_col] = solved_maze[cur_row][cur_col] + 1;
                q.push({new_row, new_col});
            }
        }
    }
    const auto end = ra::min_element(solved_maze.back(), std::less(), [](auto e) { // C++20 std::ranges::min_element
        return (e != -1) ? false : true;
    });

    const auto path = find_path(solved_maze, *end, start_row, start_col);

    return path;
}

void encode(std::vector<std::vector<int>> matrix, std::string_view wall, std::string_view space, std::string_view route, std::string ouput_file) {
        std::ofstream file;
        file.open(ouput_file);
        for (const auto& row : matrix) {
            for (const auto& elem : row)
                file << (elem == 0 ? space : (elem == 1 ? wall : route)); // Using a switch statement is 10 lines longer
            file << '\n';
        }
        file.close();
}

auto main(int argc, char** argv) -> int {
    utils::Arguments args;
    argp_parse(&utils::argp_parser, argc, argv, 0, nullptr, &args);

    std::ifstream file(args.input_filename);
    if (!file.is_open()) {
        std::cerr << args.output_filename << ": No such file or directory." << '\n';
        exit(EXIT_FAILURE);
    }

    const auto maze = [&]() {               // Immediately invoked lambda (IIFE) better for value semantics and immutability
        std::vector<std::string> result;    // ITM anti-pattern (Initialize Then Modify) unsafe but gets inmediately destroyed by the garbage collector so is fine
        for (std::string line; std::getline(file, line);)
            result.push_back(line);
        result.pop_back();                  // Removes null character, avoiding segfault when converting to integer
        return result;
    }();
    file.close();

    const auto matrix = [=]() {                 // Parse maze from "█" and ' ' to 1s and 0s
        std::vector<std::vector<int>> result;   // ITM anti-pattern
        for (const auto& row : maze) {
            std::vector<int> values;            // ITM anti-pattern
            for (std::size_t i = 0; i < row.size(); i += (row[i] == ' ' ? 1 : 3)) // Skip the next two characters
                values.push_back(row[i] == ' ' ? 0 : 1);                          // (weird bug where █ counts as 3 characters)
            result.push_back(values);
        }
        return result;
    }();

    const auto start = ra::find_if(matrix, [](const auto& e) { // Pointer to the first 0 value element using C++20 std::ranges::find, std::ranges::find_if
        return ra::find(e, 0) != e.end();
    });

    const auto start_row = std::distance(matrix.begin(), start);
    const auto start_column = std::distance(start->begin(), ra::find(*start, 0));

    const auto solved_maze = flood_fill(matrix, start_row, start_column);

    const auto solution = [=]() {
        std::vector<std::vector<int>> result = matrix;
        for (const auto& coord : solved_maze) {
            std::size_t row = coord.first;
            std::size_t col = coord.second;
            result[row][col] = 2;
        }
        return result;
    }();

    encode(solution, "█", " ", "*", args.output_filename);
}
