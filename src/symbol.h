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
    }

    Symbol(SymbolType symbolType, std::string value): symbolType(std::move(symbolType)), value(std::move(value))
    {
    }

    Symbol(SymbolType symbolType, int number): symbolType(std::move(symbolType)), number(number)
    {
    }

    Symbol(SymbolType symbolType, std::string name, int _number, int _level, int _address):
        symbolType(std::move(symbolType)),
        value(std::move(name)),
        number(_number),
        level(_level),
        address(_address)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, const Symbol &symbol)
    {
        os << symbol.symbolType.name;
        if (!symbol.value.empty())
            os << '(' << symbol.value << ')';
        else if(-1 != symbol.number)
            os << '(' << symbol.number << ')';
        return os;
    }

    const SymbolType &getSymbolType() const
    {
        return symbolType;
    }

    const std::string &getValue() const
    {
        return value;
    }

    int getNumber() const
    {
        return number;
    }

    int getLevel() const
    {
        return level;
    }

    int getAddress() const
    {
        return address;
    }

    void setLevel(int _level)
    {
        this->level = _level;
    }

    void setAddress(int _address)
    {
        this->address = _address;
    }

    void setValue(const std::string &_value)
    {
        this->value = _value;
    }

    void setSymbolType(const SymbolType &_symbolType)
    {
        this->symbolType = _symbolType;
    }

    void setNumber(int _number)
    {
        this->number = _number;
    }
private:
    SymbolType symbolType;
    std::string value = "";
    int number = -1;
    int level = -1;
    int address = -1;
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
    {SYMBOL::LS.name,    Symbol(SYMBOL::LS)},
    {SYMBOL::LEQ.name,   Symbol(SYMBOL::LEQ)},
    {SYMBOL::GT.name,    Symbol(SYMBOL::GT)},
    {SYMBOL::GEQ.name,   Symbol(SYMBOL::GEQ)},
    {SYMBOL::NEQ.name,   Symbol(SYMBOL::NEQ)},
    {SYMBOL::ADD.name,   Symbol(SYMBOL::ADD)},
    {SYMBOL::SUB.name,   Symbol(SYMBOL::SUB)},
    {SYMBOL::MUL.name,   Symbol(SYMBOL::MUL)},
    {SYMBOL::DIV.name,   Symbol(SYMBOL::DIV)},
    /* 界符 */
    {SYMBOL::CEQU.name,  Symbol(SYMBOL::CEQU)},
    {SYMBOL::COMMA.name, Symbol(SYMBOL::COMMA)},
    {SYMBOL::SEMIC.name, Symbol(SYMBOL::SEMIC)},
    {SYMBOL::POINT.name, Symbol(SYMBOL::POINT)},
    {SYMBOL::LBR.name,   Symbol(SYMBOL::LBR)},
    {SYMBOL::RBR.name,   Symbol(SYMBOL::RBR)}
});


}


#endif //PL0_COMPILER_SYMBOL_H
