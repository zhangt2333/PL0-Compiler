# PL0-Compiler

------

PL0 Compiler, which is my experiment on *Compilation Principle*.

Run Envirment:

*   IDE：CLion 2020.1 x64
*   Build Tool：CMake 3.15.3
*   C++ Compiler: MinGW 4.3.5
*   C++ standard: C++ 14

Project Tree:

```
.
├── CMakeLists.txt
├── README.md
├── report.pdf             // experiment report
├── src                    // source code
│   ├── code.h
│   ├── exceptionHandler.h
│   ├── lexer.cpp
│   ├── lexer.h
│   ├── main.cpp
│   ├── parser.cpp
│   ├── parser.h
│   ├── symbol.h
│   ├── symbolTable.cpp
│   ├── symbolTable.h
│   ├── symbolType.h
│   ├── syntaxTree.cpp
│   ├── syntaxTree.h
│   ├── utils.cpp
│   ├── utils.h
│   ├── vm.cpp
│   └── vm.h
└── test                    // test cases
    ├── PL0_code0.in
    ├── PL0_code1.in
    ├── PL0_code2.in
    ├── PL0_code3.in
    ├── PL0_code4.in
    ├── PL0_code5.in
    └── PL0_code6.in
```

For the task details: [Task Details in Chinese](https://github.com/zhangt2333/PL0-Compiler/taskDetails.md)

For the project details: [MyReport in Chinese](https://github.com/zhangt2333/PL0-Compiler/report.pdf)