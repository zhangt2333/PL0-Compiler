/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * parser.h 2020/4/19
 * Comments: 
 */
#ifndef PL0_COMPILER_PARSER_H
#define PL0_COMPILER_PARSER_H


#include "lexer.h"
#include "symbolTable.h"
#include "code.h"

class parser
{
    Lexer &lexer;
    int level = 0;
    int address = 3;
    SymbolType nowSymbolType = SYMBOL::ILLEGAL;
    Symbol nowSymbol = Symbol(nowSymbolType);
    SymbolTable symbolTable;
    std::vector<Code> codeTable;
public:
    explicit parser(Lexer &lexer);

private:
    void advance();
    void addressAdvance();
    void addressReset();

    // 〈程序〉→〈分程序〉
    void program();
    // 〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
    void subProbgram();
    // 〈常量说明部分〉 → CONST〈常量定义〉{ ,〈常量定义〉}；
    void constDeclare();
    // 〈常量定义〉 → 〈标识符〉=〈无符号整数〉
    void constDefine();
    // 〈变量说明部分〉 → VAR〈标识符〉{ ,〈标识符〉}；
    void varDeclare();
    // 〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
    void procDeclare();
    // 〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空〉
    void statement();
    // 〈赋值语句〉 → 〈标识符〉:=〈表达式〉
    void assignStatement();
    // 〈条件语句〉 → if〈条件〉then〈语句〉
    void ifStatement();
    // 〈当型循环语句〉 → while〈条件〉do〈语句〉
    void whileStatement();
    // 〈过程调用语句〉 → call〈标识符〉
    void callStatement();
    // 〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
    void readStatement();
    // 〈写语句〉 → write(〈标识符〉{，〈标识符〉})
    void writeStatement();
    // 〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
    void beginStatement();
    // 〈分号〉
    void semicolon();
    // 〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
    void expression();
    // 〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
    void term();
    // 〈因子〉 → 〈标识符〉|〈无符号整数〉|(〈表达式〉)
    void factor();
};

#endif //PL0_COMPILER_PARSER_H
