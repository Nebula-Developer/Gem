//
// Created by NebulaDev on 12/11/22.
//

#ifndef GEM_COMPILE_H
#define GEM_COMPILE_H

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

std::string compile_from_data(std::string data);

class compile_data {
public:
    std::vector<std::string> data;
};

// c++ dynamic string array:
// std::vector<std::string> args;

class compile_config {
public:
    std::vector<std::string> input_files;
    std::string output = "gem.out";
    std::string compiler = "g++";
    std::string compiler_args;
    std::string linker;
    std::string linker_args;

    void exec(compile_data data) {
        // to pipe data to gcc, use:
        // g++ -x c -o output - <<< "data"
        // To do something like this with the same method:
        // g++ -o test a.cpp b.cpp c.cpp
        // When we have the data for each of those files,
        // You would use:
        // g++ -x c -o test - <<< "a.cpp data" <<< "b.cpp data" <<< "c.cpp data"

        std::string command = compiler + " -x c++ -o " + output + " " + compiler_args + " - ";

        // Now we pipe all of the data in separate pipes
        for (int i = 0; i < data.data.size(); i++) {
            std::string file_data = data.data[i];

            // replace single \ with double, but not \\ with \\\\

            std::regex regex("\\\\(?!\\\\)");

            // replace " with \"
            std::regex r("\"");

            file_data = std::regex_replace(file_data, r, "\\\"");

            // Remove comments
            file_data = std::regex_replace(file_data, std::regex("//.*"), "");
            file_data = std::regex_replace(file_data, std::regex("/\\*.*\\*/"), "");

            command += " <<< \"" + file_data + "\"";
        }

        // create prev.sh
        std::ofstream prev("prev.sh");
        prev << command;
        prev.close();

        FILE *fp = popen(command.c_str(), "r");
        if (fp == NULL) {
            std::cout << "Error: Failed to exec command: " << command << std::endl;
            return;
        }
        pclose(fp);
    }
};

compile_data compile(const compile_config& config);

#endif //GEM_COMPILE_H
