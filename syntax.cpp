#include "syntax.h"

SyntaxAnalyzer::SyntaxAnalyzer() {
    ResultRoot = nullptr;
    Pass = true;
}

void SyntaxAnalyzer::analyze(vector <pr> *InputString) {
    Input = InputString;
    pos = stack = 0;
    if (InputString->size() == 0) setError();
    program();
    if (pos < InputString->size()) setError();
}

//PROGRAM
void SyntaxAnalyzer::program() {
    ResultRoot = new TreeNode("PROGRAM");    
    subprog(ResultRoot);
    if (nowpr().second == 6 && nowpr().first == ".") {
        ResultRoot->addChild(".");
        pos++;
    } else setError();
}
//SUBPROG
void SyntaxAnalyzer::subprog(TreeNode *Node) {
    Node = Node->addChild("SUBPROG");
    if (nowpr().second == 1 && nowpr().first == "CONST") {
        constantdeclare(Node);
    }
    if (nowpr().second == 1 && nowpr().first == "VAR") {
        variabledeclare(Node);
    }
    if (nowpr().second == 1 && nowpr().first == "PROCEDURE") {
        proceduredeclare(Node);
    }
    sentence(Node);
}
//CONSTANTDECLARE
void SyntaxAnalyzer::constantdeclare(TreeNode *Node) {
    Node = Node->addChild("CONSTANTDECLARE");
    Node->addChild("CONST");
    pos++;
    constantdefine(Node);
    while(nowpr().second == 6 && nowpr().first == ",") {
        Node->addChild("COMMA");
        pos++;
        constantdefine(Node);
    }
    checkSemicolon(Node);
}

void SyntaxAnalyzer::constantdefine(TreeNode *Node) {
    Node = Node->addChild("CONSTANTDEFINE");
    if (nowpr().second != 5) setError(Node);
    Node->addChild((*Input)[pos++].first);
    if (nowpr().second != 2 || nowpr().first != "=") setError(Node);
    Node->addChild((*Input)[pos++].first);
    if (nowpr().second != 3) setError(Node);
    Node->addChild((*Input)[pos++].first);
    
}

void SyntaxAnalyzer::variabledeclare(TreeNode *Node) {
    Node = Node->addChild("VARIABLEDECLARE");
    Node->addChild("VAR");
    pos++;
    if (nowpr().second != 5) setError(Node);
    Node->addChild((*Input)[pos++].first);
    while(nowpr().second == 6 && nowpr().first == ",") {
        Node->addChild("COMMA");
        pos++;
        if (nowpr().second != 5) setError(Node);
        Node->addChild((*Input)[pos++].first);
    }
    checkSemicolon(Node);
}

void SyntaxAnalyzer::proceduredeclare(TreeNode *Node) {
    Node = Node->addChild("PROCEDUREDECLARE");
    procedurehead(Node);
    if (++stack > 3) setError(Node);
    subprog(Node);
    stack--;
    checkSemicolon(Node);
    while(nowpr().second == 1 && nowpr().first == "PROCEDURE") 
        proceduredeclare(Node);
}

void SyntaxAnalyzer::procedurehead(TreeNode *Node) {
    Node = Node->addChild("PROCEDUREHEAD");
    Node->addChild("PROCEDURE");
    pos++;
    if (nowpr().second != 5) setError(Node);
    Node->addChild((*Input)[pos++].first);
    checkSemicolon(Node);
}

void SyntaxAnalyzer::sentence(TreeNode *Node) {
    Node = Node->addChild("SENTENCE");
    if (nowpr().second == 1) {
        if (nowpr().first == "IF") 
            ifsentence(Node);
        else if (nowpr().first == "CALL") 
            callsentence(Node);
        else if (nowpr().first == "WHILE") 
            whilesentence(Node);
        else if (nowpr().first == "WRITE") 
            writesentence(Node);
        else if (nowpr().first == "READ")
            readsentence(Node);
        else if (nowpr().first == "BEGIN")
            combined(Node);
        else 
            Node->addChild("EMPTY");
    }
    else if (nowpr().second == 5) 
        assignment(Node);
    else 
        Node->addChild("EMPTY");
} 

void SyntaxAnalyzer::assignment(TreeNode *Node) {
    Node = Node->addChild("ASSIGNMENT");
    Node->addChild((*Input)[pos++].first);
    if (nowpr().second == 2 && nowpr().first == ":=") {
        Node->addChild(":=");
        pos++;
        expression(Node);
    } else setError(Node);
}

