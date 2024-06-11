#include "utils.h"
#include <iostream>
#include <pl0.h>


int main() {
   auto s = ReadFile("样例程序.pl");

   auto vec = SplitString(s, "\n");

   for (const auto &value : vec) {
       std::cout << value << "\n";
   }
    Getch();
    std::cout <<ch<<"\n";
    return 0;
}