#include <cstddef>
#include <stdexcept>
class Matrix{
    size_t rows, cols;
    int** matr;
public:
	class Proxy{
        size_t row_ind, max_col;
        int* row;
    public:
        Proxy(size_t m_cols, int* r);
        int& operator[](size_t i);       
        const int& operator[](size_t i) const;  
    };


    Matrix(size_t a, size_t b);
    size_t getRows() const;
    size_t getColumns() const;
    Matrix& operator*=(int n);
    bool operator==(const Matrix& m) const;
    bool operator!=(const Matrix& m) const;
    Proxy operator[](size_t i);
    const Proxy operator[](size_t i) const;
    ~Matrix();
};