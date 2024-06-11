
#include "pl0.h"
#include <cstdio>

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

    if (cc == line_length) {
        if (feof(infile)) { // 如果是文件末尾
            std::printf("\nPROGRAM INCOMPLETE\n");
        }
        cc = line_length = 0;
        std::printf("%5d ", current_instruction_index);
        while (!feof(infile) && (ch = getc(infile)) != '\n') { // 读取一行
            std::printf("%c", ch);
            line[++line_length] = ch;
        }
        std::printf("\n");
        line[++line_length] = ' ';
    }
    ch = line[++cc];
}