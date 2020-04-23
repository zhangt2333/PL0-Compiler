/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * parser.cpp 2020/4/19
 * Comments: 
 */
#include "parser.h"
#include "exceptionHandler.h"

parser::parser(Lexer &lexer) : lexer(lexer)
{
    advance();
}

// 取下一个 词
void parser::advance()
{
    nowSymbol = lexer.getSymbol();
    nowSymbolType = nowSymbol.getSymbolType();
}

// 每个过程的相对起始位置在 BLOCK 内置初值 DX=3
void parser::addressReset()
{
    address = 3;
}

int parser::getCodeAddress()
{
    return codeTable.size();
}

// 〈程序〉→〈分程序〉
void parser::program()
{
    subProbgram();
    if (SYMBOL::POINT == nowSymbolType)
    {
        advance(); // 跳过 '.'
        if (!lexer.isEOF()) // 异常处理: 文件尾 '.' 后还有字符
            EXCEPTION::handleException(EXCEPTION::EXTRA_CHARACTERS);
    } else
    {
        // 异常处理: 缺少 '.'
        EXCEPTION::handleException(EXCEPTION::MISSING_SEMICOLON);
    }
}

// 〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
void parser::subProbgram()
{
    int addressTemp = address; // 保存 address
    addressReset();

    Symbol *procedure = symbolTable.getLastProcedure();

    Code *jmp = new Code(CODE::JMP);
    codeTable.push_back(jmp);

    constDeclare();
    varDeclare();
    procDeclare();

    jmp->setA(getCodeAddress()); // 过程体的目标代码生成后返填过程体的入口地址

    codeTable.emplace_back(CODE::INT, 0, address);

    if (procedure != nullptr)
    {// 不是主函数，那么该过程的起始语句的地址要保存在符号表中
        procedure->setAddress(getCodeAddress() - 1);  //起始语句即上一句INT指令
    }

    statement();

    codeTable.emplace_back(CODE::OPR, 0, OP::RET);

    address = addressTemp;
}

// 〈常量说明部分〉 → CONST〈常量定义〉{ ,〈常量定义〉}；
void parser::constDeclare()
{
    if (SYMBOL::CONST == nowSymbolType)
    {
        advance();     // 跳过 'const'
        constDefine();
        while (SYMBOL::COMMA == nowSymbolType)
        {
            advance(); // 跳过 ','
            constDefine();
        }
        semicolon();
    }
}

// 〈常量定义〉 → 〈标识符〉=〈无符号整数〉
// 〈无符号整数〉 → 〈数字〉{〈数字〉}
void parser::constDefine()
{
    if (SYMBOL::IDENTIFIER != nowSymbolType)
        EXCEPTION::handleException(EXCEPTION::MISSING_IDENTIFIER);  // 异常处理: 缺少标识符
    std::string constName = nowSymbol.getValue();
    if (symbolTable.inTable(constName))
        EXCEPTION::handleException(EXCEPTION::DUPLICATE_IDENTIFIER);// 特判标识符重复声明
    advance(); // 跳过标识符
    if (SYMBOL::EQU != nowSymbolType)
        EXCEPTION::handleException(EXCEPTION::MISSING_EQUAL);// 异常处理: 缺少 '='
    advance(); // 跳过 '='
    if (SYMBOL::NUMBER != nowSymbolType)
        EXCEPTION::handleException(EXCEPTION::MISSING_NUMBER);// 异常处理: 缺少无符号整数
    symbolTable.addSymbol(SYMBOL::CONST, constName, nowSymbol.getNumber(), level, address);
    advance(); // 跳过 无符号整数
}

