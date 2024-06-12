#pragma once

#include <new>
#include <string>
#include <vector>

#ifndef PL0_H
#define PL0_H

/**
 * @brief
 * 符号类型
 */
enum SymType {
    SYM_NULL,       // NULL
    SYM_IDENTIFIER, // 标识符
    SYM_NUMBER,     // 数字
    SYM_PLUS,       // +
    SYM_MINUS,      // -
    SYM_TIMES,      // *
    SYM_SLASH,      // /
    SYM_ODD,        // 奇数
    SYM_EQ,         // ==
    SYM_NEQ,        // !=
    SYM_LES,        // <
    SYM_LEQ,        // <=
    SYM_GTR,        // >
    SYM_GEQ,        // >=
    SYM_LPAREN,     // (
    SYM_RPAREN,     // )
    SYM_COMMA,      // ,
    SYM_SEMICOLON,  // ;
    SYM_PERIOD,     // .
    SYM_BECOMES,    // :=
    SYM_BEGIN,      // 开始 begin
    SYM_END,        // 结束 end
    SYM_IF,         // if
    SYM_ELSE,       // else
    SYM_THEN,       // then
    SYM_WHILE,      // while
    SYM_DO,         // do
    SYM_CALL,       // call
    SYM_CONST,      // const
    SYM_VAR,        // var
    SYM_PROCEDURE,  // procedure
};
/**
 * @brief
 * 标识符类型
 */
enum IdType {
    ID_CONSTANT,  // 常量
    ID_VARIABLE,  // 变量
    ID_PROCEDURE, // 过程
};

/**
 * @brief
 * 中间代码指令
 */
enum OpCode {
    LIT, // 将常数置于栈顶
    LOD, // 将变量值置于栈顶
    STO, // 将栈顶的值赋与某变量
    CAL, // 用于过程调用的指令
    INT, // 在数据栈中分配存贮空间
    JMP, // 跳转
    JPC, // 跳转（条件）
    NOP, // 空操作
};

/**
 * @brief
 * 运算操作
 */
enum OprCode {
    RET, // 返回
    NEG, // 负值
    ADD, // 加法
    MIN, // 减法
    MUL, // 乘法
    DIV, // 除法
    MOD, // 取模
    EQL, // 相等
    NEQ, // 不相等
    LES, // 小于
    LEQ, // 小于等于
    GTR, // 大于
    GEQ, // 大于等于
};

/**
 * @brief
 * 指令类型
 */
struct Instruction {
    OpCode f; // 功能
    int    l; // 层次差
    int    a; // 数值/地址
};

struct Comtab {
    std::string name;
    int         kind;
    int         value;
};

struct Mask {
    std::string name;
    int         kind;
    int         level;
    int         address;
};

extern SymType sym;         // 最近一次的token类型
extern char    ch;          // 最近一次从文件中读出的字符
extern int     cc;          // 在这一行中的位置
extern int     line_length; // 行长度
extern int     error_cnt;   //
extern int cx; // 代码分配指针，代码生成模块总在cx所指的位置生成新代码

extern int line_num; // 行号

extern std::vector<std::string> texts;


// 错误信息
const static std::string error_msg[] = {
    "",
    "Found ':=' when expecting '='.",
    "There must be a number to follow '='.",
    "There must be an identifier to follow 'const', 'var', or 'procedure'.",
    "Missing ',' or ';'.",
    // TODO: 添加更多error message
};

/**
 * @brief
 * 输出错误
 * @param 错误类型
 */
void Error(int n);

/**
 * @brief
 * 读取字符
 */
void Getch();

/**
 * @brief Get the Sym object
 * 获取下一个token
 */
void GetSym();

/**
 * 判断单词是否匹配
 */
void Test(unsigned long s1, unsigned long s2, long n);

/**
 * 常量声明
 */
void ConstDeclaration();
#endif