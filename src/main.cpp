#include <iostream>
#include <fstream>
#include "lexer.h"

int main(int argc, char **argv)
{
    std::ifstream fin(argv[1]);
    Lexer lexer(fin);
    while(!lexer.isEOF())
    {
        Symbol symbol = lexer.getSymbol();
        std::cout << symbol << std::endl;
    }
    return 0;
}