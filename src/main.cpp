#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "vm.h"

int main(int argc, char **argv)
{
    std::ifstream fin(argv[1]);
    Lexer lexer(fin);
    Parser parser(lexer);
    parser.program();
    parser.printTables();
    parser.syntaxTree.print();
    parser.printCode();
    VM vm(parser.codeTable);
    vm.run();
    return 0;
}