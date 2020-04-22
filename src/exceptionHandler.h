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
        EXTRA_CHARACTERS,
        MISSING_SEMICOLON,
        MISSING_IDENTIFIER,
        DUPLICATE_IDENTIFIER,
        MISSING_EQUAL,
        MISSING_CEQUAL,
        MISSING_NUMBER,
        MISSING_THEN,
        MISSING_DO,
        MISSING_LBR,
        MISSING_RBR,
        MISSING_END,
        MISSING_SEMIC,
        NOT_A_VAR,
        NOT_A_PROCUDURE,
        NEVER_DECLARE,
    };
    void handleException(EXCEPTION::ParserExceptionEnum parserExceptionEnum)
    {
        switch (parserExceptionEnum)
        {
            case EXCEPTION::EXTRA_CHARACTERS:
                std::cerr << "There is extra char after '.' !";
                break;
            case EXCEPTION::MISSING_SEMICOLON:
                std::cerr << "Here is missing a semicolon !";
                break;
            case EXCEPTION::MISSING_IDENTIFIER:
                std::cerr << "Here is missing a identifier !";
                break;
            case EXCEPTION::DUPLICATE_IDENTIFIER:
                std::cerr << "IDENTIFIER DUPLICATE !";
                break;
            // TODO: 异常消息
        }
        exit(1);
    }
};


#endif //PL0_COMPILER_EXCEPTIONHANDLER_H
