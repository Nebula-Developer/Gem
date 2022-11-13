//
// Created by NebulaDev on 12/11/22.
//

#include "compile.h"
#include <regex>
#include <utility>

std::string curInput;

void regreplace(std::string regex, const std::string& replace, bool replaceSpaceWhitespace = true) {
    if (replaceSpaceWhitespace) {
        // optional space
        regex = std::regex_replace(regex, std::regex(" "), "\\s*");
    }

    // required space
    regex = std::regex_replace(regex, std::regex("__"), "\\s+");

    std::regex reg(regex);
    curInput = std::regex_replace(curInput, reg, replace);
}

std::string compile_string(std::string input) {
    curInput = std::move(input);

    // Replace:
    // func [name]([args]) -> [return] { }
    // with:
    // [return] [name]([args]) { }
    regreplace(R"(func__(\w+) \(([^)]*)\) -> (\w+) \{)", "$3 $1($2) {");

    // Replace:
    // func [name]([args]) { }
    // with:
    // void [name]([args]) { }
    regreplace(R"(func__(\w+) \(([^)]*)\) \{)", "void $1($2) {");
    // Or var [name](args) -> [return] { } to auto
    regreplace(R"(var__(\w+) \(([^)]*)\) \{)", "auto $1($2) {");

    // Replace argument:
    // [name]: [type]
    // with:
    // [type] [name]
    // Include * and &
    // TODO: Make this more efficient
    regreplace(R"((\w+) : (\w+)\*\*)", "$2** $1");
    regreplace(R"((\w+) : (\w+)\&\&)", "$2&& $1");
    regreplace(R"((\w+) : (\w+)\*)", "$2* $1");
    regreplace(R"((\w+) : (\w+)\&)", "$2& $1");
    regreplace(R"((\w+) : (\w+))", "$2 $1");

    // Find every:
    // [[ [modifiers] ]][type] [name]
    // and do stuff
    std::regex reg(R"(\[\[([^\]]*)\]\]\s*(\w+)\s*(\w+))");
    std::smatch match;
    while (std::regex_search(curInput, match, reg)) {
        std::string varName = match[3].str();
        std::string varType = match[2].str();
        // replace with just the type and name
        curInput = match.prefix().str() + varType + " " + varName + match.suffix().str(); // NOLINT(performance-inefficient-string-concatenation)

        std::string modifiers = match[1].str();
        std::cout << "Modifiers: " << modifiers << std::endl;

        // split by ,
        std::regex reg2(R"(\s*,\s*)");
    }


    return curInput;
}

std::string compile_from_file(const std::string& file) {
    // read file
    FILE *fp = fopen(file.c_str(), "r");
    if (fp == nullptr) {
        return "";
    }
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *string = (char *) malloc(fsize + 1);
    fread(string, fsize, 1, fp);
    fclose(fp);
    string[fsize] = 0;

    // return
    return compile_string(string);
}

compile_data compile(const compile_config& config) {
    compile_data data;
    for (auto& i : config.input_files) {
        std::string compiled = compile_from_file(i);
        data.data.emplace_back(compiled);
    }
    return data;
}