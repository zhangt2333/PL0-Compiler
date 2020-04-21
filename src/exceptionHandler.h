/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * exceptionHandler.h 2020/4/21
 * Comments: 
 */
#ifndef PL0_COMPILER_EXCEPTIONHANDLER_H
#define PL0_COMPILER_EXCEPTIONHANDLER_H

#include <iostream>

namespace EXCEPTION {
    enum ParserExceptionEnum{
        MISSING_SEMICOLON,
    };
    void handleException(EXCEPTION::ParserExceptionEnum parserExceptionEnum)
    {
        switch (parserExceptionEnum)
        {
            case EXCEPTION::MISSING_SEMICOLON:
                std::cout << "Here is missing a semicolon !";
                break;

        }
    }
};


#endif //PL0_COMPILER_EXCEPTIONHANDLER_H
