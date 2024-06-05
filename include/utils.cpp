#include "utils.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>


std::string read_file(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if(!fileStream) {
        std::cerr << "无法打开文件: " << filePath << '\n';
        exit(1);
    }
   
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    return stringStream.str();
}