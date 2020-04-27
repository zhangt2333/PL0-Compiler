/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * symbolTable.h 2020/4/21
 * Comments: 
 */
#ifndef PL0_COMPILER_SYMBOLTABLE_H
#define PL0_COMPILER_SYMBOLTABLE_H

#include <unordered_map>
#include <list>
#include "symbol.h"

class SymbolTable
{
public:
    std::unordered_map<std::string, Symbol*> mp;
    std::list<Symbol*> lst;
    bool inTable(const std::string& name);
    void addSymbol(Symbol* symbol);
};

class SymbolTableManager
{
    std::list<SymbolTable*> symbolTableStack;  // 符号表 栈
    std::list<SymbolTable*> symbolTableList; // 符号表 数组
public:
    SymbolTableManager()
    {
        pushTable();
    }

    void addSymbol(const SymbolType& symbolType, const std::string &name, int number, int level, int address);
    Symbol *getLastProcedure();
    bool inTable(const std::string& name);
    Symbol *getSymbol(const std::string &name);
    void pushTable();
    void popTable();
    void printTables();
};


#endif // PL0_COMPILER_SYMBOLTABLE_H
