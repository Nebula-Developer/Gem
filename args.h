//
// Created by NebulaDev on 12/11/22.
//

#ifndef GEM_ARGS_H
#define GEM_ARGS_H

#include <iostream>
#include "compile/compile.h"

bool is_arg(std::string input, std::string arg, int shorten_len = 1);

std::string shorten_arg(std::string argc, int sublen);

compile_config parse_args(int argc, char **argv);

void help();

#endif //GEM_ARGS_H