// 〈变量说明部分〉 → VAR〈标识符〉{ ,〈标识符〉}；
void parser::varDeclare()
{
    if (SYMBOL::VAR == nowSymbolType)
    {
        do
        {
            advance(); // 跳过 'var' or ','
            if (SYMBOL::IDENTIFIER != nowSymbolType)
                EXCEPTION::handleException(EXCEPTION::MISSING_IDENTIFIER);  // 异常处理: 缺少标识符
            std::string varName = nowSymbol.getValue();
            if (symbolTable.inTable(varName))
                EXCEPTION::handleException(EXCEPTION::DUPLICATE_IDENTIFIER);// 特判标识符重复声明
            symbolTable.addSymbol(SYMBOL::VAR, varName, 0, level, address);
            address++;
            advance(); // 跳过 标识符
        } while (SYMBOL::COMMA == nowSymbolType);
        semicolon();
    }
}

// 〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
// 〈过程首部〉 → procedure〈标识符〉；
void parser::procDeclare()
{
    while (SYMBOL::PROCEDURE == nowSymbolType)
    {
        advance(); // 跳过 'procedure'

        if (SYMBOL::IDENTIFIER != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_IDENTIFIER);  // 异常处理: 缺少标识符
        std::string procName = nowSymbol.getValue();
        if (symbolTable.inTable(procName))
            EXCEPTION::handleException(EXCEPTION::DUPLICATE_IDENTIFIER);// 特判标识符重复声明

        symbolTable.addSymbol(SYMBOL::PROCEDURE, procName, 0, level, address);
        address++;
        advance(); // 跳过 标识符

        semicolon();   // 跳过 ';'

        level++;
        // TODO: 为该过程声明一张新的符号表
        subProbgram(); // 分程序
        // TODO: 为该过程销毁新声明的表
        level--;

        semicolon();   // 跳过 ';'
    }
}

// 〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空〉
void parser::statement()
{
    if(assignStatement() ||
       ifStatement()     ||
       whileStatement()  ||
       callStatement()   ||
       writeStatement()  ||
       readStatement()   ||
       beginStatement())
        return;
}

// 〈赋值语句〉 → 〈标识符〉:=〈表达式〉
bool parser::assignStatement()
{
    if (SYMBOL::IDENTIFIER == nowSymbolType)
    {
        std::string varName = nowSymbol.getValue();
        Symbol *symbol = symbolTable.getSymbol(varName);
        if (nullptr == symbol)
            EXCEPTION::handleException(EXCEPTION::MISSING_IDENTIFIER);  // 异常处理: 缺少标识符
        if (SYMBOL::VAR != symbol->getSymbolType())
            EXCEPTION::handleException(EXCEPTION::NOT_A_VAR);           // 并非变量
        advance(); // 跳过 标识符
        if (SYMBOL::CEQU != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_CEQUAL);      // 缺少赋值号
        advance(); // 跳过 ':='
        expression();
        codeTable.emplace_back(CODE::STO, level - symbol->getLevel(), symbol->getAddress());
        return true;
    }
    return false;
}

// 〈条件语句〉 → if〈条件〉then〈语句〉
bool parser::ifStatement()
{
    if (SYMBOL::IF == nowSymbolType)
    {
        advance(); // 跳过 'IF'
        condition();
        if (SYMBOL::THEN != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_THEN);      // 缺少 THEN
        advance();// 跳过 'THEN'
        Code *jpc = new Code(CODE::JPC);// 条件转移代码, 地址回填
        codeTable.push_back(jpc);
        statement();
        jpc->setA(getCodeAddress()); // 回填 地址
        return true;
    }
    return false;
}

// 〈当型循环语句〉 → while〈条件〉do〈语句〉
bool parser::whileStatement()
{
    if (SYMBOL::WHILE == nowSymbolType)
    {
        advance(); // 跳过 'while'
        int codeAddress = getCodeAddress();
        condition();
        if (SYMBOL::DO != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_DO);      // 缺少 DO
        advance(); // 跳过 'do'
        Code *jpc = new Code(CODE::JPC);// 条件转移代码, 地址回填
        codeTable.push_back(jpc);
        statement();
        codeTable.emplace_back(CODE::JMP, 0, codeAddress); // 无条件跳转
        jpc->setA(getCodeAddress());
        return true;
    }
    return false;
}

