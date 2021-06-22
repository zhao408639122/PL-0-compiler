#include "common.h"
#include <fstream>
const int maxS = 100000; 
int dataStack[maxS];     
vector<Instruction> code; 

int base(int addr, int lev) {
    while (lev) {
        addr = dataStack[addr]; 
        lev--; 
    }
    // cout << "addr : " << addr << endl;
    return addr;
}


int main() { //BP 基址寄存器; IR 指令寄存器; IP 指令地址寄存器; SP 栈顶寄存器
    int pc = 0, bp = 1, sp = 0;
    Instruction ir;
    ifstream in("program.code"); 
    // freopen("ans.out", "w", stdout);
    while(!in.eof()) {
        ir.readin(in);
        code.push_back(ir);
    }
    // for (auto i : code) i.print();
    // system("pause");
    do {
        ir = code[pc++];
        if (ir.code == "lit") {
            dataStack[++sp] = ir.a;
        } 
        else if (ir.code == "lod") {
            dataStack[++sp] = dataStack[base(bp, ir.l) + ir.a];
        }
        else if (ir.code == "sto") {
            dataStack[base(bp, ir.l) + ir.a] = dataStack[sp--];   
             
        } 
        else if (ir.code == "cal") {
            // cout << "cal: " << endl << "sp : " << sp << endl;
            dataStack[sp + 1] = base(bp, ir.l);
            dataStack[sp + 2] = bp;
            dataStack[sp + 3] = pc;
            bp = sp + 1, pc = ir.a;
        }
        else if (ir.code == "int") {
            sp += ir.a; 
        }
        else if (ir.code == "jmp") {
            pc = ir.a;
        }
        else if (ir.code == "jpc") {
            if (!dataStack[sp]) {
                pc = ir.a;
            }
            sp--;
        }
        else if (ir.code == "opr") {
            switch (ir.a) { //  Oodd, Oequal, Oneq, Oless, Oleq, Ogreater, Ogeq, Oread, Owrite, Oreturn
                case Oreturn: 
                    sp = bp - 1; 
                    pc = dataStack[sp + 3]; 
                    bp = dataStack[sp + 2];
                    break;
                case Oneg: 
                    dataStack[sp] = -dataStack[sp];
                    break;
                case Oplus: 
                    sp--; 
                    dataStack[sp] += dataStack[sp + 1];
                    break;
                case Ominus: 
                    sp--; 
                    dataStack[sp] -= dataStack[sp + 1];
                    break;
                case Otimes: 
                    sp--; 
                    dataStack[sp] *= dataStack[sp + 1];
                    break; 
                case Odivide: 
                    sp--; 
                    dataStack[sp] /= dataStack[sp + 1];
                    break; 
                case Oodd: 
                    dataStack[sp] %= 2;
                    break; 
                case Oequal: 
                    sp--; 
                    dataStack[sp] = dataStack[sp] == dataStack[sp + 1];
                    break; 
                case Oneq: 
                    sp--; 
                    dataStack[sp] = dataStack[sp] != dataStack[sp + 1];
                    break; 
                case Oless: 
                    sp--; 
                    dataStack[sp] = dataStack[sp] < dataStack[sp + 1];
                    break; 
                case Ogreater:
                    sp--;  
                    dataStack[sp] = dataStack[sp] > dataStack[sp + 1];
                    break;   
                case Oleq: 
                    sp--; 
                    dataStack[sp] = dataStack[sp] <= dataStack[sp + 1]; 
                    break; 
                case Ogeq:
                    sp--; 
                    dataStack[sp] = dataStack[sp] >= dataStack[sp + 1]; 
                    break; 
                case Oread: 
                    int Number; 
                    cin >> Number;
                    dataStack[++sp] = Number; 
                    break;
                case Owrite: 
                    cout << dataStack[sp--] << endl; 
                    break;  
            }
        }
        else {
            cout << "error: undefined instruction." << endl;
            ir.print(); 
        }
        // system("pause"); 
        // cout << "pc: " << pc << ", sp: " << sp << endl;  
        // for (int i = 1; i <= sp; ++i) cout << dataStack[i] << ' '; 
        // cout << endl; 
    } while(pc != 0);
    // system("pause");
}