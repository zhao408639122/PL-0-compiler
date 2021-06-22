#ifndef SYNTAX_H
#define SYNTAX_H

#include "common.h"
#include "codegen.h"

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    void setError(TreeNode* node = nullptr, string s = NULL) {
        cout << "[DELETE] pos: " << pos << "first: " << nowpr().first << " second: " << nowpr().second << endl;
        if (node != nullptr)
            cout << "[DELETE] s: " << node->s << ", children.size(): " << node->children.size() << endl;  
        if (s.length()) {
            cout << s << endl;
            Code.listTable();
        }
        Pass = false;
        printf("Syntax Error\n");
        exit(1);
    }
    void analyze(vector <pr> *InputString);
    void program(); 
    void subprog(TreeNode *Node);
    void constantdeclare(TreeNode *Node);
    void constantddefine(TreeNode *Node);
    void variabledeclare(TreeNode *Node);
    void constantdefine(TreeNode *Node);
    void proceduredeclare(TreeNode *Node);
    void procedurehead(TreeNode *Node);
    void sentence(TreeNode *Node);
    void assignment(TreeNode *Node);
    void combined(TreeNode *Node);
    void condition(TreeNode *Node);
    void expression(TreeNode *Node);
    void item(TreeNode *Node);
    void factor(TreeNode *Node);
    void ifsentence(TreeNode *Node);
    void callsentence(TreeNode *Node);
    void whilesentence(TreeNode *Node);
    void readsentence(TreeNode *Node);
    void writesentence(TreeNode *Node);
    void listCode();
    OprType relationalOperator(TreeNode *Node) {
        const pr &tmp = nowpr();
        Node->addChild(nowpr().first);
        pos++;
        if (tmp.second == 2) {
            switch (tmp.first[0]) {
                case '=': 
                    return Oequal;
                case '#':
                    return Oneq;
                case '<': 
                    return Oless;
                case '>':
                    return Ogreater;
                default: 
                    setError(Node);
            }
        }
        else if (tmp.second == 4) {
            OprType res = tmp.first[0] == '<' ? Oleq : Ogeq;
            return res;    
        }       
        else setError(Node);
    }

    void printAnswer() {
        ResultRoot->print();
    }
private:   
    pr& nowpr() {
        return (*Input)[pos];
    }
    
    void checkSemicolon(TreeNode *Node) {
        if (nowpr().second == 6 && nowpr().first == ";") {
            Node->addChild(";");
            pos++;
        } else setError(Node);
    }
    int stack, pos, Pass;
    TreeNode *ResultRoot;
    vector <pr> *Input;
    CodeGenerator Code;
};

#endif