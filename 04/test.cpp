#include "bigint.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

void test1() {
    BigInt a = -2;
    BigInt b = 5;
    assert(a + b == BigInt(3));
    assert(a == BigInt(-2));
    assert(b == BigInt(5));
    assert(a - b == BigInt(-7));
    assert(b - a == BigInt(7));
    assert(-a + b == BigInt(7));
    assert(-a + a == BigInt(0));
    assert(a + b + a == BigInt(1));
    assert(a == a);
    assert(-a != a);
    assert(a < b);
    assert(a <= b);
    assert(a != b);
}
void test2() {
    BigInt a = 1; 
    for(int i = 0; i < 100; i++) {
        a = a + a;
    }
    std::stringstream ss;
    ss << a;
    assert(ss.str() == "1267650600228229401496703205376"); // 2 ^ 100
}
int main() {
    test1();
    test2();
    return 0;
}