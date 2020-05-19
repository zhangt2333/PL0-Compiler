/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * symbolTable.cpp 2020/4/21
 * Comments: 
 */


#include <iostream>
#include <iomanip>
#include "symbolTable.h"

bool SymbolTable::inTable(const std::string &name)
{
    return mp.find(name) != mp.end();
}

void SymbolTable::addSymbol(Symbol *symbol)
{
    mp[symbol->getValue()] = symbol;
    lst.push_back(symbol);
}

void SymbolTableManager::addSymbol(const SymbolType &symbolType, const std::string &name, int number, int level, int address)
{
    symbolTableStack.back()->addSymbol(new Symbol(symbolType, name, number, level, address));
}

Symbol *SymbolTableManager::getLastProcedure()
{
    for (auto it = symbolTableList.rbegin(); it != symbolTableList.rend(); it++)
        for (auto it2 =  (*it)->lst.rbegin(); it2 != (*it)->lst.rend(); it2++)
            if (SYMBOL::PROCEDURE == (*it2)->getSymbolType())
                return *it2;
    return nullptr;
}

bool SymbolTableManager::inTable(const std::string &name)
{
    return symbolTableStack.back()->inTable(name);
}

Symbol *SymbolTableManager::getSymbol(const std::string &name)
{
    for (auto *t : symbolTableStack)
    {
        auto it = t->mp.find(name);
        if (it != t->mp.end())
            return it->second;
    }
    return nullptr;
}

void SymbolTableManager::pushTable()
{
    symbolTableStack.push_back(new SymbolTable());
    symbolTableList.push_back(symbolTableStack.back());
}

void SymbolTableManager::popTable()
{
    symbolTableStack.pop_back();
}

void SymbolTableManager::printTables()
{
    int idx = 0;
    for (auto *t : symbolTableList)
    {
        std::cout << "TX" << idx++ << "->" << std::endl;
        for (auto *symbol : t->lst)
        {
            std::cout << "NAME: ";
            std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10) << symbol->getValue();
            std::cout << "KIND: ";
            std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                      << symbol->getSymbolType().name;
            if (SYMBOL::CONST == symbol->getSymbolType())
            {
                std::cout << "VAL: ";
                std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                          << symbol->getNumber();
            } else
            {
                std::cout << "LEVEL: ";
                std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                          << symbol->getLevel();
                std::cout << "ADR: ";
                std::cout << std::setiosflags(std::ios::left) << std::setfill(' ') << std::setw(10)
                          << symbol->getAddress();
            }
            std::cout << std::endl;
        }
    }
}

