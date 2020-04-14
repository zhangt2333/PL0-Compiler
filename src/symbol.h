/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * Symbol.h 2020/4/14
 * Comments: 
 */
#ifndef PL0_COMPILER_SYMBOL_H
#define PL0_COMPILER_SYMBOL_H


#include <utility>
#include <unordered_map>
#include <ostream>

#include "symbolType.h"

/*
 * 符号类
 */
class Symbol
{
public:
    explicit Symbol(SymbolType symbolType): symbolType(std::move(symbolType))
    {
        this->number = 0;
    }
    Symbol(SymbolType symbolType, std::string value): symbolType(std::move(symbolType)), value(std::move(value))
    {
        this->number = 0;
    }
    Symbol(SymbolType symbolType, int number): symbolType(std::move(symbolType)), number(number)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, const Symbol &symbol)
    {
        os << "[symbolType: " <<  symbol.symbolType << ", value: " << symbol.value << ", number: " << symbol.number << "]";
        return os;
    }

private:
    SymbolType symbolType;
    std::string value;
    int number;
};

namespace SYMBOL {

const std::unordered_map<std::string, Symbol> KEYWORD_MAP({
    /* 关键词 */
    {SYMBOL::BEGIN.name,     Symbol(SYMBOL::BEGIN)},
    {SYMBOL::END.name,       Symbol(SYMBOL::END)},
    {SYMBOL::IF.name,        Symbol(SYMBOL::IF)},
    {SYMBOL::THEN.name,      Symbol(SYMBOL::THEN)},
    {SYMBOL::ELSE.name,      Symbol(SYMBOL::ELSE)},
    {SYMBOL::CONST.name,     Symbol(SYMBOL::CONST)},
    {SYMBOL::PROCEDURE.name, Symbol(SYMBOL::PROCEDURE)},
    {SYMBOL::VAR.name,       Symbol(SYMBOL::VAR)},
    {SYMBOL::DO.name,        Symbol(SYMBOL::DO)},
    {SYMBOL::WHILE.name,     Symbol(SYMBOL::WHILE)},
    {SYMBOL::CALL.name,      Symbol(SYMBOL::CALL)},
    {SYMBOL::READ.name,      Symbol(SYMBOL::READ)},
    {SYMBOL::WRITE.name,     Symbol(SYMBOL::WRITE)},
    {SYMBOL::ODD.name,       Symbol(SYMBOL::ODD)}
});

const std::unordered_map<std::string, Symbol> SIGN_MAP({
     /* 算符 */
    {SYMBOL::EQU.name,   Symbol(SYMBOL::EQU)},
    {SYMBOL::LE.name,    Symbol(SYMBOL::LE)},
    {SYMBOL::LEQ.name,   Symbol(SYMBOL::LEQ)},
    {SYMBOL::GT.name,    Symbol(SYMBOL::GT)},
    {SYMBOL::GEQ.name,   Symbol(SYMBOL::GEQ)},
    {SYMBOL::NEQ.name,   Symbol(SYMBOL::NEQ)},
    {SYMBOL::ADD.name,   Symbol(SYMBOL::ADD)},
    {SYMBOL::SUB.name,   Symbol(SYMBOL::SUB)},
    {SYMBOL::MUL.name,   Symbol(SYMBOL::MUL)},
    {SYMBOL::DIV.name,   Symbol(SYMBOL::DIV)},
    /* 界符 */
    {SYMBOL::COLON.name, Symbol(SYMBOL::COLON)},
    {SYMBOL::CEQU.name,  Symbol(SYMBOL::CEQU)},
    {SYMBOL::COMMA.name, Symbol(SYMBOL::COMMA)},
    {SYMBOL::SEMIC.name, Symbol(SYMBOL::SEMIC)},
    {SYMBOL::POINT.name, Symbol(SYMBOL::POINT)},
    {SYMBOL::LBR.name,   Symbol(SYMBOL::LBR)},
    {SYMBOL::RBR.name,   Symbol(SYMBOL::RBR)}
});


}


#endif //PL0_COMPILER_SYMBOL_H
