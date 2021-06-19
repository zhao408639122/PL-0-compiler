#include "lexcial.h"
#include "syntax.h"

unordered_map<string, int> LexcicalAnalyzer::Keywords;
void initialize() {
    LexcicalAnalyzer::Keywords["CONST"] = 1;
    LexcicalAnalyzer::Keywords["VAR"] = 1;
    LexcicalAnalyzer::Keywords["PROCEDURE"] = 1;
    LexcicalAnalyzer::Keywords["BEGIN"] = 1;
    LexcicalAnalyzer::Keywords["END"] = 1; 
    LexcicalAnalyzer::Keywords["ODD"] = 1;
    LexcicalAnalyzer::Keywords["IF"] = 1;
    LexcicalAnalyzer::Keywords["THEN"] = 1;
    LexcicalAnalyzer::Keywords["CALL"] = 1;
    LexcicalAnalyzer::Keywords["WHILE"] = 1;
    LexcicalAnalyzer::Keywords["DO"] = 1;
    LexcicalAnalyzer::Keywords["READ"] = 1;
    LexcicalAnalyzer::Keywords["WRITE"] = 1;
    LexcicalAnalyzer::Keywords["="] = 2;
    LexcicalAnalyzer::Keywords[":="] = 2;
    LexcicalAnalyzer::Keywords["+"] = 2;
    LexcicalAnalyzer::Keywords["-"] = 2;
    LexcicalAnalyzer::Keywords["*"] = 2;
    LexcicalAnalyzer::Keywords["/"] = 2;
    LexcicalAnalyzer::Keywords["#"] = 2;
    LexcicalAnalyzer::Keywords["<"] = 2;
    LexcicalAnalyzer::Keywords["<="] = 4;
    LexcicalAnalyzer::Keywords[">"] = 2;
    LexcicalAnalyzer::Keywords[">="] = 4;
    LexcicalAnalyzer::Keywords[";"] = 6;
    LexcicalAnalyzer::Keywords[","] = 6;
    LexcicalAnalyzer::Keywords["."] = 6;
    LexcicalAnalyzer::Keywords["("] = 6;
    LexcicalAnalyzer::Keywords[")"] = 6;
    
}


int main() {
    // freopen("lexical_partial_testcases/std.in", "r", stdin);
    // freopen("ans.out", "w", stdout);
    ios::sync_with_stdio(false);
    initialize();
    string input;
    vector<pr> *LexcicalAnswer = new vector<pr>; 
    LexcicalAnalyzer *LexicalMachine = new LexcicalAnalyzer(LexcicalAnswer);
    SyntaxAnalyzer *SyntaxMachine = new SyntaxAnalyzer;
    while(cin >> input) {
        // if (input[0] == '&') break;
        LexicalMachine->analyze(input);
    }
 
    if (LexicalMachine->getAns() == 0) {
        cout << "Lexical Error" << endl;
        exit(1);
    }
    // int pos = 0;
    // for (auto i : *LexcicalAnswer) {
    //     cout << pos++ << " first: " << i.first << "  second: " << i.second << endl; 
    // }
    
    SyntaxMachine->analyze(LexcicalAnswer);
    SyntaxMachine->printAnswer();

    // machine->printAnswer();
    
    // system("fc ans.out ans1.out");
    // system("pause");
}