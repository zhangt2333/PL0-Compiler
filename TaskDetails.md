本文分两部分：

*   原封不动の任务描述
*   我的分析与实现



# 任务描述

### PL/0 语言文法的 BNF 表示

```
〈程序〉→〈分程序〉。
〈分程序〉→ [〈常量说明部分〉][〈变量说明部分〉][〈过程说明部分〉]〈语句〉
〈常量说明部分〉 → CONST〈常量定义〉{ ,〈常量定义〉}；
〈常量定义〉 → 〈标识符〉=〈无符号整数〉
〈无符号整数〉 → 〈数字〉{〈数字〉}
〈变量说明部分〉 → VAR〈标识符〉{ ,〈标识符〉}；
〈标识符〉 → 〈字母〉{〈字母〉|〈数字〉}
〈过程说明部分〉 → 〈过程首部〉〈分程序〉；{〈过程说明部分〉}
〈过程首部〉 → procedure〈标识符〉；
〈语句〉 → 〈赋值语句〉|〈条件语句〉|〈当型循环语句〉|〈过程调用语句〉|〈读语句〉|〈写语句〉|〈复合语句〉|〈空〉
〈赋值语句〉 → 〈标识符〉:=〈表达式〉
〈复合语句〉 → begin〈语句〉{ ；〈语句〉}〈end〉
〈条件〉 → 〈表达式〉〈关系运算符〉〈表达式〉|odd〈表达式〉
〈表达式〉 → [+|-]〈项〉{〈加减运算符〉〈项〉}
〈项〉 → 〈因子〉{〈乘除运算符〉〈因子〉}
〈因子〉 → 〈标识符〉|〈无符号整数〉|(〈表达式〉)
〈加减运符〉 → +|-
〈乘除运算符〉 → *|/
〈关系运算符〉 → =|#|<|<=|>|>=
〈条件语句〉 → if〈条件〉then〈语句〉
〈过程调用语句〉 → call〈标识符〉
〈当型循环语句〉 → while〈条件〉do〈语句〉
〈读语句〉 → read(〈标识符〉{ ，〈标识符〉})
〈写语句〉 → write(〈标识符〉{，〈标识符〉})
〈字母〉 → a|b|c…x|y|z
〈数字〉 → 0|1|2…7|8|9
```

### 一、PL/0 语言建立一个词法分析程序 GETSYM（函数）

把关键字、算符、界符称为语言固有的单词，标识符、常量称为用户自定义的单词。

为此设置三个全程量：SYM, ID, NUM 

*   SYM：存放每个单词的类别，为内部编码的表示形式。
*   ID：存放用户所定义的标识符的值，即标识符字符串的机内表示。
*   NUM：存放用户定义的数。

GETSYM 要完成的任务：

1.	滤掉单词间的空格。
2.	识别关键字，用查关键字表的方法识别。当单词是关键字时，将对应的类别放在 SYM 中。如 IF 的类别为 IFSYM，THEN 的类别为 THENSYM。
3.	识别标识符，标识符的类别为 IDENT，IDENT 放在 SYM 中，标识符本身的值放在 ID 中。关键字或标识符的最大长度是 10。
4.	拼数，将数的类别 NUMBER 放在SYM中，数本身的值放在 NUM 中。
5.	拼由两个字符组成的运算符，如：>=、<= 等等，识别后将类别存放在 SYM 中。
6.	打印源程序，边读入字符边打印。（由于一个单词是由一个或多个字符组成的，所以在词法分析程序 GETSYM 中定义一个读字符过程 GETCH）

### 二、PL/0 语言建立一个语法分析程序 BLOCK（函数）

PL/0 编译程序采用一遍扫描的方法，所以语法分析和代码生成都在 BLOCK 中完成。BLOCK的工作分为两步：

