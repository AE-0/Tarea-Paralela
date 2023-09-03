#include "utils.h"

namespace utils {
    error_t parse_opt(int key, char* arg, argp_state* state) {
        auto* arguments = static_cast<Arguments*>(state->input);
        switch (key) {
            case 'i':
                arguments->input_filename = arg;
                break;
            case 'o':
                arguments->output_filename = arg;
                break;
            case 'h':
                argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
                exit(EXIT_SUCCESS);
            case ARGP_KEY_END:
                if (arguments->input_filename.empty() || arguments->output_filename.empty()) {
                    fprintf(stderr, "Error: Both -i and -o parameters are required.\n");
                    argp_state_help(state, stderr, ARGP_HELP_USAGE);
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

    const argp_option options[] = {
        {"input", 'i', "FILE", 0, "Input file", 0},
        {"output", 'o', "FILE", 0, "Output file", 0},
        {"help", 'h', nullptr, 0, "Display this message", 0},
        {nullptr, 0, nullptr, 0, nullptr, 0}  // supresses -Werror=missing-field-initializers
    };

    const argp argp_parser = {
        options, parse_opt, "FILE", "Pass a file as an argument using -i and -o flag",
        nullptr, nullptr, nullptr}; // -Werror=missing-field-initializers
}
