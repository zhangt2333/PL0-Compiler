/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * parser.cpp 2020/4/19
 * Comments: 
 */
#define DEBUG    // 调试模式


#include "parser.h"
#include "exceptionHandler.h"
#include "utils.h"


Parser::Parser(Lexer &lexer) : lexer(lexer)
{
    advance();
}

// 取下一个 词
void Parser::advance()
{
    if (SYMBOL::ILLEGAL != nowSymbolType)
    {
        if (!nowSymbol.getValue().empty())
            syntaxTree.merge(nowSymbol.getValue());
        else if(-1 != nowSymbol.getNumber())
            syntaxTree.merge(std::to_string(nowSymbol.getNumber()));
        else
            syntaxTree.merge(nowSymbolType.name);
    }
    nowSymbol = lexer.getSymbol();
    nowSymbolType = nowSymbol.getSymbolType();
    logs("LOG[Parser: advance] ", nowSymbol);
}

// 每个过程的相对起始位置在 BLOCK 内置初值 DX=3
void Parser::addressReset()
{
    address = 3;
}

int Parser::getCodeAddress()
{
    return codeTable.size();
}

// 〈程序〉→〈分程序〉.
void Parser::program()
{
    logs("LOG[Parser: program]");
    syntaxTree.begin("[program]");

    /* 语义分析 */
    subProbgram();                      // 〈分程序〉
    ASSERT(SYMBOL::POINT == nowSymbolType, EXCEPTION::MISSING_SEMICOLON); // 异常处理: 缺少 '.'
    advance();                          // 跳过 '.'
    ASSERT(lexer.isEOF(), EXCEPTION::EXTRA_CHARACTERS); // 异常处理: 文件尾 '.' 后还有字符

    syntaxTree.end();
}

// 〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
void Parser::subProbgram()
{
    logs("LOG[Parser: subProbgram]");
    syntaxTree.begin("[subProbgram]");

    /* 中间代码生成 */
    int addressTemp = address; // 保存 address
    addressReset();
    Symbol *procedure = symbolTable.getLastProcedure(); // 得到当前〈分程序〉所在的过程 Proc
    Code *jmp = new Code(CODE::JMP);
    codeTable.push_back(jmp);

    /* 语义分析 */
    constDeclare(); // [〈常量说明部分〉]
    varDeclare();   // [〈变量说明部分〉]
    procDeclare();  // [〈过程说明部分〉]

    /* 中间代码生成 */
    jmp->setA(getCodeAddress());                            // 过程体的目标代码生成后, 返填过程体的入口地址
    codeTable.push_back(new Code(CODE::INT, address));      // 申请栈空间，大小即 address=3+常量声明+变量声明
    if (procedure != nullptr)                                  // 不是主函数，那该过程的起始语句的地址要保存在符号表
        procedure->setAddress(getCodeAddress() - 1);   // 起始语句即上一句INT指令

    /* 语义分析 */
    statement();   // 〈语句〉

    /* 中间代码生成 */
    codeTable.push_back(new Code(CODE::OPR, OP::RET)); // 过程结束, 返回调用点
    address = addressTemp;

    syntaxTree.end();
}

// 〈常量说明部分〉 → const〈常量定义〉{ ,〈常量定义〉}；
void Parser::constDeclare()
{
    logs("LOG[Parser: constDeclare]");
    syntaxTree.begin("[constDeclare]");

    if (SYMBOL::CONST == nowSymbolType)
    { // const
        advance();     // 跳过 'const'
        constDefine();
        while (SYMBOL::COMMA == nowSymbolType)
        {
            advance(); // 跳过 ','
            constDefine();
        }
        semicolon();
    }

    syntaxTree.end();
}

