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

void encode(std::vector<std::vector<int>> matrix, std::string wall, std::string space, std::string ouput_file) {
        std::ofstream file;
        file.open(ouput_file);
        for (const auto& row : matrix) {
            for (std::size_t i = 0; i < row.size(); i ++)
                file << (row[i]? wall : space );
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
        {nullptr, 0, nullptr, 0, nullptr, 0}};  // -Werror=missing-field-initializers
    static const argp argp_parser = {
        options, parse_opt, "FILE", "Pass a file as an argument using -i and -o flag",
        nullptr, nullptr, nullptr};             // -Werror=missing-field-initializers
    argp_parse(&argp_parser, argc, argv, 0, 0, &args);

    std::ifstream file(args.input_filename);

    std::cout << args.output_filename;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(EXIT_FAILURE);
    }

    const auto labyrinth = [&]() {
        std::vector<std::string> result;    // ITM anti-pattern
        for (std::string line; std::getline(file, line);)
            result.push_back(line);
        return result;
    }();

    const auto matrix = [&]() {
        std::vector<std::vector<int>> result;   // ITM anti-pattern
        for (const auto& row : labyrinth) {
            std::vector<int> values;            // ITM anti-pattern
            for (std::size_t i = 0; i < row.size(); i += (row[i] == ' ' ? 1 : 3)) // Skip the next two characters
                values.push_back(row[i] == ' ' ? 0 : 1);                          // (weird bug where █ counts as 3 characters)
            result.push_back(values);
        }
        return result;
    }();


    for (const auto &s : labyrinth)
        std::cout << s << '\n';

    for (const auto& row : matrix) {
        for (int val : row)
            std::cout << val << " ";
        std::cout << "\n";
    }

    encode(matrix, "█", " ", args.output_filename);

    file.close();
    exit(EXIT_SUCCESS);
}
