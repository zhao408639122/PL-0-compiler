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
    Code.init();
    subprog(ResultRoot);
    if (nowpr().second == 6 && nowpr().first == ".") {
        ResultRoot->addChild(".");
        pos++;
    } else setError();
}
//SUBPROG
void SyntaxAnalyzer::subprog(TreeNode *Node) {
    int OrgTableptr, OrgDataptr;
    Code.dataptr = 3;
    OrgTableptr = Code.tableptr;
    Code.table[OrgTableptr].addr = Code.getCodeptr();
    Code.gen("jmp", 0, 0);
    
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

    Code.code[Code.table[OrgTableptr].addr].a = Code.getCodeptr();
    Code.table[OrgTableptr].addr = Code.getCodeptr();
    Code.gen("int", 0, Code.dataptr);
    sentence(Node);
    Code.gen("opr", 0, Oreturn);
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
    if (nowpr().second != 5) setError(Node); // identifier 
    const string &name = nowpr().first;
    Node->addChild((*Input)[pos++].first);
    if (nowpr().second != 2 || nowpr().first != "=") setError(Node);
    Node->addChild((*Input)[pos++].first);
    
    if (nowpr().second != 3) setError(Node); // number 
    int num = atoi(nowpr().first.c_str());
    Code.enter(name, constant, num); // add a constant to table
    Node->addChild((*Input)[pos++].first);
    
}

void SyntaxAnalyzer::variabledeclare(TreeNode *Node) {
    Node = Node->addChild("VARIABLEDECLARE");
    Node->addChild("VAR");
    pos++;
    if (nowpr().second != 5) setError(Node);
    Code.enter(nowpr().first, variable);
    Node->addChild((*Input)[pos++].first);
    while(nowpr().second == 6 && nowpr().first == ",") {
        Node->addChild("COMMA");
        pos++;
        if (nowpr().second != 5) setError(Node);
        Code.enter(nowpr().first, variable); // add a variable to table
        Node->addChild((*Input)[pos++].first);
    }
    checkSemicolon(Node);
}