// 〈常量定义〉 → 〈标识符〉=〈无符号整数〉
// 〈无符号整数〉 → 〈数字〉{〈数字〉}
void Parser::constDefine()
{
    logs("LOG[Parser: constDefine]");
    syntaxTree.begin("[constDefine]");

    ASSERT(SYMBOL::IDENTIFIER == nowSymbolType, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
    std::string constName = nowSymbol.getValue();
    ASSERT(!symbolTable.inTable(constName), EXCEPTION::DUPLICATE_IDENTIFIER); // 特判标识符重复声明
    advance();                               // 跳过标识符
    ASSERT(SYMBOL::EQU == nowSymbolType, EXCEPTION::MISSING_EQUAL); // 异常处理: 缺少 '='
    advance();                               // 跳过 '='
    ASSERT(SYMBOL::NUMBER == nowSymbolType, EXCEPTION::MISSING_NUMBER); // 异常处理: 缺少无符号整数
    symbolTable.addSymbol(SYMBOL::CONST, constName, nowSymbol.getNumber(), level, address);
    advance();                               // 跳过 无符号整数

    syntaxTree.end();
}

// 〈变量说明部分〉 → var〈标识符〉{ ,〈标识符〉}；
void Parser::varDeclare()
{
    logs("LOG[Parser: varDeclare]");
    syntaxTree.begin("[varDeclare]");

    if (SYMBOL::VAR == nowSymbolType)
    {
        do
        {
            advance();                                  // 跳过 'var' or ','
            ASSERT(SYMBOL::IDENTIFIER == nowSymbolType, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
            std::string varName = nowSymbol.getValue();
            ASSERT(!symbolTable.inTable(varName), EXCEPTION::DUPLICATE_IDENTIFIER); // 特判标识符重复声明
            symbolTable.addSymbol(SYMBOL::VAR, varName, 0, level, address);
            address++;                                  // 地址++
            advance();                                  // 跳过 标识符
        } while (SYMBOL::COMMA == nowSymbolType);
        semicolon();                                    // 跳过 ';'
    }

    syntaxTree.end();
}

// 〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
// 〈过程首部〉 → procedure〈标识符〉；
void Parser::procDeclare()
{
    logs("LOG[Parser: procDeclare]");
    syntaxTree.begin("[procDeclare]");

    while (SYMBOL::PROCEDURE == nowSymbolType)
    {
        advance();                               // 跳过 'procedure'

        ASSERT(SYMBOL::IDENTIFIER == nowSymbolType, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
        std::string procName = nowSymbol.getValue();
        ASSERT(!symbolTable.inTable(procName), EXCEPTION::DUPLICATE_IDENTIFIER);    // 特判标识符重复声明

        symbolTable.addSymbol(SYMBOL::PROCEDURE, procName, 0, level, address);
        address++;
        advance();                               // 跳过 标识符
        semicolon();                             // 跳过 ';'
        /* 进入〈分程序〉*/
        level++;
        symbolTable.pushTable();
        subProbgram(); // 分程序
        symbolTable.popTable();
        level--;
        semicolon();                            // 跳过 ';'
    }

    syntaxTree.end();
}

// 〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空〉
void Parser::statement()
{
    logs("LOG[Parser: statement]");
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
bool Parser::assignStatement()
{
    logs("LOG[Parser: assignStatement]");
    if (SYMBOL::IDENTIFIER == nowSymbolType)
    {
        syntaxTree.begin("[assignStatement]");
        std::string varName = nowSymbol.getValue();
        Symbol *symbol = symbolTable.getSymbol(varName);
        ASSERT(nullptr != symbol, EXCEPTION::MISSING_IDENTIFIER);          // 异常处理: 缺少标识符
        ASSERT(SYMBOL::VAR == symbol->getSymbolType(), EXCEPTION::NOT_A_VAR); // 异常处理: 并非变量
        advance();                                   // 跳过 标识符
        ASSERT(SYMBOL::CEQU == nowSymbolType, EXCEPTION::MISSING_CEQUAL); // 异常处理: 缺少赋值号
        advance();                                   // 跳过 ':='
        expression();
        /* 中间代码生成 */
        codeTable.push_back(new Code(CODE::STO, level - symbol->getLevel(), symbol->getAddress())); // 赋值
        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈条件语句〉 → if〈条件〉then〈语句〉
bool Parser::ifStatement()
{
    logs("LOG[Parser: ifStatement]");
    if (SYMBOL::IF == nowSymbolType)
    {
        syntaxTree.begin("[ifStatement]");

        /* 语义分析 */
        advance();                         // 跳过 'IF'
        condition();
        ASSERT(SYMBOL::THEN == nowSymbolType, EXCEPTION::MISSING_THEN); // 缺少 THEN
        advance();                         // 跳过 'THEN'

        /* 中间代码生成 */
        Code *jpc = new Code(CODE::JPC);// 条件转移代码, 地址回填
        codeTable.push_back(jpc);

        /* 语义分析 */
        statement();

        /* 中间代码生成 */
        jpc->setA(getCodeAddress());   // 回填地址

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈当型循环语句〉 → while〈条件〉do〈语句〉
bool Parser::whileStatement()
{
    logs("LOG[Parser: whileStatement]");
    if (SYMBOL::WHILE == nowSymbolType)
    {
        syntaxTree.begin("[whileStatement]");

        /* 语义分析 */
        advance();                          // 跳过 'while'

        /* 中间代码生成 */
        int whileBeginAddress = getCodeAddress();

        /* 语义分析 */
        condition();
        ASSERT(SYMBOL::DO == nowSymbolType, EXCEPTION::MISSING_DO); // 缺少 DO
        advance(); // 跳过 'do'

        /* 中间代码生成 */
        Code *jpc = new Code(CODE::JPC);// 条件转移代码, 地址回填
        codeTable.push_back(jpc);

        /* 语义分析 */
        statement();
        codeTable.push_back(new Code(CODE::JMP, whileBeginAddress)); // 无条件跳转
        jpc->setA(getCodeAddress());

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈过程调用语句〉 → call〈标识符〉
bool Parser::callStatement()
{
    logs("LOG[Parser: callStatement]");
    if (SYMBOL::CALL == nowSymbolType)
    {
        syntaxTree.begin("[callStatement]");

        /* 语义分析 */
        advance();                               // 跳过 'CALL'
        ASSERT(SYMBOL::IDENTIFIER == nowSymbolType, EXCEPTION::MISSING_IDENTIFIER);// 异常处理: 缺少标识符
        std::string procName = nowSymbol.getValue();
        Symbol *symbol = symbolTable.getSymbol(procName);
        ASSERT(nullptr != symbol, EXCEPTION::NOT_A_PROCUDURE); // 并非过程
        advance();                               // 跳过 '标识符'

        /* 中间代码生成 */
        codeTable.push_back(new Code(CODE::CAL, level - symbol->getLevel(), symbol->getAddress()));

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
bool Parser::readStatement()
{
    logs("LOG[Parser: readStatement]");
    if (SYMBOL::READ == nowSymbolType)
    {
        syntaxTree.begin("[readStatement]");

        /* 语义分析 */
        advance();                                   // 跳过 'read'
        ASSERT(SYMBOL::LBR == nowSymbolType, EXCEPTION::MISSING_LBR); // 异常处理: 缺少'('
        do
        {
            advance();                               // 跳过 '('、','
            ASSERT(SYMBOL::IDENTIFIER == nowSymbolType, EXCEPTION::MISSING_IDENTIFIER); // 异常处理: 缺少标识符
            std::string varName = nowSymbol.getValue();
            Symbol *symbol = symbolTable.getSymbol(varName);
            ASSERT(nullptr != symbol && SYMBOL::VAR == symbol->getSymbolType(), EXCEPTION::NOT_A_VAR); // 并非变量
        /* 中间代码生成 */
            codeTable.push_back(new Code(CODE::OPR, OP::READ));                                  // 读一个数到栈顶
            codeTable.push_back(new Code(CODE::STO, level - symbol->getLevel(), symbol->getAddress()));// 把栈顶送到变量
        /* 语义分析 */
            advance(); // 跳过 标识符
        } while (SYMBOL::COMMA == nowSymbolType);
        rightBracket(); // 跳过 ')'

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈写语句〉 → write(〈表达式〉{，〈表达式〉})
bool Parser::writeStatement()
{
    logs("LOG[Parser: writeStatement]");
    if (SYMBOL::WRITE == nowSymbolType)
    {
        syntaxTree.begin("[writeStatement]");

        /* 语义分析 */
        advance();                       // 跳过 'write'
        ASSERT(SYMBOL::LBR == nowSymbolType, EXCEPTION::MISSING_LBR); // 异常处理: 缺少'('
        do
        {
            advance(); // 跳过 '('、','
            expression();
        /* 中间代码生成 */
            codeTable.push_back(new Code(CODE::OPR,  OP::WRITE)); // 栈顶 输出到屏幕
        } while (SYMBOL::COMMA == nowSymbolType);
        codeTable.push_back(new Code(CODE::OPR, OP::LINE)); // 输出 换行
        /* 语义分析 */
        rightBracket(); // 跳过 ')'

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
bool Parser::beginStatement()
{
    logs("LOG[Parser: beginStatement]");
    if (SYMBOL::BEGIN == nowSymbolType)
    {
        syntaxTree.begin("[beginStatement]");

        do
        {
            advance();                    // 跳过 'begin' 或 ';'
            statement();
        } while (SYMBOL::SEMIC == nowSymbolType);
        ASSERT(SYMBOL::END == nowSymbolType, EXCEPTION::MISSING_END); // 异常处理: 缺少'end'
        advance();                        // 跳过 'end'

        syntaxTree.end();
        return true;
    }
    return false;
}

// 〈分号〉
void Parser::semicolon()
{
    logs("LOG[Parser: semicolon]");
    ASSERT(SYMBOL::SEMIC == nowSymbolType, EXCEPTION::MISSING_SEMIC); // 异常处理: 缺少';'
    advance();                          // 跳过 ';'
}

void Parser::rightBracket()
{
    logs("LOG[Parser: rightBracket]");
    ASSERT(SYMBOL::RBR == nowSymbolType, EXCEPTION::MISSING_RBR); // 异常处理: 缺少';'
    advance();                         // 跳过 ')'
}

// 〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
// 〈加减运算符〉 → +|-
void Parser::expression()
{
    logs("LOG[Parser: expression]");
    syntaxTree.begin("[expression]");

    SymbolType op = nowSymbolType;
    if (SYMBOL::ADD == nowSymbolType || SYMBOL::SUB == nowSymbolType)
        advance();         // 跳过 '+' / '-'
    term();
    if (SYMBOL::SUB == op) // 栈顶 取负
        codeTable.push_back(new Code(CODE::OPR,  OP::NEG));
    while (SYMBOL::ADD == nowSymbolType || SYMBOL::SUB == nowSymbolType)
    {
        op = nowSymbolType;
        advance();             // 跳过 '+' / '-'
        term();
        if (SYMBOL::ADD == op) // 栈顶两个元素弹出相加结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::ADD));
        else                   // 栈顶两个元素弹出，次栈顶减去栈顶结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::SUB));
    }

    syntaxTree.end();
}

// 〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
// 〈乘除运算符〉 → *|/
void Parser::term()
{
    logs("LOG[Parser: term]");
    syntaxTree.begin("[term]");

    factor();
    while (SYMBOL::MUL == nowSymbolType || SYMBOL::DIV == nowSymbolType)
    {
        SymbolType op = nowSymbolType;
        advance(); // 跳过 '*' / '/'
        factor();
        if (SYMBOL::MUL == op) // 栈顶两个元素弹出相乘结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::MUL));
        else                   // 栈顶两个元素弹出，次栈顶除以栈顶结果进栈
            codeTable.push_back(new Code(CODE::OPR, OP::DIV));
    }

    syntaxTree.end();
}

// 〈因子〉 → 〈标识符〉|〈无符号整数〉| (〈表达式〉)
void Parser::factor()
{
    logs("LOG[Parser: factor]");
    syntaxTree.begin("[factor]");

    if (SYMBOL::IDENTIFIER == nowSymbolType)
    {
        std::string name = nowSymbol.getValue();
        advance();// 跳过标识符
        Symbol* symbol = symbolTable.getSymbol(name);
        ASSERT(nullptr != symbol, EXCEPTION::NEVER_DECLARE);  // 异常处理: 未声明
        if (SYMBOL::CONST == symbol->getSymbolType())
            codeTable.push_back(new Code(CODE::LIT, symbol->getNumber()));        // 常量放入栈顶
        else if (SYMBOL::VAR == symbol->getSymbolType())
            codeTable.push_back(new Code(CODE::LOD, level - symbol->getLevel(), symbol->getAddress())); // 变量放入栈顶
    } else if (SYMBOL::NUMBER == nowSymbolType)
    {
        codeTable.push_back(new Code(CODE::LIT, nowSymbol.getNumber()));      // 常量放入栈顶
        advance(); // 跳过 '数字'
    } else if (SYMBOL::LBR == nowSymbolType)
    {
        advance(); // 跳过 '('
        expression();
        rightBracket();
    }

    syntaxTree.end();
}

// 〈条件〉 → 〈表达式〉〈关系运算符〉〈表达式〉| odd〈表达式〉
// 〈关系运算符〉 → =|#|<|<=|>|>=
void Parser::condition()
{
    logs("LOG[Parser: condition]");
    syntaxTree.begin("[condition]");

    if (SYMBOL::ODD == nowSymbolType)
    {
        advance(); // 跳过 'odd'
        expression();
        codeTable.push_back(new Code(CODE::OPR, OP::LEQ)); // TODO: 奇偶判断
    }else
    {
        expression();
        SymbolType op = nowSymbolType; // 保存关系算符
        advance();                     // 跳过关系运算符
        expression();
        if(SYMBOL::EQU == op)
            codeTable.push_back(new Code(CODE::OPR, OP::EQU));
        else if(SYMBOL::NEQ == op)
            codeTable.push_back(new Code(CODE::OPR, OP::NEQ));
        else if(SYMBOL::LS == op)
            codeTable.push_back(new Code(CODE::OPR, OP::LS));
        else if(SYMBOL::LEQ == op)
            codeTable.push_back(new Code(CODE::OPR, OP::LEQ));
        else if(SYMBOL::GT == op)
            codeTable.push_back(new Code(CODE::OPR, OP::GT));
        else if(SYMBOL::GEQ == op)
            codeTable.push_back(new Code(CODE::OPR, OP::GEQ));
    }

    syntaxTree.end();
}

void Parser::printCode()
{
    int index = 0;
    for (auto x: codeTable)
        std::cout << index++ << '\t' << (*x) << std::endl;
}


