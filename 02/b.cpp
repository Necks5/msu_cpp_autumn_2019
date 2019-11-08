#include "allocator.h"
#include <iostream>
#include <cassert>

void test1() {
    LinearAllocator temp(10);
    assert(nullptr != temp.alloc(10));
    assert(nullptr == temp.alloc(1));
    temp.reset();
    assert(nullptr != temp.alloc(5));
    assert(nullptr != temp.alloc(5));
    assert(nullptr == temp.alloc(1));
}
void test2() {
    LinearAllocator temp(10);
    assert(nullptr == temp.alloc(11));
    assert(nullptr != temp.alloc(5));
    assert(nullptr != temp.alloc(5));
    assert(nullptr == temp.alloc(1));
}

int main() {
    test1();
    test2();
    return 0;
}