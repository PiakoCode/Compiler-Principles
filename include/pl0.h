#pragma once

#include <map>
#include <set>
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
    SYM_PLUSEQ,     // +=
    SYM_PLUSPLUS,   // ++
    SYM_MINUSEQ,    // -=
    SYM_MINUSMINUS, // --
    SYM_MINUS,      // -
    SYM_TIMES,      // *
    SYM_SLASH,      // /

    SYM_ODD,       // 奇数
    SYM_EQ,        // ==
    SYM_NEQ,       // !=
    SYM_LES,       // <
    SYM_LEQ,       // <=
    SYM_GTR,       // >
    SYM_GEQ,       // >=
    SYM_LPAREN,    // (
    SYM_RPAREN,    // )
    SYM_COMMA,     // ,
    SYM_SEMICOLON, // ;
    SYM_PERIOD,    // .
    SYM_BECOMES,   // :=
    SYM_BEGIN,     // 开始 begin
    SYM_END,       // 结束 end
    SYM_IF,        // if
    SYM_ELSE,      // else
    SYM_THEN,      // then
    SYM_WHILE,     // while
    SYM_FOR,       // for
    SYM_TO,        // to
    SYM_DO,        // do
    SYM_DOWNTO,    // downto
    SYM_CALL,      // call
    SYM_CONST,     // const
    SYM_VAR,       // var
    SYM_PROCEDURE, // procedure
    SYM_WRITE,     // write
    SYM_LBRACKET,  // [
    SYM_RBRACKET,  // ]
    SYM_ARRAY,     // array

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
    OPR, // 执行运算
    LOD, // 将变量值置于栈顶
    STO, // 将栈顶的值赋与某变量
    CAL, // 用于过程调用的指令
    INT, // 在数据栈中分配存贮空间
    JMP, // 跳转
    JPC, // 跳转（条件）
    NOP, // 空操作
    WRT, // 输出
};

/*  lit 0, a : load constant a
    opr 0, a : execute operation a
    lod l, a : load variable l, a
    sto l, a : store variable l, a
    cal l, a : call procedure a at level l
    Int 0, a : increment t-register by a
    jmp 0, a : jump to a
    jpc 0, a : jump conditional to a       */

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

/**
 * @brief
 * 符号表
 */
struct Table {
    std::string name;    // 符号名
    SymType     kind;    // 符号类型
    int         value;   // 符号值
    int         level;   // 层差/偏移地址
    int         address; // 地址
};
extern std::vector<Table> tables; // 符号表
extern int                s[10000];
extern SymType            sym; // 最近一次的token类型
extern char               ch;  // 最近一次从文件中读出的字符
extern int                cc;  // 在这一行中的位置
extern int                line_length; // 行长度
extern int                error_cnt;   //
extern int cx; // 代码分配指针，代码生成模块总在cx所指的位置生成新代码
extern int tx; // 符号表分配指针，符号表模块总在tx所指的位置生成新符号

extern std::string id;    // 最近一次读入的id
extern int         num;   // 最近一次读入的数字
extern int         level; // 层次
extern int         dx;    // 层次中的偏移地址

extern int                      line_num; // 行号
extern std::vector<Instruction> codes;    // 中间代码列表
extern std::vector<std::string> texts;    // 程序文本

// 声明符号集合
const static std::set<SymType> declare_sym = {
    SYM_CONST,
    SYM_VAR,
    SYM_PROCEDURE,

};

// 表达式开始符号集合
const static std::set<SymType> start_sym = {
    SYM_BEGIN, SYM_CALL, SYM_WHILE, SYM_IF, SYM_FOR, SYM_TO, SYM_DO,SYM_DOWNTO,SYM_ELSE};

// factor开始符号集合
const static std::set<SymType> factor_sym = {
    SYM_IDENTIFIER, SYM_NUMBER, SYM_LPAREN};

