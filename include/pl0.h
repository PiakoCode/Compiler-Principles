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
 *
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

struct Instruction {
    int f; // function code
    int l; // level
    int a; // displacement address
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

static char ch = 0;          // 读取到的字符
static int  cc = 0;          // char count
static int  line_length = 0; // line length
static int  error_cnt = 0;
static int  current_instruction_index = 0; // 当前指令的索引
static char line[1000] = {};

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

void Getch();

static FILE *infile;

#endif