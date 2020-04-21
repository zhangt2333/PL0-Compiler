/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * symbolTable.cpp 2020/4/21
 * Comments: 
 */
#include "symbolTable.h"

void SymbolTable::addSymbol(Symbol symbol)
{
    symbolList.push_back(symbol);
    symbolMap[symbol.getValue()] = symbol;
}
