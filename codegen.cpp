#include "common.h"
#include "codegen.h"

CodeGenerator::CodeGenerator() {}

void CodeGenerator::setError(string && s = NULL) {
    if (s.length()) {
        cout << s << endl;
        exit(0);
    }
}

void CodeGenerator::init() {
    dataptr = lev = tableptr = 0;
    code.clear();
}

void CodeGenerator::gen(string s, int l, int a) {
    code.push_back(Instruction(s, l, a));
}

void CodeGenerator::enter(const string &s, enum WordType type, int num) {
    tableptr = tableptr + 1;

    table[tableptr].name = s;
    table[tableptr].type = type;
    
    switch (type) {
        case constant: 
            if (num > INT_MAX) {
                setError("Number out of range");
            } 
            table[tableptr].val = num;
            break; 
        case variable:
            table[tableptr].level = lev;  
            table[tableptr].addr = dataptr++;
            break;
        case procedure: 
            table[tableptr].level = lev;
            break;       
    }
}

int CodeGenerator::find(const string &s) {
    for (int i = tableptr; i; --i) {
        if (table[i].name == s) return i;
    }
    return 0;
}

int CodeGenerator::getCodeptr() { 
    return code.size();
}
