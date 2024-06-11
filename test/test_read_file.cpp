#include "utils.h"
#include <iostream>
#include <pl0.h>


int main() {
   auto s = ReadFile("/home/Piako/code/编译原理课程设计/Compiler_Principles/readme.md");

   auto vec = SplitString(s, "\n");

   for (const auto &value : vec) {
       std::cout << value << "\n";
   }

    return 0;
}