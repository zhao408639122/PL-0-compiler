#ifndef CODEGEN_H
#define CODEGEN_H
#include "common.h"

#define maxTable 10000

class CodeGenerator{
public:
    CodeGenerator();
    void setError(string &&);
    void init();
    void gen(string s, int l, int a);
    void enter(const string &s, enum WordType type, int num = 0);
    int find(const string &s);
    int getCodeptr();
    void listCode();
    void listTable();
    vector<Instruction> code;
    int tableptr, dataptr, lev;
    TableItem table[maxTable];
};

#endif
