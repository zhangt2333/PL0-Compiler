/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * vm.cpp 2020/5/18
 * Comments: 
 */
#include <iostream>
#include <cstring>
#include "vm.h"


int VM::getBaseAddress(int nowBaseAddress, int levelDiff)
{
    while (levelDiff--)
        nowBaseAddress = stack[nowBaseAddress + 1];
    return nowBaseAddress;
}

void VM::run()
{
    top = pc = base = 0;
    memset(stack, 0, sizeof(stack));
    do
    {
        code = codeTable.at(pc++);
        switch (code->getF())
        {
            // 常量送栈顶
            case CODE::LIT:
                stack[top++] = code->getA();
                break;
            // 变量送栈顶
            case CODE::LOD:
                stack[top++] = stack[getBaseAddress(base, code->getL()) + code->getA()];
                break;
            // 栈顶送变量
            case CODE::STO:
                stack[getBaseAddress(base, code->getL()) + code->getA()] = stack[--top];
                break;
            // 调用
            case CODE::CAL:
                stack[top] = base;   // 静态连
                stack[top + 1] = getBaseAddress(base, code->getL()); // 动态链
                stack[top + 2] = pc; // 返回地址
                base = top;          // 不修改top，前面已将address+3，生成code后会产生INT语句，修改top值
                pc = code->getA();
                break;
            // 开辟空间
            case CODE::INT:
                top = top + code->getA(); // 3(静态链、动态链、返回地址)+变量+常量
                break;
            // 无条件跳转
            case CODE::JMP:
                pc = code->getA();
                break;
            // 有条件跳转
            case CODE::JPC:
                if (stack[top - 1] == 0)
                    pc = code->getA();
                break;
            // 操作
            case CODE::OPR:
                this->opr();
                break;
        }
    } while (pc != 0);
}

void VM::opr()
{
    switch (code->getA())
    {
        case OP::RET:
            top = base;
            pc = stack[base + 2];// 返回地址
            base = stack[base];  // 静态连
            break;
        case OP::NEG:
            stack[top - 1] = -stack[top - 1];
            break;
        case OP::ADD:
            stack[top - 2] = stack[top - 1] + stack[top - 2];
            --top;
            break;
        case OP::SUB:
            stack[top - 2] = stack[top - 2] - stack[top - 1];
            --top;
            break;
        case OP::MUL:
            stack[top - 2] = stack[top - 2] * stack[top - 1];
            --top;
            break;
        case OP::DIV:
            stack[top - 2] = stack[top - 2] / stack[top - 1];
            --top;
            break;
        case OP::LEQ:
            stack[top - 2] = stack[top - 2] <= stack[top - 1];
            --top;
            break;
        case OP::LS:
            stack[top - 2] = stack[top - 2] < stack[top - 1];
            --top;
            break;
        case OP::EQU:
            stack[top - 2] = stack[top - 2] == stack[top - 1];
            --top;
            break;
        case OP::NEQ:
            stack[top - 2] = stack[top - 2] != stack[top - 1];
            --top;
            break;
        case OP::GT:
            stack[top - 2] = stack[top - 2] > stack[top - 1];
            --top;
            break;
        case OP::GEQ:
            stack[top - 2] = stack[top - 2] >= stack[top - 1];
            --top;
            break;
        case OP::WRITE:
            std::cout << stack[top - 1] << '\t';
            break;
        case OP::LINE:
            std::cout << std::endl;
            break;
        case OP::READ:
            std::cout << "Please Input: ";
            std::cin >> stack[top++];
            break;
        case OP::ODD:
            stack[top - 1] &= 1;
            break;
    }
}
