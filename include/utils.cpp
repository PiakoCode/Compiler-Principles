#include "utils.h"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string ReadFile(const std::string &filePath) {
    std::ifstream file_stream(filePath);
    if (!file_stream) {
        std::cerr << "无法打开文件: " << filePath << '\n';
        exit(1);
    }

    std::stringstream string_stream;
    string_stream << file_stream.rdbuf();
    return string_stream.str();
}

std::vector<std::string> SplitString(const std::string &str,
                                     const std::string &delimiter) {
    std::vector<std::string> tokens;

    std::size_t start = 0;
    std::size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.emplace_back(str.substr(start, end - start)+"\n");
        start = end + delimiter.length() ;
        end = str.find(delimiter, start);
    }

    tokens.emplace_back(str.substr(start, end - start)+"\n");

    return tokens;
}
