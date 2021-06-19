#ifndef CODEGEN_H
#define CODEGEN_H
#include "common.h"

#define maxTable 10000
 

struct struct tableitem{
    string name;
    enum WordType type;
    int val;
    int level;
    int addr; 
};

class CodeGenerator{
public:
    CodeGenerator();
    void setError(string &&);
    void clear();
    void gen(string s, int l, int a);
    void enter(const string &s, enum WordType type, int num = 0);
    int find(const string &s);
    vector<string> code;
    int tableptr, dataptr, lev;
    tableitem table[maxTable];
};

#endif