void SyntaxAnalyzer::combined(TreeNode *Node) {
    Node = Node->addChild("COMBINED");
    Node->addChild("BEGIN");
    pos++;
    sentence(Node);
    while(nowpr().second == 6 && nowpr().first == ";") {
        Node->addChild(";");
        pos++;
        sentence(Node);
    } 
    if (nowpr().second == 1 && nowpr().first == "END") {
        Node->addChild("END");
        pos++;
    } else setError(Node);
}

void SyntaxAnalyzer::expression(TreeNode *Node) {
    Node = Node->addChild("EXPRESSION");
    if (nowpr().second == 2 && (nowpr().first == "+" || nowpr().first == "-")) {
        Node->addChild((*Input)[pos++].first);
    } 
    item(Node);
    while(nowpr().second == 2 && (nowpr().first == "+" || nowpr().first == "-")) {
        Node->addChild((*Input)[pos++].first);
        item(Node);
    }
}

void SyntaxAnalyzer::item(TreeNode *Node) {
    Node = Node->addChild("ITEM");
    factor(Node);
    while(nowpr().second == 2 && (nowpr().first == "*" || nowpr().first == "/")) {
        Node->addChild((*Input)[pos++].first);
        factor(Node);
    }
}

void SyntaxAnalyzer::factor(TreeNode *Node) {
    Node = Node->addChild("FACTOR");
    if (nowpr().second == 5 || nowpr().second == 3) {
        Node->addChild((*Input)[pos++].first);
    } 
    else if (nowpr().second == 6 && nowpr().first == "(") {
        Node->addChild("LP");
        pos++;
        expression(Node);
        if (nowpr().second == 6 && nowpr().first == ")") {
            Node->addChild("RP");
            pos++;
        } else setError(Node);
    } else setError(Node);
}

void SyntaxAnalyzer::ifsentence(TreeNode *Node) {
    Node = Node->addChild("IFSENTENCE");
    Node->addChild("IF");
    pos++;
    condition(Node);
    if (nowpr().second == 1 && nowpr().first == "THEN") {
        Node->addChild("THEN");
        pos++;
    } else setError(Node);
    sentence(Node);
}


void SyntaxAnalyzer::condition(TreeNode *Node) {
    Node = Node->addChild("CONDITION");
    if (nowpr().second == 1 && nowpr().first == "ODD") {
        Node->addChild("ODD");
        pos++;
        expression(Node);
    } else {
        expression(Node);
        relationalOperator(Node);
        expression(Node);
    }
}

void SyntaxAnalyzer::callsentence(TreeNode *Node) {
    Node = Node->addChild("CALLSENTENCE");
    Node->addChild("CALL");
    pos++;
    if (nowpr().second == 5) {
        Node->addChild((*Input)[pos++].first);
    } else setError(Node);
}

void SyntaxAnalyzer::whilesentence(TreeNode *Node) {
    Node = Node->addChild("WHILESENTENCE");
    Node ->addChild("WHILE");
    pos++;
    condition(Node);
    if (nowpr().second == 1 && nowpr().first == "DO") {
        Node->addChild("DO");
        pos++;
        sentence(Node);
    } else setError(Node);
}

void SyntaxAnalyzer::readsentence(TreeNode *Node) {
    Node = Node->addChild("READSENTENCE");
    Node->addChild("READ");
    pos++;
    if (nowpr().second == 6 && nowpr().first == "(") {
        Node->addChild("LP");
        pos++;
        if (nowpr().second != 5) setError(Node);
        Node->addChild((*Input)[pos++].first);
        while(nowpr().second == 6 && nowpr().first == ",") {
            Node->addChild("COMMA");
            pos++;
            if (nowpr().second != 5) setError(Node);
            Node->addChild((*Input)[pos++].first);    
        } 
        if (nowpr().second == 6 && nowpr().first == ")") {
            Node->addChild("RP");
            pos++;
        } else setError(Node);
    } else setError(Node);
}

void SyntaxAnalyzer::writesentence(TreeNode *Node) {
    Node = Node->addChild("WRITESENTENCE");
    Node ->addChild("WRITE");
    pos++;
    if (nowpr().second == 6 && nowpr().first == "(") {
        Node->addChild("LP");
        pos++;
        if (nowpr().second != 5) setError(Node);
        Node->addChild((*Input)[pos++].first);
        while(nowpr().second == 6 && nowpr().first == ",") {
            Node->addChild("COMMA");
            pos++;
            if (nowpr().second != 5) setError(Node);
            Node->addChild((*Input)[pos++].first);    
        } 
        if (nowpr().second == 6 && nowpr().first == ")") {
            Node->addChild("RP");
            pos++;
        } else setError(Node);
    } else setError(Node);   
}

// Keywords 1 
// operator 2 4
// number 3
// identifier 5
// flag 6