void SyntaxAnalyzer::proceduredeclare(TreeNode *Node) {
    Node = Node->addChild("PROCEDUREDECLARE");
    procedurehead(Node);
    if (++stack > 3) setError(Node);
    Code.lev++;
    int PreTableptr = Code.tableptr;
    int PreDataptr = Code.dataptr;
    subprog(Node);
    Code.lev--;
    Code.tableptr = PreTableptr;
    Code.dataptr = PreDataptr;
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
    Code.enter(nowpr().first, procedure);
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

    int TablePos = Code.find(nowpr().first);
    if (TablePos == 0) setError(Node, "Identifier was not declared.");
    TableItem &TableObject = Code.table[TablePos];
    if (TableObject.type != variable) setError(Node, "Invalid type of identifier.");

    Node->addChild((*Input)[pos++].first);
    if (nowpr().second == 2 && nowpr().first == ":=") {
        Node->addChild(":=");
        pos++;
        expression(Node);
        Code.gen("sto", Code.lev - TableObject.level, TableObject.addr);
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
    int opr = 0;
    if (nowpr().second == 2 && (nowpr().first == "+" || nowpr().first == "-")) {
        opr = nowpr().first == "-";
        Node->addChild((*Input)[pos++].first);    
    } 
    item(Node);
    if (opr) Code.gen("opr", 0, Oneg);
    
    while(nowpr().second == 2 && (nowpr().first == "+" || nowpr().first == "-")) {
        opr = nowpr().first == "+" ? Oplus : Ominus;
        
        Node->addChild((*Input)[pos++].first);
        item(Node);
        Code.gen("opr", 0, opr);
    }
}

void SyntaxAnalyzer::item(TreeNode *Node) {
    Node = Node->addChild("ITEM");
    factor(Node);
    while(nowpr().second == 2 && (nowpr().first == "*" || nowpr().first == "/")) {
        OprType opr = nowpr().first == "*" ? Otimes : Odivide;
        Node->addChild((*Input)[pos++].first);
        factor(Node);
        Code.gen("opr", 0, opr);
    }
}

void SyntaxAnalyzer::factor(TreeNode *Node) {
    Node = Node->addChild("FACTOR");
    if (nowpr().second == 5) {
        int TablePos = Code.find(nowpr().first);
        if (!TablePos) setError(Node, "Identifier was not declared.");
        TableItem TableObject = Code.table[TablePos];
        switch (TableObject.type) {
            case constant: 
                Code.gen("lit", 0, TableObject.val);
                break;
            case variable:
                Code.gen("lod", Code.lev - TableObject.level, TableObject.addr);
                break;
            case procedure: 
                setError(Node, "Procedure identifier cannot place in a factor.");
                break;
        }
        Node->addChild((*Input)[pos++].first);
    } 
    else if (nowpr().second == 3) {
        Code.gen("lit", 0, atoi(nowpr().first.c_str()));
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
    int PreCodeptr;
    Node = Node->addChild("IFSENTENCE");
    Node->addChild("IF");
    pos++;
    condition(Node);
    if (nowpr().second == 1 && nowpr().first == "THEN") {
        Node->addChild("THEN");
        pos++;
    } else setError(Node);
    PreCodeptr = Code.getCodeptr();
    Code.gen("jpc", 0, 0);
    sentence(Node);
    Code.code[PreCodeptr].a = Code.getCodeptr();
    
}


void SyntaxAnalyzer::condition(TreeNode *Node) {
    Node = Node->addChild("CONDITION");
    if (nowpr().second == 1 && nowpr().first == "ODD") {
        Node->addChild("ODD");
        pos++;
        expression(Node);
        Code.gen("opr", 0, Oodd);
    } else {
        expression(Node);
        OprType relop = relationalOperator(Node);
        expression(Node);
        Code.gen("opr", 0, relop);
    }
}

void SyntaxAnalyzer::callsentence(TreeNode *Node) {
    Node = Node->addChild("CALLSENTENCE");
    Node->addChild("CALL");
    pos++;
    if (nowpr().second == 5) {
        int TablePos = Code.find(nowpr().first);
        if (TablePos == 0) setError(Node, "Identifier was not declared.");
        TableItem &TableObject = Code.table[TablePos];
        if (TableObject.type != procedure) setError(Node, "The identifier was not a procedure.");
        Code.gen("cal", Code.lev - TableObject.level, TableObject.addr);
        Node->addChild((*Input)[pos++].first);
    } else setError(Node);
}

void SyntaxAnalyzer::whilesentence(TreeNode *Node) {
    Node = Node->addChild("WHILESENTENCE");
    Node ->addChild("WHILE");
    pos++;
    int WhileCodeptr = Code.getCodeptr();
    condition(Node);
    int DoCodeptr = Code.getCodeptr();
    Code.gen("jpc", 0, 0);
    if (nowpr().second == 1 && nowpr().first == "DO") {
        Node->addChild("DO");
        pos++;
        sentence(Node);
        Code.gen("jmp", 0, WhileCodeptr);
        Code.code[DoCodeptr].a = Code.getCodeptr();
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
        
        int TablePos = Code.find(nowpr().first);
        if (TablePos == 0) setError(Node, "Identifier was not declared.");
        TableItem &TableObject = Code.table[TablePos];
        if (TableObject.type != variable) setError(Node, "Invalid type of identifier.");
        Code.gen("opr", 0, Oread);
        Code.gen("sto", Code.lev - TableObject.level, TableObject.addr);
        
        Node->addChild((*Input)[pos++].first);
        while(nowpr().second == 6 && nowpr().first == ",") {
            Node->addChild("COMMA");
            pos++;
            if (nowpr().second != 5) setError(Node);
            // check in t
            int TablePos = Code.find(nowpr().first);
            if (TablePos == 0) setError(Node, "Identifier was not declared.");
            TableItem &TableObject = Code.table[TablePos];
            if (TableObject.type != variable) setError(Node, "Invalid type of identifier.");
            Code.gen("opr", 0, Oread);
            Code.gen("sto", Code.lev - TableObject.level, TableObject.addr);
            
            Node->addChild((*Input)[pos++].first);    
        } 
        if (nowpr().second == 6 && nowpr().first == ")") {
            Node->addChild("RP");
            pos++;
        } else setError(Node);
    } else setError(Node);
}

void SyntaxAnalyzer::writesentence(TreeNode *Node) {// to do: add generator
    Node = Node->addChild("WRITESENTENCE");
    Node ->addChild("WRITE");
    pos++;
    if (nowpr().second == 6 && nowpr().first == "(") {
        Node->addChild("LP");
        pos++;
        if (nowpr().second != 5) setError(Node);
        
        int TablePos = Code.find(nowpr().first);
        if (TablePos == 0) setError(Node, "Identifier was not declared.");
        TableItem &TableObject = Code.table[TablePos];
        switch (TableObject.type) {
            case procedure: 
                setError(Node, "Invalid type of identifier.");
            case variable: 
                Code.gen("lod", Code.lev - TableObject.level, TableObject.addr);
                Code.gen("opr", 0, Owrite);
                break; 
            case constant: 
                Code.gen("lit", 0, TableObject.val);
                Code.gen("opr", 0, Owrite);
                break; 
        }
        
        Node->addChild((*Input)[pos++].first);
        while(nowpr().second == 6 && nowpr().first == ",") {
            Node->addChild("COMMA");
            pos++;
            if (nowpr().second != 5) setError(Node);
            
            int TablePos = Code.find(nowpr().first);
            if (TablePos == 0) setError(Node, "Identifier was not declared.");
            TableItem &TableObject = Code.table[TablePos];
            switch (TableObject.type) {
                case procedure: 
                    setError(Node, "Invalid type of identifier.");
                case variable: 
                    Code.gen("lod", Code.lev - TableObject.level, TableObject.addr);
                    Code.gen("opr", 0, Owrite);
                    break; 
                case constant: 
                    Code.gen("lit", 0, TableObject.val);
                    Code.gen("opr", 0, Owrite);
                    break;
            }
            
            Node->addChild((*Input)[pos++].first);    
        } 
        if (nowpr().second == 6 && nowpr().first == ")") {
            Node->addChild("RP");
            pos++;
        } else setError(Node);
    } else setError(Node);   
}
void SyntaxAnalyzer::listCode() { 
    Code.listCode();
}
// Keywords 1 
// operator 2 4
// number 3
// identifier 5
// flag 6