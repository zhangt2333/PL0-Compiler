/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * parser.cpp 2020/4/19
 * Comments: 
 */
#include "parser.h"

parser::parser(Lexer &lexer) : lexer(lexer)
{
    advance();
}

void parser::advance()
{
    nowSymbol = lexer.getSymbol();
    nowSymbolType = nowSymbol.getSymbolType();
}

void parser::addressAdvance()
{

}

// 〈程序〉→〈分程序〉
void parser::program()
{
}

// 〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
void parser::subProbgram()
{
}

// 〈常量说明部分〉 → CONST〈常量定义〉{ ,〈常量定义〉}；
void parser::constDeclare()
{
}

// 〈常量定义〉 → 〈标识符〉=〈无符号整数〉
// 〈无符号整数〉 → 〈数字〉{〈数字〉}
void parser::constDefine()
{
}

// 〈变量说明部分〉 → VAR〈标识符〉{ ,〈标识符〉}；
void parser::varDeclare()
{
}

// 〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
// 〈过程首部〉 → procedure〈标识符〉；
void parser::procDeclare()
{
}

// 〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空〉
void parser::statement()
{
}

// 〈赋值语句〉 → 〈标识符〉:=〈表达式〉
void parser::assignStatement()
{
}

// 〈条件语句〉 → if〈条件〉then〈语句〉
void parser::ifStatement()
{
}

// 〈当型循环语句〉 → while〈条件〉do〈语句〉
void parser::whileStatement()
{
}

// 〈过程调用语句〉 → call〈标识符〉
void parser::callStatement()
{
}

// 〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
void parser::readStatement()
{
}

// 〈写语句〉 → write(〈标识符〉{，〈标识符〉})
void parser::writeStatement()
{
}

// 〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
void parser::beginStatement()
{
}

// 〈分号〉
void parser::semicolon()
{
}

// 〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
// 〈加减运符〉 → +|-
void parser::expression()
{
}

// 〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
// 〈乘除运算符〉 → *|/
void parser::term()
{
}

// 〈因子〉 → 〈标识符〉|〈无符号整数〉|(〈表达式〉)
void parser::factor()
{
}

// 〈条件〉 → 〈表达式〉〈关系运算符〉〈表达式〉|odd〈表达式〉
// 〈关系运算符〉 → =|#|<|<=|>|>=
void condition()
{
}

