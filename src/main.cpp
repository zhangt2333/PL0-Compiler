#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "vm.h"

/* 词法分析测试代码 */
//int main(int argc, char **argv)
//{
//    std::ifstream fin(argv[1]);
//    Lexer lexer(fin);
//    while (!lexer.isEOF())
//    {
//        Symbol symbol = lexer.getSymbol();
//        std::cout << symbol << ' ';
//        if (SYMBOL::SEMIC == symbol.getSymbolType())
//            std::cout << std::endl;
//    }
//    return 0;
//}

/* 语法分析测试代码 */
//int main(int argc, char **argv)
//{
//    std::ifstream fin(argv[1]);
//    Lexer lexer(fin);
//    Parser parser(lexer);
//    parser.program();
//    parser.printTables();
//    return 0;
//}

/* 语法树生成测试代码 */
//int main(int argc, char **argv)
//{
//    std::ifstream fin(argv[1]);
//    Lexer lexer(fin);
//    Parser parser(lexer);
//    parser.program();
//    parser.syntaxTree.print();
//    return 0;
//}

/* 目标代码生成测试代码 */
//int main(int argc, char **argv)
//{
//    std::ifstream fin(argv[1]);
//    Lexer lexer(fin);
//    Parser parser(lexer);
//    parser.program();
//    parser.printCode();
//    return 0;
//}

/* 解释执行测试代码 */
int main(int argc, char **argv)
{
    std::ifstream fin(argv[1]);
    Lexer lexer(fin);
    Parser parser(lexer);
    parser.program();
    VM vm(parser.codeTable);
    vm.run();
    return 0;
}