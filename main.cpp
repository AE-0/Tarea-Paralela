#include <functional>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <argp.h>

namespace ra = std::ranges;
namespace rv = ra::views;

void print(ra::viewable_range auto &&v, std::string_view separator = ", ") {
    std::cout << '[';
    for (const auto &x : v) {
        for (const auto &e: x)
            std::cout << e << separator;
    }
    std::cout << ']' << '\n';
}

auto main(int argc, char **argv) -> int {
    argp_parse(0, argc, argv, 0, 0, 0);

    std::ifstream file("example.txt"); // Open your file here

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::vector labyrinth = [&]() {
        std::vector<std::string> result;
        for (std::string line; std::getline(file, line); )
            result.push_back(line);
        return result;
    }();

    std::vector<std::vector<int>> matrix;

    for (const std::string& row : labyrinth) {
        std::vector<int> values;
        for (std::size_t i = 0; i < row.size(); ++i) {
            if (row[i] == ' ') values.push_back(0);
            else {
                values.push_back(1);
                i += 2; // Skip the next two characters (weird bug where █ counts as 3 characters)
            }
        }
        matrix.push_back(values);
    }

    for (const auto &s : labyrinth)
        std::cout << s << '\n';

    // print(matrix);
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }

    file.close();

    exit(EXIT_SUCCESS);
}
