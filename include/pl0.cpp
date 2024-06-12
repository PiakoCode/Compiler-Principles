
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

int                      line_num; // 行号
std::vector<Instruction> codes{};
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
 * @brief
 * 中间代码生成
 */
void Gen(OpCode x, int a, int b) {
    codes.push_back({x, a, b});
    cx++;
}

/**
 * @brief 
 * 符号表
 */





/**
 * 常量声明
 */
void ConstDeclaration() {
    if (sym == SYM_IDENTIFIER) { // 如果是标识符
        GetSym();
        if (sym == SYM_EQ || sym == SYM_BECOMES) // sym为等号或赋值号
        {
            if (sym == = SYM_BECOMES) { // 赋值号报错
                Error(1)
            }

            GetSym();

            if (sym == SYM_NUMBER) { // sym为数字记录到符号表
                // TODO 将数字记录到符号表     enter(constant);

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


/**
 * 变量声明
 */
void VarDeclaration() {
    if(sym == SYM_IDENTIFIER) {
        //TODO 将标识符记录到符号表中   enter(variable);
        GetSym();
    } else {
        Error(4);
    }
}

/**
 * 输出当前代码块的中间代码
 */




/**
 * factor处理 生成中间代码
 */
void Factor(unsigned long fsys) {
    long i;
    test(face_begsys,fsys,24); // 检查当前token是否在face_begsys集合中
                              // 若不合法，抛错误，并通过fsys集合恢复护法处理

    while (sym & face_begsys) { // 当sym在face_bgsys中
       if(sym == SYM_IDENTIFIER) // 为标识符
        {
            // TODO 查找符号表中对应的索引值    i = position(id);

            if(i == 0) { // 为找到标识符
                Error(11);
            } else {
                // TODO switch语句判断是否符号类型
//                switch(table[i].kind)
//                {
//                case constant: //常量
//                    gen(lit, 0, table[i].val);
//                    break;
//
//                case variable: //变量
//                    gen(lod, lev-table[i].level, table[i].addr);
//                    break;
//
//                case proc:     //过程
//                    error(21);
//                    break;
//                }
            }
            GetSym();
        } else if( sym == SYM_NUMBER){ // 为数字
            // TODO 运用中间代码函数写入到中间代码数组 gen(lit,0,num)

           GetSym();
        } else if(sym == SYM_LPAREN) { // 为左括号
           GetSym();
           Expression(fsys | SYM_RPAREN);
           if(sym == SYM_RPAREN) {
               GetSym();
           } else {
               Error(22)
           }
        }
        Test(fsys,SYM_LPAREN,23); // 一个因子处理完毕，遇到的token应在fsys集合中
    }
}


void Term(unsigned long fsys) {
   unsigned  long mulop;
   Factor(fsys | SYM_TIMES | SYM_SLASH);  // 判断*、/

   while (sym == SYM_TIMES || sym == SYM_SLASH) { // 处理乘除法

        mulop = sym; // 保留当前运算符
        GetSym();
        Factor(fsys | SYM_TIMES | SYM_SLASH);
        if(mulop == SYM_TIMES) { // 若为*
            //TODO 生成乘法的中间代码    gen(opr,0,4);

        } else {
            // TODO 生成除法的中间代码   gen(opr,0,5);
        }
   }

}

/**
 * exp 处理
 */
void Expression(unsigned long fsys) {
   unsigned long addop;
   if(sym == SYM_PLUS || sym == SYM_MINUS) { // 处理正负号
        addop = sym; // 保存当前符号
                     // 存正负号
        GetSym();
        Term(fsys | SYM_PLUS | SYM_MINUS);

        if(addop == SYM_MINUS) { // 若为符号
           // TODO 生成负号的中间代码    gen(opr,0,1);  负号，取反运算
        }
   } else {
       Term(fsys | SYM_PLUS | SYM_MINUS);
   }

   while (sym == SYM_PLUS || sym == SYM_MINUS) { // 处理加减
      addop = sym;
      GetSym();

      Term(fsys | SYM_PLUS | SYM_MINUS);

      if(addop == SYM_PLUS) {
          // TODO 生成加法的中间代码  gen(opr,0,2);          // 加
      } else {
          // TODO 生成减法的中间代码   gen(opr,0,3);          // 减
      }
   }
}

// cond 处理

void Condition(unsigned  long fsys) {
    unsigned long relop;
    if(sym == SYM_ODD) { // 一元运算符
        GetSym();
        Expression(fsys);
        // TODO 生成运算符的中间代码  gen(opr, 0, 6);


    } else {  // 二元运算符
        Expression(fsys | SYM_EQ | SYM_NEQ | SYM_GTR | SYM_LES | SYM_LEQ | SYM_GEQ);
        if(!(sym &(SYM_EQ | SYM_NEQ | SYM_GTR | SYM_LES | SYM_LEQ | SYM_GEQ))) {
            Error(20);
        } else {
            relop = sym; // 保存当前运算符
            GetSym();
            Expression(fsys); // 处理新的正负号

            // TODO switch生成对应的中间代码
//            switch(relop)
//            {
//            case eql:
//                gen(opr, 0, 8);
//                break;
//
//            case neq:
//                gen(opr, 0, 9);
//                break;
//
//            case lss:
//                gen(opr, 0, 10);
//                break;
//
//            case geq:
//                gen(opr, 0, 11);
//                break;
//
//            case gtr:
//                gen(opr, 0, 12);
//                break;
//
//            case leq:
//                gen(opr, 0, 13);
//                break;
//            }

        }
    }
}

// 声明处理

void statement(unsigned long fsys) {
    long i,cx1,cx2;

    if(sym == SYM_IDENTIFIER) { // 标识符
        // TODO 查找id在符号表对应的索引   i=position(id);

        if(i == 0) {
            Error(11); // 未定义错误
        } else if(table[i].kind != SYM_VAR) { // 为非变量
            Error(12);
            i = 0;
        }

        GetSym();

        if(sym == SYM_BECOMES) { // 若为赋值
            GetSym();
        } else {
            Error(13);
        }

        Expression(fsys);

        if(i != 0) {  // 产生一个sto代码
            // TODO  生成sto中间代码  gen(sto,lev-table[i].level,table[i].addr);
        }
    } else if(sym == SYM_CALL) { // call语句
        GetSym();
        if(sym != SYM_IDENTIFIER) {
            Error(14);
        } else {
            // TODO id在符号表中的位置  i=position(id);

            if(i == 0) {
                Error(11);
            } else if(table[i].kind == SYM_PROCEDURE) { // 若为过程
                // TODO 生成中间代码   gen(cal,lev-table[i].level,table[i].addr);

            } else {
                Error(15);
            }
            GetSym();
        }
    } else if(sym == SYM_IF) { // 若为if语句
       GetSym();
       Condition(fsys | SYM_THEN | SYM_DO);

       if(sym == SYM_THEN) {
           GetSym();
       } else {
           Error(16);
       }
       cx1 = cx;
       // TODO 生成中间代码  gen(jpc,0,0);

       statement(fsys); // 后面一个stament
       code[cx1].a = cx;
    }  else if(sym == SYM_BEGIN) { // beign语句
       GetSym();
       statement(fsys | SYM_SEMICOLON | SYM_END);
       while ((sym == SYM_SEMICOLON) || (sym&SYM_END)) { //处理分号和语句
           if(sym == SYM_SEMICOLON) // 分号
           {
               GetSym();
           } else {
               Error(10);
           }
           statement(fsys | SYM_SEMICOLON|SYM_END);
       }

       if(sym == SYM_END) {
           GetSym();
       } else {
           Error(17);
       }
    } else if(sym == SYM_WHILE) { // while语句
        cx1 = cx; // 记录中间代码起始指针
        GetSym();
        Condition(fsys | SYM_DO);
        cx2 = cx; // 记录中间代码位置，要放退出地址

       // TODO  gen(jpc,0,0);

        if(sym == SYM_DO) { // do语句
           GetSym();
        } else {
            Error(18);
        }
        statement(fsys) // 后面是stmt

        // TODO gen(jmp,0,cx1);  循环跳转

            code[cx2].a = cx; // 将退出地址补上
    }
    Test(fsys,0,19);
}

