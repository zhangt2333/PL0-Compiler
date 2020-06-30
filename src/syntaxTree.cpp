/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * syntaxTree.cpp 2020/5/13
 * Comments: 
 */
#include "syntaxTree.h"

#include <iostream>


void SyntaxTree::begin(const std::string &name)
{
    SyntaxTreeNode *syntaxTreeNode = new SyntaxTreeNode(name);
    if (syntaxTreeStack.empty())
        this->root = syntaxTreeNode;
    else
        syntaxTreeStack.back()->children.push_back(syntaxTreeNode);
    syntaxTreeStack.push_back(syntaxTreeNode);
}

void SyntaxTree::end()
{
    if (syntaxTreeStack.back()->children.empty())
    {
        syntaxTreeStack.pop_back();
        syntaxTreeStack.back()->children.pop_back();
    } else
    {
        syntaxTreeStack.pop_back();
    }
}

void SyntaxTree::merge(Symbol *symbol)
{
    syntaxTreeStack.back()->children.push_back(new SyntaxTreeNode(symbol->getValue()));
}

void SyntaxTree::merge(std::string name)
{
    syntaxTreeStack.back()->children.push_back(new SyntaxTreeNode(std::move(name)));
}

void SyntaxTree::print()
{
    std::cout << root->name << std::endl;
    int n = root->children.size();
    for (auto x : root->children)
        print(x, "", --n==0);
}

void SyntaxTree::print(SyntaxTreeNode *now, const std::string &prefix, bool isLast)
{
    std::cout << prefix;
    std::cout << (isLast ? "|__" : "|--");
    std::cout << now->name << std::endl;

    int n = now->children.size();
    for (auto x : now->children)
        print(x, prefix + (isLast ? "   " : "|   "), --n == 0);
}
