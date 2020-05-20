/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * vm.h 2020/5/18
 * Comments: 
 */
#ifndef PL0_COMPILER_VM_H
#define PL0_COMPILER_VM_H


#include <utility>
#include <vector>
#include "code.h"

class VM
{
public:
    VM(std::vector<Code *> codeTable) : codeTable(std::move(codeTable))
    {}
    void run();

private:
    std::vector<Code*> codeTable;
    int stack[1024];
    int top;
    int base;
    int pc;
    Code* code;
    int getBaseAddress(int nowBaseAddress, int levelDiff);
    void opr();
};


#endif //PL0_COMPILER_VM_H