// 〈过程调用语句〉 → call〈标识符〉
bool parser::callStatement()
{
    if (SYMBOL::CALL == nowSymbolType)
    {
        advance(); // 跳过 'CALL'
        if (SYMBOL::IDENTIFIER != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_IDENTIFIER);  // 异常处理: 缺少标识符
        std::string procName = nowSymbol.getValue();
        Symbol *symbol = symbolTable.getSymbol(procName);
        if (symbol == nullptr)
            EXCEPTION::handleException(EXCEPTION::NOT_A_PROCUDURE);     // 并非状态
        codeTable.emplace_back(CODE::CAL, level - symbol->getLevel(), symbol->getAddress());
        advance(); // 跳过 '标识符'
        return true;
    }
    return false;
}

// 〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
bool parser::readStatement()
{
    if (SYMBOL::READ == nowSymbolType)
    {
        advance(); // 跳过 'read'
        if (SYMBOL::LBR != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_LBR);  // 异常处理: 缺少'('
        do
        {
            advance(); // 跳过 '('、','
            if (SYMBOL::IDENTIFIER != nowSymbolType)
                EXCEPTION::handleException(EXCEPTION::MISSING_IDENTIFIER);  // 异常处理: 缺少标识符
            std::string varName = nowSymbol.getValue();
            Symbol *symbol = symbolTable.getSymbol(varName);
            if (nullptr == symbol || SYMBOL::VAR != symbol->getSymbolType())
                EXCEPTION::handleException(EXCEPTION::NOT_A_VAR);           // 并非变量
            codeTable.emplace_back(CODE::OPR, 0, OP::READ); // 读一个数到栈顶
            codeTable.emplace_back(CODE::STO, level - symbol->getLevel(), symbol->getAddress());// 把栈顶送到变量
            advance(); // 跳过 标识符
        } while (SYMBOL::COMMA == nowSymbolType);
        rightBracket(); // 跳过 ')'
        return true;
    }
    return false;
}

// 〈写语句〉 → write(〈表达式〉{，〈表达式〉})
bool parser::writeStatement()
{
    if (SYMBOL::WRITE == nowSymbolType)
    {
        advance(); // 跳过 'write'
        if (SYMBOL::LBR != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_LBR);  // 异常处理: 缺少'('
        do
        {
            advance(); // 跳过 '('、','
            expression();
            codeTable.emplace_back(CODE::OPR, 0, OP::WRITE); // 栈顶 输出到屏幕
        } while (SYMBOL::COMMA == nowSymbolType);
        codeTable.emplace_back(CODE::OPR, 0, OP::LINE); // 输出 换行
        rightBracket(); // 跳过 ')'
        return true;
    }
    return false;
}

// 〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
bool parser::beginStatement()
{
    if (SYMBOL::BEGIN == nowSymbolType)
    {
        do
        {
            advance(); // 跳过 'begin' 或 ';'
            statement();
        } while (SYMBOL::COMMA == nowSymbolType);
        if (SYMBOL::END != nowSymbolType)
            EXCEPTION::handleException(EXCEPTION::MISSING_END);  // 异常处理: 缺少'('
        advance(); // 跳过 'end'
        return true;
    }
    return false;
}

// 〈分号〉
void parser::semicolon()
{
    if (SYMBOL::SEMIC != nowSymbolType)
        EXCEPTION::handleException(EXCEPTION::MISSING_SEMIC);  // 异常处理: 缺少';'
    advance(); // 跳过 ';'
}

void parser::rightBracket()
{
    if (SYMBOL::RBR != nowSymbolType)
        EXCEPTION::handleException(EXCEPTION::MISSING_RBR);  // 异常处理: 缺少';'
    advance(); // 跳过 ')'
}

