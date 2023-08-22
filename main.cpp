//#include <fmt/core.h>
//#include <fmt/ranges.h>
#include <cstdlib>
#include <ranges>
#include <array>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
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
    constexpr int xs[2][3] = {{1, 2, 3}, {4, 5, 6}};

    auto sum = xs | rv::transform([](auto& e) {
        return e | rv::transform([](auto& d) { return d + 10; });
    });

    //fmt::print("{}\n", sum);
    print(sum);

    std::ifstream file("example.txt"); // Open your file here

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (std::string line; std::getline(file, line); ) {
        std::cout << line << '\n';
    }

    file.close();
    exit(EXIT_SUCCESS);
}
