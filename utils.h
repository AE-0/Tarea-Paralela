#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <argp.h>

namespace utils {
    struct Arguments {
        std::string input_filename;
        std::string output_filename;
    };

    error_t parse_opt(int key, char* arg, argp_state* state);

    extern const argp_option options[];
    extern const argp argp_parser;
}

#endif
