/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * symbolTable.cpp 2020/4/21
 * Comments: 
 */
#include "symbolTable.h"

void SymbolTable::addSymbol(const SymbolType& symbolType, const std::string &name, int number, int level, int address)
{
    symbolList.emplace_back(symbolType);
    symbolList.back().setValue(name);
    symbolList.back().setNumber(number);
    symbolList.back().setLevel(level);
    symbolList.back().setAddress(address);
    symbolMap[name] = symbolList.back();
}

Symbol *SymbolTable::getLastProcedure()
{
    for(int i=symbolList.size()-1; i>=0; i--)
        if(SYMBOL::PROCEDURE == symbolList[i].getSymbolType())
            return &symbolList[i];
    return nullptr;
}

bool SymbolTable::inTable(const std::string& name)
{
    return symbolMap.find(name) != symbolMap.end();
}

Symbol *SymbolTable::getSymbol(const std::string &name)
{
    auto it = symbolMap.find(name);
    if (it != symbolMap.end())
        return &(it->second);
    return nullptr;
}

