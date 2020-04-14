/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * lexer.h 2020/4/13
 * Comments: 
 */
#ifndef PL0_COMPILER_LEXER_H
#define PL0_COMPILER_LEXER_H

#include <istream>
#include <vector>
#include "symbol.h"

/**
 * 词法分析器
 */
class Lexer
{
public:
    Lexer(std::istream &_inputStream) : inputStream(_inputStream)
    {}

    Symbol getSymbol();
    bool isEOF();

private:
    std::istream &inputStream;

    char get();

    char peek();
};


#endif //PL0_COMPILER_LEXER_H
