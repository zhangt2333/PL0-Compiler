/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * code.h 2020/4/21
 * Comments: 
 */
#ifndef PL0_COMPILER_CODE_H
#define PL0_COMPILER_CODE_H

namespace CODE {
    enum CODE_TYPE {
        LIT, LOD, STO, CAL, INT, JMP, JPC, OPR
    };
}


class Code
{
    CODE::CODE_TYPE f;
    int l = 0;
    int a = 0;
    explicit Code(CODE::CODE_TYPE _f): f(_f)
    {}
    Code(CODE::CODE_TYPE _f, int _l, int _a): f(_f), l(_l), a(_a)
    {}
};


#endif //PL0_COMPILER_CODE_H
