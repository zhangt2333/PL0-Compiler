#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
    std::ifstream fin(argv[1]);
    Lexer lexer(fin);
    Parser parser(lexer);
    parser.program();
    parser.printTables();
    parser.syntaxTree.print();
    return 0;
}