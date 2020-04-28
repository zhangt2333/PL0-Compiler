/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * utils.cpp 2020/4/14
 * Comments: 
 */
#include "utils.h"

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