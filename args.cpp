//
// Created by NebulaDev on 12/11/22.
//

#include "args.h"
#include "compile/compile.h"

bool is_arg(std::string input, std::string arg, int shorten_len) {
    return input == arg || input == shorten_arg(arg, shorten_len);
}

std::string shorten_arg(std::string argc, int sublen) {
    // sublen 1:
    // --help -> -h
    // sublen 2:
    // --help -> -he

    if (argc.length() > 2 && argc.substr(0, 2) == "--")
        return "-" + argc.substr(2, sublen);
    else return argc;
}


compile_config parse_args(int argc, char **argv) {
    compile_config config;

    for (int i = 1; i < argc; i++) {
        if (is_arg(argv[i], "--help")) {
            help();
        } else {
            FILE *file = fopen(argv[i], "r");

            if (file == nullptr) {
                std::cout << "Error: File " << argv[i] << " does not exist." << std::endl;
                exit(1);
            }

            config.input_files.emplace_back(argv[i]);
        }
    }

    if (config.input_files.empty()) {
        std::cout << "Error: No input file specified." << std::endl;
        exit(1);
    }

    return config;
}

void help() {
    std::cout << "Usage: gem [options] [file]" << std::endl;
}