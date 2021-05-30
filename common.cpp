#include "common.h"

bool equiv(const string &a, const string &b, int offset) {
    if (a.length() < b.length() + offset) return 0;
    for (int i = 0; i < b.length(); ++i) {
        if (a[i + offset] != b[i]) return false;
    }
    return true;
}

bool isCharacter(const char &a) {
    return a >= 'A' && a <= 'Z';
}


bool isNumber(const char &a) {
    return a >= '0' && a <= '9';
}