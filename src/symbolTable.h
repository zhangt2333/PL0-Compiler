/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * symbolTable.h 2020/4/21
 * Comments: 
 */
#ifndef PL0_COMPILER_SYMBOLTABLE_H
#define PL0_COMPILER_SYMBOLTABLE_H

#include <vector>
#include "symbol.h"

class SymbolTable
{
    std::vector<Symbol> symbolList;
    std::unordered_map<std::string, Symbol> symbolMap;
    void addSymbol(Symbol symbol);

};


#endif //PL0_COMPILER_SYMBOLTABLE_H
