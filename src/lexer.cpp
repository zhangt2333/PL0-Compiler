/*
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * lexer.cpp 2020/4/13
 * Comments:
 */

//#define DEBUG    // 调试模式

#include <iostream>
#include <cctype>
#include "utils.h"
#include "lexer.h"

char Lexer::get()
{
    char ch = inputStream.get();
    logs("LOG[Lexer: get]: ", ch);
    return ch;
}

char Lexer::peek()
{
    char ch = inputStream.peek();
    logs("LOG[Lexer: peek]: ", ch);
    return ch;
}

bool Lexer::isEOF()
{
    return inputStream.eof();
}

Symbol Lexer::getSymbol()
{
    if (isEOF())
        return Symbol(SYMBOL::END_OF_FILE);

    while(isspace(peek()))
        get();

    std::vector<char> strToken;
    if (isalpha(peek()))
    {
        do
        {
            strToken.emplace_back(get());
        }while(isalpha(peek()));
        std::string value(strToken.begin(), strToken.end());
        auto it = SYMBOL::KEYWORD_MAP.find(value);
        return it != SYMBOL::KEYWORD_MAP.end() ? it->second : Symbol(SYMBOL::IDENTIFIER, value);
    }
    if (isdigit(peek()))
    {
        do
        {
            strToken.emplace_back(get());
        }while(isdigit(peek()));
        int number = vectorToInt(strToken);
        return Symbol(SYMBOL::NUMBER, number);
    }
    std::string str(1, get());
    char pk = peek();
    switch (str[0])
    {
        case '<': case '>': case ':':
            if(pk == '=') str += pk; break;
    }
    auto it = SYMBOL::SIGN_MAP.find(str);
    logs("LOG[getSymbol: ToFind]", str, "\n");
    return it != SYMBOL::SIGN_MAP.end() ? it->second : Symbol(SYMBOL::ILLEGAL, str);
}



