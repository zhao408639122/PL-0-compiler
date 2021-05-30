#include "lexcial.h"



LexcicalAnalyzer::LexcicalAnalyzer(vector<pr> *NewResult = nullptr) {
    result = new vector<string>;
    ResultTable = NewResult;
    ans = 1;
}
    
bool LexcicalAnalyzer::checkKeywords(const string &input, int &pos) {
    pair<string, int> entity = make_pair("", 0);
    for (auto &i : Keywords) {
        if (equiv(input, i.first, pos)) {
            if (i.second == 1) {
                int tmp = pos + i.first.length();
                // cout << "INPUT[TMP]: " << input[tmp] << endl;
                if (tmp < input.length() && (isCharacter(input[tmp]) || isNumber(input[tmp]))) return false;
            }
            if (i.second > entity.second) entity = i;
        }
    }
    if (entity.second) {
        result->push_back(entity.first);
        if (ResultTable) ResultTable->push_back(entity);
        pos = pos + entity.first.length(); 
        return true;
    }
    return false;
}

bool LexcicalAnalyzer::checkIdentifier(const string &input, int &pos) {
    int tmp = pos;
    if (input[tmp] >= '0' && input[tmp] <= '9') {
        int aa = 0;
        while(tmp < input.length() && isNumber(input[tmp])) aa = aa * 10 + input[tmp++] - '0';
        if (tmp < input.length() && isCharacter(input[tmp])) {
            ans = 0;
            return 0;
        } else {
            result->push_back("NUMBER " + to_string(aa));
            if (ResultTable) ResultTable->push_back(make_pair(to_string(aa), 3));
            pos = tmp;
            return 1;
        }
    } else if (isCharacter(input[tmp])) {
        while(tmp < input.length() && (isCharacter(input[tmp]) || isNumber(input[tmp]))) tmp++;
        if (tmp - pos > 10) return 0;
        result->push_back("IDENTIFIER " + input.substr(pos, tmp - pos));
        if (ResultTable) ResultTable->push_back(make_pair(input.substr(pos, tmp - pos), 5));
        pos = tmp;
        return 1;
    }
    return 0;
} 

void LexcicalAnalyzer::analyze(string input) {
    for (auto &i : input) {
        if (i >= 'a' && i <= 'z')
            i = i - 'a' + 'A';
    }
    // cout << input << endl;
    int pos = 0;
    while(pos < input.length()) {
        if (checkKeywords(input, pos)) continue;
        if (checkIdentifier(input, pos)) continue;
        else {
            ans = 0;
            break;
        }      
    }
}

void LexcicalAnalyzer::printAnswer(int PrintAns = 1) {
    if (ans) {
        if (PrintAns) 
            for (auto &i : *result)
                cout << i << endl;
    }
    else cout << "Lexical Error" << endl;
}

int LexcicalAnalyzer::getAns() { return ans; }
