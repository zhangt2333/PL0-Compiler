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
    enum ExceptionEnum{
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
    std::unordered_map<ExceptionEnum, std::string> EXCEPTION_ENUM_MAP({
        {EXTRA_CHARACTERS, "There is extra char after '.' !"},
        {EXTRA_CHARACTERS, "Here is missing a semicolon !"},
        {MISSING_SEMICOLON, "Here is missing a identifier !"},
        {MISSING_IDENTIFIER, "MISSING_IDENTIFIER !"},
        {DUPLICATE_IDENTIFIER, "DUPLICATE_IDENTIFIER"},
        {MISSING_EQUAL, "MISSING_EQUAL"},
        {MISSING_CEQUAL, "MISSING_CEQUAL"},
        {MISSING_NUMBER, "MISSING_NUMBER"},
        {MISSING_THEN, "MISSING_THEN"},
        {MISSING_DO, "MISSING_DO"},
        {MISSING_LBR, "MISSING_LBR"},
        {MISSING_RBR, "MISSING_RBR"},
        {MISSING_END, "MISSING_END"},
        {MISSING_SEMIC, "MISSING_SEMIC"},
        {NOT_A_VAR, "NOT_A_VAR"},
        {NOT_A_PROCUDURE, "NOT_A_PROCUDURE"},
        {NEVER_DECLARE, "NEVER_DECLARE"}
    });

};
void ASSERT(bool _, EXCEPTION::ExceptionEnum parserExceptionEnum)
{ // TODO: 更详细的异常信息、更方便的异常处理
    if (_) return;
    std::cout << "-------------------"<< EXCEPTION::EXCEPTION_ENUM_MAP[parserExceptionEnum] << std::endl;
    exit(1);
}


#endif //PL0_COMPILER_EXCEPTIONHANDLER_H
