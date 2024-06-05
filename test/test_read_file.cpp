#include "utils.h"
#include <iostream>

int main() {
    auto s =  read_file("./readme.md");

    std::cout<<s<<"\n";
    return 0;
}