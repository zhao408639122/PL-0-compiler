#ifndef LEXICAL_H
#define LEXICAL_H

#include "common.h"


class LexcicalAnalyzer {
public:
    LexcicalAnalyzer(vector<pr> *NewResult);
    bool checkKeywords(const string &, int &);
    bool checkIdentifier(const string &, int &);
    void analyze(string);
    static unordered_map<string, int> Keywords;
    // Keywords 1
    // operator 2 4
    // number 3
    // identifier 5
    // flag 6
    void printAnswer(int PrintAns);
    int getAns();
private:
    vector<string> *result;
    vector<pr> *ResultTable;
    int ans;
};


#endif