*   说明部分的处理

   *   说明部分的处理任务就是对每个过程（包括主程序，可以看成是一个主过程）的说明对象造名字表。填写所在层次（主程序是 0 层，在主程序中定义的过程是 1 层，随着嵌套的深度增加而层次数增大。PL/0 最多允许 3 层），标识符的属性和分配的相对地址等。标识符的属性不同则填写的信息不同。

   *   所造的表放在全程量一维数组 TABLE 中，TX 为指针，数组元素为结构体类型数据。LEV 给出层次，DX 给出每层的局部量的相对地址，每说明完一个变量后 DX 加 1。

   *   例如：一个过程的说明部分为：

       ```PL0
       const a=35,b=49;
       var c,d,e;
       procedure p;
       var g;
       ```
       
   *   对它的常量、变量和过程说明处理后，TABLE 表中的信息如下：

   | TX0->                                                        |                                                              |                                                              |                                                            |
   | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ---------------------------------------------------------- |
   | NAME:  a<br/>NAME:  b<br/>NAME:  c <br/>NAME:  d<br/>NAME:  e <br/>NAME:  p | KIND:  CONSTANT<br/>KIND:  CONSTANT<br/>KIND:  VARIABLE<br/>KIND:  VARIABLE<br/>KIND:  VARIABLE<br/>KIND:  PROCEDURE | VAL:  35<br/>VAL:  49<br/>LEVEL:  LEV<br/>LEVEL:  LEV<br/>LEVEL:  LEV<br/>LEVEL:  LEV | <br/><br/>ADR:  DX <br/>ADR:  DX+1<br/>ADR:  DX+2<br/>ADR: |

   | TX1->             |                         |                         |                    |
   | ----------------- | ----------------------- | ----------------------- | ------------------ |
   | NAME:  g <br/>... | KIND:  VARIABLE<br/>... | LEVEL:  LEV+1  <br/>... | ADR:  DX <br/> ... |

    *   对于过程名的 ADR 域，是在过程体的目标代码生成后返填过程体的入口地址。
    *   TABLE 表的索引 TX 和层次单元 LEV 都是以 BLOCK 的参数形式出现，在主程序调用 BLOCK 时实参的值为 0。每个过程的相对起始位置在 BLOCK 内置初值 DX=3。

*   语句处理和代码生成

    *    对语句逐句分析，语法正确则生目标代码，当遇到标识符的引用则去查 TABLE 表，看是否有过正确的定义，若有则从表中取出相关的信息，供代码生成用。PL/0 语言的代码生成是由过程 GEN 完成。GEN 过程有三个参数，分别代表目标代码的功能码、层差、和位移量。生成的目标代码放在数组 CODE 中。CODE 是一维数组，数组元素是结构体类型数据。

    *   PL/0 语言的目标指令是一种假想的栈式计算机的汇编语言，其格式如下： 
        `[f, l, a]`
        
    *   其中 f 代表功能码，l 代表层次差，a 代表位移量。目标指令有 8 条：
    
        ① LIT：将常数放到运栈顶，a 域为常数。
    
        ② LOD：将变量放到栈顶。a 域为变量在所说明层中的相对位置，l 为调用层与说明层的层差值。
    
        ③ STO：将栈顶的内容送到某变量单元中。a,l 域的含义与 LOD 的相同。
    
        ④ CAL：调用过程的指令。a 为被调用过程的目标程序的入口地址，l 为层差。
    
        ⑤ INT：为被调用的过程（或主程序）在运行栈中开辟数据区。a 域为开辟的个数。
    
        ⑥ JMP：无条件转移指令，a 为转向地址。
    
        ⑦ JPC：条件转移指令，当栈顶的布尔值为非真时，转向 a 域的地址，否则顺序执行。
    
        ⑧ OPR：关系和算术运算。具体操作由 a 域给出。运算对象为栈顶和次顶的内容进行运算，结果存放在次顶。a 域为 0 时是退出数据区。



# 分析与实现

### 一、词法分析程序 getSymbol

枚举类的设计：

*   【关键字、算符、界符】是语言固有单词，【标识符、常量】是用户自定义单词
*   设计一个枚举类承载它们，但是 C++ 的枚举类没法像 Java 一样设计多值组和方法。
*   所以设计一个类 SymbolType 用于承载常量，设计一个类 Symbol 来承载具体的 Symbol 及其值

词法分析器类的设计：

*   采用面向对象的思想去设计整个实验，那么可以设计一个类 Lexer，用于做词法分析
*   主要功能是识别 Symbol，成员变量需要一个输入流 istream 的实现类实例
*   完成的任务是：
    1.  识别 关键字
    2.  识别 标识符
    3.  识别 数
    4.  识别 两个字符构成的 Symbol
    5.  识别 其他 Symbol
    6.  打印源程序，边读入字符边打印

### 二、语法分析程序 block
