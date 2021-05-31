#ifndef SYNTAX_H
#define SYNTAX_H

#include "common.h"

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer();
    void setError(TreeNode* node = nullptr) {
        // cout << "[DELETE] pos: " << pos << "first: " << nowpr().first << " second: " << nowpr().second << endl;
        // if (node != nullptr)
        //     cout << "[DELETE] s: " << node->s << ", children.size(): " << node->children.size() << endl;  
        Pass = false;
        printf("Syntax Error\n");
        exit(0);
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
    void relationalOperator(TreeNode *Node) {
        if ((nowpr().second == 2  && (nowpr().first == "=" || nowpr().first == "#" || nowpr().first == "<" || nowpr().first == ">"))
        || nowpr().second == 4) {
            Node->addChild(nowpr().first);
            pos++;
        } else setError(Node);
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
};

#endif