#ifndef COMMON_H
#define COMMON_H

#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<vector>
#include<cstdlib>

using namespace std;

typedef pair<string, int> pr;

bool equiv(const string &a, const string &b, int offset);
bool isCharacter(const char &a);
bool isNumber(const char &a);

// Keywords 1
// operator 2 4
// number 3
// identifier 5
// flag 6

struct TreeNode{
    string s;
    int size;
    TreeNode* father;
    vector<TreeNode*> children;
    TreeNode(const string &a) {
        s = a;
        father = nullptr;
    }
    TreeNode(string && a) {
        s = a;
        father = nullptr;
    }
    template<typename T>
    TreeNode* addChild(T && a) {
        TreeNode *son = new TreeNode(forward<T>(a));
        children.push_back(son);
        son->father = this;
        return son;
    }

    void print() {
        cout << s;
        if (children.size() > 0) {
            cout << '(';
            int k = children.size() - 1;
            for (int i = 0; i < k; ++i) {
                children[i]->print();
                cout << ',';
            } 
            children[k]->print();
            cout << ')';
        }
    }
};

#endif // common_h