#include "matrix.h"
#include <iostream>
#include <cassert>

void test1() {
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);

    m[1][2] = 5; 
    double x = m[1][2];
    assert(x == 5);
}
void test2() {
    const size_t rows = 4;
    const size_t cols = 4;
    Matrix m1(rows, cols);
    Matrix m2(rows, cols);
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < rows; j++) {
            m1[i][j] = i * j;
            m2[i][j] = m1[i][j];
        }
    }
    assert(m1 == m2);
    m1 *= 3;
    assert(m1 != m2);
    assert(m1[2][2] == 12);

}
void test3(){
    const size_t rows = 4;
    const size_t cols = 4;
    const Matrix m(rows, cols);
    assert(m.getRows() == 4);
    assert(m.getColumns() == 4);
    assert(m[2][2] == 0);
}
int main() {
    test1();
    test2();
    test3();
    return 0;
}