// 〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
// 〈加减运算符〉 → +|-
void parser::expression()
{
    SymbolType op = nowSymbolType;
    if (SYMBOL::ADD == nowSymbolType || SYMBOL::SUB == nowSymbolType)
        advance(); // 跳过 '+' / '-'
    term();
    if (SYMBOL::SUB == op)
        codeTable.emplace_back(CODE::OPR, 0, OP::NEG); // 栈顶 取负
    while (SYMBOL::ADD == nowSymbolType || SYMBOL::SUB == nowSymbolType)
    {
        op = nowSymbolType;
        advance(); // 跳过 '+' / '-'
        term();
        if (SYMBOL::ADD == op)
            codeTable.emplace_back(CODE::OPR, 0, OP::ADD); // 栈顶两个元素弹出相加结果进栈
        else // SUB
            codeTable.emplace_back(CODE::OPR, 0, OP::SUB); // 栈顶两个元素弹出，次栈顶减去栈顶结果进栈
    }
}

// 〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
// 〈乘除运算符〉 → *|/
void parser::term()
{
    factor();
    while (SYMBOL::MUL == nowSymbolType || SYMBOL::DIV == nowSymbolType)
    {
        SymbolType op = nowSymbolType;
        advance(); // 跳过 '*' / '/'
        factor();
        if (SYMBOL::MUL == op)
            codeTable.emplace_back(CODE::OPR, 0, OP::MUL); // 栈顶两个元素弹出相乘结果进栈
        else
            codeTable.emplace_back(CODE::OPR, 0, OP::DIV); // 栈顶两个元素弹出，次栈顶除以栈顶结果进栈
    }
}

// 〈因子〉 → 〈标识符〉|〈无符号整数〉|(〈表达式〉)
void parser::factor()
{
    if (SYMBOL::IDENTIFIER == nowSymbolType)
    {
        std::string name = nowSymbol.getValue();
        advance();// 跳过标识符
        Symbol* symbol = symbolTable.getSymbol(name);
        if (nullptr != symbol)
            EXCEPTION::handleException(EXCEPTION::NEVER_DECLARE);  // 异常处理: 未声明
        if (SYMBOL::CONST == symbol->getSymbolType())
            codeTable.emplace_back(CODE::LIT, 0, symbol->getNumber()); // 常量放入栈顶
        else if (SYMBOL::VAR == symbol->getSymbolType())
            codeTable.emplace_back(CODE::LOD, level - symbol->getLevel(), symbol->getAddress()); // 常量放入栈顶
    } else if (SYMBOL::NUMBER == nowSymbolType)
    {
        codeTable.emplace_back(CODE::LIT, 0, nowSymbol.getNumber());
        advance(); // 跳过 '数字'
    } else if (SYMBOL::LBR == nowSymbolType)
    {
        advance(); // 跳过 '('
        expression();
        rightBracket();
    }
}

// 〈条件〉 → 〈表达式〉〈关系运算符〉〈表达式〉| odd〈表达式〉
// 〈关系运算符〉 → =|#|<|<=|>|>=
void parser::condition()
{
    if (SYMBOL::ODD == nowSymbolType)
    {
        expression();
    }else
    {
        expression();
        if(SYMBOL::EQU == nowSymbolType)
            codeTable.emplace_back(CODE::OPR, 0, OP::EQU);
        else if(SYMBOL::NEQ == nowSymbolType)
            codeTable.emplace_back(CODE::OPR, 0, OP::NEQ);
        else if(SYMBOL::LS == nowSymbolType)
            codeTable.emplace_back(CODE::OPR, 0, OP::LS);
        else if(SYMBOL::LEQ == nowSymbolType)
            codeTable.emplace_back(CODE::OPR, 0, OP::LEQ);
        else if(SYMBOL::GT == nowSymbolType)
            codeTable.emplace_back(CODE::OPR, 0, OP::GT);
        else if(SYMBOL::GEQ == nowSymbolType)
            codeTable.emplace_back(CODE::OPR, 0, OP::GEQ);
    }
}


