#include "matrix.h"

Matrix::Proxy::Proxy(size_t m_cols, int* r) : max_col(m_cols) , row(r) { }
int& Matrix::Proxy::operator[](size_t i) {
    if(i >= max_col) {
        throw std::out_of_range("");
    }
    return row[i];
}      
const int& Matrix::Proxy::operator[](size_t i) const {
    if(i >= max_col) {
        throw std::out_of_range("");
    }
    return row[i];
}       
 
Matrix::Matrix(size_t a, size_t b) : rows(a), cols(b) {
    matr = new int*[rows];
    for(int i = 0; i < rows; ++i) {
        matr[i] = new int[cols];
    }
}
size_t Matrix::getRows() const {
    return this->rows;
}
size_t Matrix::getColumns() const {
    return this->cols;
}
Matrix& Matrix::operator*=(int n) {
    for(int i = 0; i < this->getRows(); ++i) {
        for(int j = 0; j < this->getColumns(); ++j) {
            matr[i][j] *= n;
        }
    }
    return *this;
}
bool Matrix::operator==(const Matrix & m) const {
    if(this == &m) {
        return true;
    }
    if(rows != m.getRows() || cols != m.getColumns()) {
        return false;
    }
    for(int i = 0; i < this->getRows(); ++i) {
        for(int j = 0; j < this->getColumns(); ++j) {
            if(matr[i][j] != m.matr[i][j]) {
                return false;
            }
        }
    }
    return true;
}
bool Matrix::operator!=(const Matrix& m) const {
    return !(*this == m);
}
Matrix::Proxy Matrix::operator[](size_t i) {
    if(i >= this->getRows()) {
        throw std::out_of_range("");
    }
    return Proxy(this->getColumns(), matr[i]);
}
const Matrix::Proxy Matrix::operator[](size_t i) const {
    if(i >= this->getRows()) {
        throw std::out_of_range("");
    }
    return Proxy(this->getColumns(), matr[i]);
}

Matrix::~Matrix(){
    for(int i = 0; i < this->getRows(); i++) {
        delete[] matr[i];
    }
    delete[] matr;
}