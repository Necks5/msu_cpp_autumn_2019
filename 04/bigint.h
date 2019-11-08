#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
const int osnov = 100;
class BigInt{
    int * mem;
    int max_ind;
    int now_ind;
    int sign;
    
    int get_sign(int a);
    int int_to_mas(int val);
    void realloc();
public:
    BigInt(int val);
    BigInt(const BigInt& d);
    ~BigInt();
    BigInt& operator=(const BigInt& d);
    bool operator>(const BigInt& other) const;
    bool operator<(const BigInt& other)const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator-() const;
    friend std::ostream& operator<<(std::ostream& stream, const BigInt& num);   
};