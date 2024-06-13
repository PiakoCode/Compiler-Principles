
#include "pl0.h"
#include "utils.h"
#include <cctype>
#include <cstdio>

#include <set>
#include <vector>

SymType sym = SYM_NULL;  // 最近一次的token类型
char    ch = 0;          // 最近一次从文件中读出的字符
int     cc = 0;          // 在这一行的位置
int     line_length = 0; // 行长度
int     error_cnt = 0;
int cx = 0; // 代码分配指针，代码生成模块总在cx所指的位置生成新代码
int s[10000];
int tx = 0; // 符号表分配指针，符号表模块总在tx所指的位置生成新符号
std::string id;
int         num = 0;   // 最近一次读入的数字
int         level = 0; // 层次
int         dx = 0;    // 层次中的偏移地址

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
void Test(std::set<SymType> s1, std::set<SymType> s2, int n) {
    if (!IsInSet(sym, s1)) { // sym不在s1中
        Error(n);
        s1 = MergeSet(s1, s2);      // 将s2补充到s1中
        while (!IsInSet(sym, s1)) { // 找到下一个合法字符
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
void Entry(IdType k) {
    tx++;
    tables.push_back({id, k, 0, 0, 0});

    switch (k) {

    case ID_CONSTANT: // 常量
        tables[tx].value = num;
        break;

    case ID_VARIABLE: // 变量
        tables[tx].level = level;
        tables[tx].address = dx;
        dx++;
        break;

    case ID_PROCEDURE: // 过程
        tables[tx].level = level;
        break;
    }
}

int Position(const std::string &name) {
    for (int i = 0; i < tx; i++) {
        if (tables[i].name == name) {
            return i;
        }
    }
    return -1;
}

/**
 * 常量声明
 */
void ConstDeclaration() {
    if (sym == SYM_IDENTIFIER) { // 如果是标识符
        GetSym();
        if (sym == SYM_EQ || sym == SYM_BECOMES) // sym为等号或赋值号
        {
            if (sym == SYM_BECOMES) { // 赋值号报错
                Error(1);
            }

            GetSym();

            if (sym == SYM_NUMBER) { // sym为数字记录到符号表
                Entry(ID_CONSTANT);
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

void Block(std::set<SymType> syms) {

    long tx0;
    long cx0;
    long tx1;
    long dx1;

    dx = 3;
    // 地址寄存器给出每层局部量当前已分配到的相对位置
    // 置初始值为 3 的原因是：每一层最开始的位置有三个空间用于存放
    // 静态链 SL、动态链 DL 和 返回地址 RA

    tx0 = tx;                // 记录本层开始时符号表的位置
    tables[tx].address = cx; // 符号表记下当前层代码的开始地址

    Gen(JMP, 0, 0); // block开始时首先写下一句跳转指令，地址到后面再补

    do {
        if (sym == SYM_CONST) { // 常量
            GetSym();
            do {
                ConstDeclaration();        // 常量声明
                while (sym == SYM_COMMA) { // 多个常量声明
                    GetSym();
                    ConstDeclaration();
                }

                if (sym == SYM_SEMICOLON) { // 分号
                    GetSym();
                } else {
                    Message("5");
                }

            } while (sym == SYM_IDENTIFIER);
        }

        if (sym == SYM_VAR) { // 变量
            GetSym();
            do {
                VarDeclaration();
                while (sym == SYM_COMMA) { // 多个变量声明
                    GetSym();
                    VarDeclaration();
                }

                if (sym == SYM_SEMICOLON) {
                    GetSym();
                } else {
                    Message("5");
                }

            } while (sym == SYM_IDENTIFIER);
        }

        while (sym == SYM_PROCEDURE) { // 过程
            GetSym();
            if (sym == SYM_IDENTIFIER) { // 标识符
                Entry(ID_PROCEDURE);
                GetSym();
            } else {
                Error(4);
            }

            if (sym == SYM_SEMICOLON) { // 分号
                GetSym();
            } else {
                Error(5);
            }

            level += 1; // 层数加一
            tx1 = tx;   // 记录当前层级
            dx1 = dx;   // 记录当前数据指针
            Block(MergeSet(syms, CreateSet(SYM_SEMICOLON)));

            level -= 1; // 层数减一
            tx = tx1;
            dx = dx1; // 恢复

            if (sym == SYM_SEMICOLON) { // 分号
                GetSym();
                Test(MergeSet(start_sym,
                              CreateSet(SYM_IDENTIFIER, SYM_PROCEDURE)),
                     syms,
                     6); // 检查当前 token 是否合法，不合法 则用 fsys
                         // 恢复语法分析同时抛 6 号错
            } else {
                Error(5);
            }
        }
        Test(MergeSet(start_sym, CreateSet(SYM_IDENTIFIER)), declare_sym, 7);

    } while (IsInSet(sym, declare_sym));

    codes[tables[tx0].address].a = cx; // 把block开头写下的跳转指令的地址补上
    tables[tx0].address = cx; // tx0的符号表存的是当前block的参数
    cx0 = cx;
    Gen(INT, 0, dx);

    Statement(MergeSet(syms, CreateSet(SYM_SEMICOLON, SYM_END)));

    Gen(OPR, 0, 0); // return
    Test(syms, std::set<SymType>{}, 8);
}

/**
 * 变量声明
 */
void VarDeclaration() {
    if (sym == SYM_IDENTIFIER) {
        Entry(ID_VARIABLE);
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
void Factor(std::set<SymType> fsys) {
    long i;
    Test(factor_sym, fsys, 24); // 检查当前token是否在factor_sym集合中
                                // 若不合法，抛错误，并通过fsys集合恢复护法处理

    while (IsInSet(sym, factor_sym)) { // 当sym在face_bgsys中
        if (sym == SYM_IDENTIFIER)     // 为标识符
        {
            i = Position(id);
            if (i == 0) { // 为找到标识符
                Error(11);
            } else {

                switch (tables[i].kind) {
                case ID_CONSTANT: // 常量
                    Gen(LIT, 0, tables[i].value);
                    break;

                case ID_VARIABLE: // 变量
                    Gen(LOD, level - tables[i].level, tables[i].address);
                    break;

                case ID_PROCEDURE: // 过程
                    Error(21);
                    break;
                }
            }
            GetSym();
        } else if (sym == SYM_NUMBER) { // 为数字

            Gen(LIT, 0, num);
            GetSym();
        } else if (sym == SYM_LPAREN) { // 为左括号
            GetSym();
            Expression(MergeSet(fsys, CreateSet(SYM_RPAREN)));
            if (sym == SYM_RPAREN) {
                GetSym();
            } else {
                Error(22);
            }
            if (sym == SYM_RPAREN) {
                GetSym();
            } else {
                Error(22);
            }
        }
        Test(fsys,
             CreateSet(SYM_LPAREN),
             23); // 一个因子处理完毕，遇到的token应在fsys集合中
    }
}

void Term(std::set<SymType> fsys) {
    SymType mulop;
    Factor(MergeSet(fsys, CreateSet(SYM_TIMES, SYM_SLASH))); // 判断*、/

    while (sym == SYM_TIMES || sym == SYM_SLASH) { // 处理乘除法

        mulop = sym; // 保留当前运算符
        GetSym();
        Factor(MergeSet(fsys, CreateSet(SYM_TIMES, SYM_SLASH)));

        if (mulop == SYM_TIMES) { // 若为*

            Gen(OPR, 0, 4);
        } else {
            Gen(OPR, 0, 5);
        }
    }
}

/**
 * exp 处理
 */
void Expression(std::set<SymType> fsys) {
    SymType addop;
    if (sym == SYM_PLUS || sym == SYM_MINUS) { // 处理正负号
        addop = sym;                           // 保存当前符号
                                               // 存正负号
        GetSym();
        Term(MergeSet(fsys, CreateSet(SYM_PLUS, SYM_MINUS)));

        if (addop == SYM_MINUS) { // 若为符号
            Gen(OPR, 0, 1);
        }
    } else {
        Term(MergeSet(fsys, CreateSet(SYM_PLUS, SYM_MINUS)));
    }

    while (sym == SYM_PLUS || sym == SYM_MINUS) { // 处理加减
        addop = sym;
        GetSym();

        Term(MergeSet(fsys, CreateSet(SYM_PLUS, SYM_MINUS)));

        if (addop == SYM_PLUS) {
            Gen(OPR, 0, 2); // 加
        } else {
            Gen(OPR, 0, 3); // 减
        }
    }
}

// cond 处理

void Condition(std::set<SymType> fsys) {
    int relop;
    if (sym == SYM_ODD) { // 一元运算符
        GetSym();
        Expression(fsys);
        Gen(OPR, 0, 6);

    } else { // 二元运算符
        Expression(MergeSet(
            fsys,
            CreateSet(SYM_EQ, SYM_NEQ, SYM_GTR, SYM_LES, SYM_LEQ, SYM_GEQ)));
        if (!(IsInSet(
                sym,
                CreateSet(
                    SYM_EQ, SYM_NEQ, SYM_GTR, SYM_LES, SYM_LEQ, SYM_GEQ)))) {

            Error(20);
        } else {
            relop = sym; // 保存当前运算符
            GetSym();
            Expression(fsys); // 处理新的正负号

            switch (relop) {
            case EQL:
                Gen(OPR, 0, 8);
                break;

            case NEQ:
                Gen(OPR, 0, 9);
                break;

            case LES:
                Gen(OPR, 0, 10);
                break;

            case GEQ:
                Gen(OPR, 0, 11);
                break;

            case GTR:
                Gen(OPR, 0, 12);
                break;

            case LEQ:
                Gen(OPR, 0, 13);
                break;
            }
        }
    }
}

// 声明处理

void Statement(std::set<SymType> fsys) {
    long i, cx1, cx2;

    if (sym == SYM_IDENTIFIER) { // 标识符
        i = Position(id);

        if (i == 0) {
            Error(11);                          // 未定义错误
        } else if (tables[i].kind != SYM_VAR) { // 为非变量
            Error(12);
            i = 0;
        }

        GetSym();

        if (sym == SYM_BECOMES) { // 若为赋值
            GetSym();
        } else {
            Error(13);
        }

        Expression(fsys);

        if (i != 0) { // 产生一个sto代码
            Gen(STO, level - tables[i].level, tables[i].address);
        }
    } else if (sym == SYM_CALL) { // call语句
        GetSym();
        if (sym != SYM_IDENTIFIER) {
            Error(14);
        } else {
            i = Position(id);

            if (i == 0) {
                Error(11);
            } else if (tables[i].kind == SYM_PROCEDURE) { // 若为过程
                Gen(CAL, level - tables[i].level, tables[i].address);

            } else {
                Error(15);
            }
            GetSym();
        }
    } else if (sym == SYM_IF) { // 若为if语句
        GetSym();
        Condition(MergeSet(fsys, CreateSet(SYM_THEN, SYM_DO)));

        if (sym == SYM_THEN) {
            GetSym();
        } else {
            Error(16);
        }
        cx1 = cx;
        Gen(JPC, 0, 0);

        Statement(fsys); // 后面一个stament
        codes[cx1].a = cx;
    } else if (sym == SYM_BEGIN) { // beign语句
        GetSym();
        Statement(MergeSet(fsys, CreateSet(SYM_SEMICOLON, SYM_END)));
        while ((sym == SYM_SEMICOLON) || (sym & SYM_END)) { // 处理分号和语句
            if (sym == SYM_SEMICOLON)                       // 分号
            {
                GetSym();
            } else {
                Error(10);
            }
            Statement(MergeSet(fsys, CreateSet(SYM_SEMICOLON, SYM_END)));
        }

        if (sym == SYM_END) {
            GetSym();
        } else {
            Error(17);
        }
    } else if (sym == SYM_WHILE) { // while语句
        cx1 = cx;                  // 记录中间代码起始指针
        GetSym();
        Condition(MergeSet(fsys, CreateSet(SYM_DO)));
        cx2 = cx; // 记录中间代码位置，要放退出地址

        Gen(JPC, 0, 0);

        if (sym == SYM_DO) { // do语句
            GetSym();
        } else {
            Error(18);
        }
        Statement(fsys); // 后面是stmt

        Gen(JMP, 0, cx1); //  循环跳转

        codes[cx2].a = cx; // 将退出地址补上
    }
    Test(fsys, std::set<SymType>{}, 19);
}

int Base(int b, int l) {
    int b1;
    b1 = b;

    while (l > 0) {
        b1 = s[b1];
        l = l - 1;
    }
    return b1;
}

void Interpret() {
    int p, b, t;   // 程序寄存器PC、基地址寄存器、栈顶寄存器
    Instruction i; // 指令寄存器

    printf("start PL/0\n");
    t = 0;
    b = 1;
    p = 0;
    s[1] = 0;
    s[2] = 0;
    s[3] = 0;

    do {
        i = codes[p];
        p = p + 1; // 每次在code表中读取一条指令

        switch (i.f) {
        case LIT: // 常数指令
            t = t + 1;
            s[t] = i.a;
            break;

        case OPR: // 运算指令
            switch (i.a) {
            case 0: // 返回指令
                t = b - 1;
                p = s[t + 3];
                b = s[t + 2];
                break;

            case 1: // 负号
                s[t] = -s[t];
                break;

            case 2: // 加法
                t = t - 1;
                s[t] = s[t] + s[t + 1];
                break;

            case 3: // 减法
                t = t - 1;
                s[t] = s[t] - s[t + 1];
                break;

            case 4: // 乘法
                t = t - 1;
                s[t] = s[t] * s[t + 1];
                break;

            case 5: // 除法
                t = t - 1;
                s[t] = s[t] / s[t + 1];
                break;

            case 6: // odd
                s[t] = s[t] % 2;
                break;

            case 8: // ==
                t = t - 1;
                s[t] = (s[t] == s[t + 1]);
                break;

            case 9: // !=
                t = t - 1;
                s[t] = (s[t] != s[t + 1]);
                break;

            case 10: // <
                t = t - 1;
                s[t] = (s[t] < s[t + 1]);
                break;

            case 11: // >=
                t = t - 1;
                s[t] = (s[t] >= s[t + 1]);
                break;

            case 12: // >
                t = t - 1;
                s[t] = (s[t] > s[t + 1]);
                break;

            case 13: // <=
                t = t - 1;
                s[t] = (s[t] <= s[t + 1]);
            }
            break;

        case LOD: // 调用变量值指令
            t = t + 1;
            s[t] = s[Base(b, i.l) + i.a];
            break;

        case STO: // 将值存入变量指令
            s[Base(b, i.l) + i.a] = s[t];
            printf("%10d\n", s[t]);
            t = t - 1;
            break;

        case CAL: // 过程调用，产生新的块标记
            s[t + 1] = Base(b, i.l);
            s[t + 2] = b;
            s[t + 3] = p; // 记录返回地址等参数
            b = t + 1;
            p = i.a;
            break;

        case INT: // 开内存空间
            t = t + i.a;
            break;

        case JMP: // 无条件跳转指令
            p = i.a;
            break;

        case JPC: // 栈顶为0跳转
            if (s[t] == 0) {
                p = i.a;
            }
            t = t - 1;
        }
    } while (p != 0);
    printf("end PL/0\n");
}