#include "utils.h"
#include <iostream>

int main() {
    auto s = ReadFile("./readme.md");

    auto vec = SplitString(s, "\n");

    for (const auto &value : vec) {
        std::cout << value << "\n";
    }
    return 0;
}