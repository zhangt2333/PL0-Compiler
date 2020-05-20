/* 
 * Copyright 2020 ZhangT. All Rights Reserved.
 * Author: ZhangT
 * Author-Github: github.com/zhangt2333
 * syntaxTree.h 2020/5/13
 * Comments: 
 */
#ifndef PL0_COMPILER_SYNTAXTREE_H
#define PL0_COMPILER_SYNTAXTREE_H

#include <list>
#include <utility>
#include "symbol.h"

class SyntaxTreeNode
{
public:
    std::string name;
    std::list<SyntaxTreeNode*> children;
    explicit SyntaxTreeNode(std::string name): name(std::move(name)) {}
};

class SyntaxTree
{
    std::list<SyntaxTreeNode*> syntaxTreeStack;  // 语法树 栈
    SyntaxTreeNode* root;                        // 语法树 根
public:
    void merge(Symbol* symbol);
    void merge(std::string name);
    void begin(const std::string &name);
    void end();
    void print();
    void print(SyntaxTreeNode* now, const std::string& prefix, bool isLast);
};



#endif //PL0_COMPILER_SYNTAXTREE_H
