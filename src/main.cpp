#include "pl0.h"
#include "utils.h"
#include <iostream>

int main(int argc, char *argv[]) {

    std::cout << "========== Compiler_Principles =========="
              << "\n";
    std::string path;

    // 获取程序文件路径
    if (argc <= 1) {
        Message("Usage: pl0 sourcefile\n");
        Message("You can input the source file name at here:\n");
        std::cin >> path;
    } else {
        path = std::string(argv[1]);
    }

    // 读取程序文本内容
    texts = SplitString(ReadFile(path), "\n"); // 不会保留\n

    // 输出程序文本内容
    for (auto const &value : texts) {
        std::cout << value << "\\\n";
    }
    std::cout << "\n";

    std::cout << "===========\n";

    while (true) {
        Getch();
    }

    return 0;
}