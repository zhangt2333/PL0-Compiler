/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * utils.h 2020/4/14
 * Comments: 
 */
#ifndef PL0_COMPILER_UTILS_H
#define PL0_COMPILER_UTILS_H

#ifdef DEBUG
#include <iostream>
void dbg() { std::cout << "\n"; }
template<typename T, typename... A>
void dbg(T a, A... x) { std::cout << a << ' '; dbg(x...); }
#define logv(x...) std::cout << #x << " -> "; dbg(x);
#define logs(x...) dbg(x);
#else
#define logs(...)
#define logv(...)
#endif



#include <vector>
#include <cassert>
#include <cctype>

int vectorToInt(std::vector<char> &v)
{
    int num = 0;
    for (char c : v)
    {
        assert(isdigit(c));
        num = num * 10 + (c - '0');
    }
    return num;
}


#endif //PL0_COMPILER_UTILS_H
