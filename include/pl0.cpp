
#include "pl0.h"
#include "utils.h"
#include <cctype>
#include <cstdio>

int  sym = 0;         //
char ch = 0;          // 最近一次从文件中读出的字符
int  cc = 0;          // 在这一行的位置
int  line_length = 0; // 行长度
int  error_cnt = 0;
int cx; // 代码分配指针，代码生成模块总在cx所指的位置生成新代码
char line[100]; // 行缓冲区

int line_num; // 行号

std::vector<std::string> texts;

void Error(int n) {

    std::printf("        ");
    for (int i = 1; i <= cc - 1; i++) {
        std::printf(" ");
    }
    std::printf("\n");
    std::printf("Error %3d: %s\n", n, error_msg[n].c_str());
    error_cnt++;
}

void Getch() {

    while (cc + 1 >= texts[line_num].length()) { // 是否读完一行
        if (line_num + 1 >= texts.size()) {      // 读取完整个文件
            Message("================");
            Message("PROGRAM INCOMPLETE");
            Message("================");
            exit(0); // 退出程序
        }

        // 下一行

        std::printf("%5d ", cx);

        std::cout << texts[line_num] << "\n";
        line_num++;
        cc = 0;
    }

    ch = texts[line_num][cc];

    cc++;
}

void GetSym() {
    while (std::isspace(ch)) {
        Getch();
    }

    if (std::isalpha(ch)) { // 字母

    } else if (std::isdigit(ch)) { // 数字

    } else {
    }
}
