#include <functional>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <array>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <argp.h>

namespace ra = std::ranges;
namespace rv = ra::views;

struct Arguments {
    std::string input_filename;
    std::string output_filename;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    Arguments *arguments = static_cast<Arguments *>(state->input);
    switch (key) {
        case 'i':
            arguments->input_filename = arg;
            break;
        case 'o':
            arguments->output_filename = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

template <typename T>
void print(const std::vector<T> &v, std::string_view separator = " ") {
    for (const auto& e : v)
        std::cout << e << separator << '\n';
}

template <typename T>
void print(const std::vector<std::vector<T>> &m, std::string_view separator = " ") {
    for (const auto& v : m) {
        for (const auto& e : v)
            std::cout << e << separator;
        std::cout << '\n';
    }
}

auto flood_fill(std::vector<std::vector<int>> maze, std::size_t start_row, std::size_t start_col) -> std::vector<std::vector<int>> {
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
    return solved_maze;
}

void encode(std::vector<std::vector<int>> matrix, std::string wall, std::string space, std::string ouput_file) {
        std::ofstream file;
        file.open(ouput_file);
        for (const auto& row : matrix) {
            for (const auto& elem : row)
                file << (elem ? wall : space);
             file << '\n';
        }
        file.close();
}

auto main(int argc, char **argv) -> int {
    Arguments args;
    std::string input_filename, output_filename;
    static const argp_option options[] = {
        {"input", 'i', "FILE", 0, "Input file", 0},
        {"output", 'o', "FILE", 0, "Output file", 0},
        {nullptr, 0, nullptr, 0, nullptr, 0}};  // supresses -Werror=missing-field-initializers
    static const argp argp_parser = {
        options, parse_opt, "FILE", "Pass a file as an argument using -i and -o flag",
        nullptr, nullptr, nullptr};             // -Werror=missing-field-initializers
    argp_parse(&argp_parser, argc, argv, 0, 0, &args);

    std::ifstream file(args.input_filename);
    if (!file.is_open()) {
        std::cerr << args.output_filename << ": No such file or directory." << '\n';
        exit(EXIT_FAILURE);
    }

    const auto maze = [&]() {          // Immediately invoked lambda (IIFE) improves performance and inmutability
        std::vector<std::string> result;    // ITM anti-pattern
        for (std::string line; std::getline(file, line);)
            result.push_back(line);
        result.pop_back();                  // Removes null character, avoiding segfault when converting to integer
        return result;
    }();

    const auto matrix = [=]() {
        std::vector<std::vector<int>> result;   // ITM anti-pattern
        for (const auto& row : maze) {
            std::vector<int> values;            // ITM anti-pattern
            for (std::size_t i = 0; i < row.size(); i += (row[i] == ' ' ? 1 : 3)) // Skip the next two characters
                values.push_back(row[i] == ' ' ? 0 : 1);                          // (weird bug where █ counts as 3 characters)
            result.push_back(values);
        }
        return result;
    }();

    const auto start = ra::find_if(matrix, [](const auto& e) { // pointer to the first 0 value element
        return ra::find(e, 0) != e.end();
    });
    const auto start_row = std::distance(matrix.begin(), start);
    const auto start_column = std::distance(start->begin(), ra::find(*start, 0));

    const auto solved_maze = flood_fill(matrix, start_row, start_column);

    print(maze, "");
    print(solved_maze);

    encode(matrix, "█", " ", args.output_filename);

    file.close();
    exit(EXIT_SUCCESS);
}
