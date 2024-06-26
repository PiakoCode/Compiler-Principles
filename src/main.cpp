#include "pl0.h"
#include "utils.h"
#include <algorithm>
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
    texts = SplitString(ReadFile(path), "\n");

    // // 输出程序文本内容
    // for (auto const &value : texts) {
    //     std::cout << value << "\\\n";
    // }
    // std::cout << "\n";

    // std::cout << "===========\n";

    // while (true) {
    //     Getch();
    // }

    ch = ' ';
    GetSym();

    Block(MergeSet(CreateSet(SYM_PERIOD), MergeSet(declare_sym, start_sym)));

    if (sym != SYM_PERIOD) // 点
    {
        Error(9);
    }

    //语法分析完成


    if (error_cnt == 0) {
        std::cout << "INTERPRETER START"
                  << "\n";
        Interpret();
    }

    return 0;
}