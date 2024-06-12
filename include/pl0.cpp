
#include "pl0.h"
#include "utils.h"
#include <cctype>
#include <cstdio>
#include <vector>

SymType sym = SYM_NULL;  // 最近一次的token类型
char    ch = 0;          // 最近一次从文件中读出的字符
int     cc = 0;          // 在这一行的位置
int     line_length = 0; // 行长度
int     error_cnt = 0;
int cx = 0; // 代码分配指针，代码生成模块总在cx所指的位置生成新代码

int line_num; // 行号

std::vector<std::string> texts{};
std::vector<Table>       tables{};

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
            std::printf("%5d ", cx);
            std::cout << texts[line_num] << "\n";

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
    while (std::isspace(ch)) { // 跳过空白字符
        Getch();
    }
    int         token_length = 0;
    std::string token{};

    if (std::isalpha(ch)) { // 字母

        token.push_back(ch);
        Getch();
        while (std::isalpha(ch) || std::isdigit(ch)) {
            token.push_back(ch);
            Getch();
        }

        if (words.count(token) != 0) { // 是关键字
            sym = words[token];
        } else { // 是变量
            sym = SYM_IDENTIFIER;
        }

    } else if (std::isdigit(ch)) { // 数字

        token.push_back(ch);
        Getch();

        while (std::isdigit(ch)) {
            token.push_back(ch);
            Getch();
        }

        sym = SYM_NUMBER;

    } else if (ch == ':') {
        Getch();

        if (ch == '=') { // 是赋值
            sym = SYM_BECOMES;
            Getch();

        } else {            // 不是赋值
            sym = SYM_NULL; // unknown
        }
    } else if (ch == '<') {
        Getch();

        if (ch == '=') { // 是小于等于
            sym = SYM_LES;
            Getch();

        } else { // 不是小于等于
            sym = SYM_LES;
        }
    } else if (ch == '>') {
        Getch();

        if (ch == '=') { // 是大于等于
            sym = SYM_GEQ;
            Getch();
        } else { // 不是大于等于
            sym = SYM_GTR;
        }
    } else if (ch == '=') { // 是等于
        Getch();

        if (ch == '=') { // 是等于
            sym = SYM_EQ;
            Getch();

        } else {            // 不是等于
            sym = SYM_NULL; // unknown
        }
    }
}

// 测试当前单词是否合法
void Test(unsigned long s1, unsigned long s2, int n) {
    if (!(sym & s1)) { // sym不在s1中
        Error(n);
        s1 = s1 | s2;         // 将s2补充到s1中
        while (!(sym & s1)) { // 找到下一个合法字符
            GetSym();
        }
    }
}


/**
 * 常量声明
 */
void ConstDeclaration()
{
    if( sym == SYM_IDENTIFIER) { // 如果是标识符
       GetSym();
       if(sym == SYM_EQ || sym == SYM_BECOMES) // sym为等号或赋值号
       {
           if(sym === SYM_BECOMES) { // 赋值号报错
               Error(1)
           }

           GetSym();

           if(sym == SYM_NUMBER) { // sym为数字记录到符号表
              // TODO 将数字记录到符号表

              GetSym();
           } else {
               Error(2);
           }
       } else {
           Error(3);
       }
    } else {
        Error(4);
    }
}