// 中间代码
static std::map<OpCode, std::string> op_code_str = {{LIT, "LIT"},
                                                    {LOD, "LOD"},
                                                    {STO, "STO"},
                                                    {CAL, "CAL"},
                                                    {INT, "INT"},
                                                    {JMP, "JMP"},
                                                    {JPC, "JPC"},
                                                    {OPR, "OPR"},
                                                    {NOP, "NOP"}};

// 符号对应的symbol类型表
static std::map<char, SymType> ssym = {
    {'+', SYM_PLUS},
    {'-', SYM_MINUS},
    {'*', SYM_TIMES},
    {'/', SYM_SLASH},
    {'<', SYM_LES},
    {'>', SYM_GTR},
    {';', SYM_SEMICOLON},
    {',', SYM_COMMA},
    {'.', SYM_PERIOD},
    {'(', SYM_LPAREN},
    {')', SYM_RPAREN},
    {'=', SYM_EQ},
    {'[', SYM_LBRACKET},
    {']', SYM_RBRACKET},
};

// 保留字表
static std::map<std::string, SymType> words = {
    {"begin", SYM_BEGIN},
    {"call", SYM_CALL},
    {"const", SYM_CONST},
    {"do", SYM_DO},
    {"end", SYM_END},
    {"if", SYM_IF},
    {"odd", SYM_ODD},
    {"procedure", SYM_PROCEDURE},
    {"then", SYM_THEN},
    {"var", SYM_VAR},
    {"while", SYM_WHILE},
    {"write", SYM_WRITE},
    {"for", SYM_FOR},
    {"to", SYM_TO},
    {"downto", SYM_DOWNTO},
    {"else",SYM_ELSE}
};

// extern unsigned long face_begsys; // factor开始符号集合

// 错误信息
const static std::string error_msg[] = {
    "",
    "Found ':=' when expecting '='.",
    "There must be a number to follow '='.",
    "There must be an '=' to follow the identifier.",
    "There must be an identifier to follow 'const', 'var', or 'procedure'.",
    "Missing ',' or ';'.",
    "Incorrect procedure name.",
    "Statement expected.",
    "Follow the statement is an incorrect symbol.",
    "'.' expected.",
    "';' expected.",
    "Undeclared identifier.",
    "Illegal assignment.",
    "':=' expected.",
    "There must be an identifier to follow the 'call'.",
    "A constant or variable can not be called.",
    "'then' expected.",
    "';' or 'end' expected.",
    "'do' expected.",
    "Incorrect symbol.",
    "Relative operators expected.",
    "Procedure identifier can not be in an expression.",
    "Missing ')'.",
    "The symbol can not be followed by a factor.",
    "The symbol can not be as the beginning of an expression.",
    "",
    "",
    "",
    "",
    "",
    "",
    "The number is too great.",
    "There are too many levels."};

/**
 * @brief
 * 输出错误
 * @param 错误类型
 */
void Error(int n);

void Error(const char *);

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
void Test(std::set<SymType> s1, std::set<SymType> s2, int n);

/**
 * @brief
 * 中间代码生成
 * @param x 中间指令
 * @param a
 * @param b
 */
void Gen(Instruction x, int a, int b);

/**
 * @brief
 * 符号表
 * @param k 符号类型
 */
void Entry(IdType k);

/**
 * @brief
 * 查找符号在符号表中的位置
 * @param id 符号名
 * @return int 符号表中的位置
 */
int Position(const std::string &name);

/**
 * 常量声明
 */
void ConstDeclaration();

void Block(std::set<SymType> syms);

/**
 * 变量声明
 */
void VarDeclaration();

/**
 * exp 处理
 */
void Expression(std::set<SymType> syms);

/**
 * @brief
 * 语句处理
 * @param syms
 */
void Statement(std::set<SymType> syms);

// 通过静态链求出数据区基地址
int Base(int b, int l);

/**
 * @brief
 * 解释器
 */
void Interpret();

void Listcode(int cx0);

#endif