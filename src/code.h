/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * code.h 2020/4/21
 * Comments: 
 */
#ifndef PL0_COMPILER_CODE_H
#define PL0_COMPILER_CODE_H

namespace CODE
{
    enum CODE_TYPE
    {
        LIT, LOD, STO, CAL, INT, JMP, JPC, OPR
    };
}

namespace OP
{
    enum OP_TYPE
    {
        RET = 0,   // 过程调用结束后, 返回调用点并退栈
        NEG = 1,   // 栈顶元素取反
        ADD = 2,   // 弹出次栈顶与栈顶相加，结果进栈
        SUB = 3,   // 弹出次栈顶减去栈顶，结果进栈
        MUL = 4,   // 弹出次栈顶、栈顶，相乘结果进栈
        DIV = 5,   // 弹出次栈顶除以栈顶，结果值进栈
        LEQ = 6,   // 栈顶两个元素弹出，判次栈顶小于等于栈顶结果进栈
        LS = 7,    // 栈顶两个元素弹出，判次栈顶小于栈顶结果进栈
        EQU = 8,   // 弹出栈顶两元素判相等结果入栈顶
        NEQ = 9,   // 弹出栈顶两元素判不等结果入栈顶
        GT = 10,   // 栈顶两个元素弹出，判次栈顶大于栈顶结果进栈
        GEQ = 11,  // 栈顶两个元素弹出，判次栈顶小于等于栈顶结果进栈
        WRITE = 14,// 栈顶值输出至屏幕
        LINE = 15, // 屏幕输出换行
        READ = 16  // 从命令行读一个数到栈顶
    };
}


class Code
{
private:
    CODE::CODE_TYPE f;
    int l = 0;
    int a = 0;
public:
    void setL(int _l)
    {
        this->l = _l;
    }

    void setA(int _a)
    {
        this->a = _a;
    }

    explicit Code(CODE::CODE_TYPE _f) : f(_f)
    {}

    Code(CODE::CODE_TYPE _f, int _l, int _a) : f(_f), l(_l), a(_a)
    {}

    Code(CODE::CODE_TYPE _f, int _a) : f(_f), a(_a)
    {}
};


#endif //PL0_COMPILER_CODE_H
