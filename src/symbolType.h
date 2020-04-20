/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * symbolType.h 2020/4/13
 * Comments: 
 */
#ifndef PL0_COMPILER_SYMBOLTYPE_H
#define PL0_COMPILER_SYMBOLTYPE_H

#include <string>
#include <utility>
#include <ostream>

/*
 * 符号类型的承载类，完善枚举类不能实现的多值组
 */
class SymbolType
{
public:
    std::string name;
    int id;
    SymbolType() {}
    SymbolType(std::string name, int id) : name(std::move(name)), id(id) {}

    friend std::ostream &operator<<(std::ostream &os, const SymbolType &type)
    {
        os << "[name: " << type.name << ", id: " << type.id << "]";
        return os;
    }

    bool operator==(const SymbolType &rhs) const
    {
        return name == rhs.name && id == rhs.id;
    }

    bool operator!=(const SymbolType &rhs) const
    {
        return !(rhs == *this);
    }
};

namespace SYMBOL {

const SymbolType/* 关键字 */
                BEGIN       = SymbolType("begin", 1),
                END         = SymbolType("end", 2),
                IF          = SymbolType("if", 3),
                THEN        = SymbolType("then", 4),
                ELSE        = SymbolType("else", 5),
                CONST       = SymbolType("const", 6),
                PROCEDURE   = SymbolType("procedure", 7),
                VAR         = SymbolType("var", 8),
                DO          = SymbolType("do", 9),
                WHILE       = SymbolType("while", 10),
                CALL        = SymbolType("call", 11),
                READ        = SymbolType("read", 12),
                WRITE       = SymbolType("write", 13),
                ODD         = SymbolType("odd", 14),
                /* 算符 */
                EQU         = SymbolType("=", 15),
                LE          = SymbolType("<", 16),
                LEQ         = SymbolType("<=", 17),
                GT          = SymbolType(">", 18),
                GEQ         = SymbolType(">=", 19),
                NEQ         = SymbolType("<>", 20),
                ADD         = SymbolType("+", 21),
                SUB         = SymbolType("-", 22),
                MUL         = SymbolType("*", 23),
                DIV         = SymbolType("/", 24),
                /* 界符 */
                CEQU        = SymbolType(":=", 26),
                COMMA       = SymbolType(",", 27),
                SEMIC       = SymbolType(";", 28),
                POINT       = SymbolType(".", 29),
                LBR         = SymbolType("(", 30),
                RBR         = SymbolType(")", 31),
                /* 标识符, 常量 */
                IDENTIFIER  = SymbolType("identifier", 32),
                NUMBER      = SymbolType("number", 33),
                /* 终结符 */
                END_OF_FILE = SymbolType("$", 34),
                /* 非法符 */
                ILLEGAL     = SymbolType("illegal", 35);
}
#endif //PL0_COMPILER_SYMBOLTYPE_H
