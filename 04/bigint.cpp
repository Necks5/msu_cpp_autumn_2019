#include "bigint.h"

int BigInt::get_sign(int a) {
    if(a >= 0) {
        return 1;
    } else {
        return -1;
    }
}
int BigInt::int_to_mas(int val) {
    int i = 0;
    if(val == 0) {
        return 1;
    }
    while(val != 0) {
        mem[i] = val % osnov;
        val /= osnov;
        i++;
    }
    return i;
} 
void BigInt::realloc() {
    int* new_mem = new int[max_ind * 2];
    memset(new_mem, 0, max_ind * 2 * sizeof(int));
    memcpy(new_mem, mem, now_ind * sizeof(int));
    max_ind *= 2;
    delete[] mem;
    mem = new_mem;        
}
BigInt::BigInt(int val = 0) : max_ind(sizeof(int)) {
    mem = new int[max_ind];
    memset(mem, 0, max_ind* sizeof(int));
    sign = get_sign(val);
    now_ind = int_to_mas(val * sign);
}   
BigInt::~BigInt() {
    delete[] mem;
}
BigInt::BigInt(const BigInt& d) : max_ind(d.max_ind), now_ind(d.now_ind), sign(d.sign){
    mem = new int[max_ind];
    memset(mem, 0, max_ind * sizeof(int));
    memcpy(mem, d.mem, now_ind * sizeof(int));    
}
BigInt& BigInt::operator=(const BigInt& d){
    now_ind = d.now_ind;
    max_ind = d.max_ind;
    sign = d.sign;
    delete[] mem;
    mem = new int[max_ind];
    memcpy(mem, d.mem, max_ind * sizeof(int));
    return *this;        
}
bool BigInt::operator>(const BigInt& other) const {
    if(this->sign == 1 && other.sign == -1) {
        return true;
    }
    if(this->sign == -1 && other.sign == 1) {
        return false;
    }
    if(this->sign == 1 && other.sign == 1) {
        if(this->now_ind > other.now_ind) {
            return true;
        } else if(this->now_ind < other.now_ind) {
            return false;
        } else {
            for(int i = this->now_ind - 1; i >= 0; i--) {
                if(this->mem[i] > other.mem[i]) {
                    return true;
                } else if(this->mem[i] < other.mem[i]) {
                    return false;
                }
            }
            return false;
        }
    } else {
        if(this->now_ind > other.now_ind) {
            return false;
        } else if(this->now_ind < other.now_ind) {
            return true;
        } else {
            for(int i = this->now_ind - 1; i >= 0; i--) {
                if(this->mem[i] > other.mem[i]) {
                    return false;
                } else if(this->mem[i] < other.mem[i]) {
                    return true;
                }
            }
            return false;
        }
    }
    
}
bool BigInt::operator==(const BigInt& other) const {
    if(this == &other) {
        return true;
    }
    if(this->sign == other.sign && this->now_ind == other.now_ind) {
        for(int i = 0; i < this->now_ind; i++) {
            if(this->mem[i] != other.mem[i]) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}
bool BigInt::operator<(const BigInt& other)const  {
    return !(*this > other);
}
bool BigInt::operator<=(const BigInt& other) const {
    return (*this < other || *this == other);
}
bool BigInt::operator>=(const BigInt& other) const {
    return (*this > other || *this == other);
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}
BigInt BigInt::operator+(const BigInt& other) const {
    if(this->sign == 1 && other.sign == -1) {
        BigInt tmp = other;
        tmp.sign = 1;
        return *this - tmp;
    }
    if(this->sign == -1 && other.sign == 1) {
        BigInt tmp = *this;
        tmp.sign = 1;
        return other - tmp;
    }
    if(other > *this) {
        return other + *this;
    }
    
    BigInt tmp = *this;
    if(other.now_ind + 1 > tmp.max_ind) {
        tmp.realloc();
    }
    for(int i = 0; i < other.now_ind; i++){
        tmp.mem[i] += other.mem[i];
        tmp.mem[i + 1] += tmp.mem[i] / osnov;
        tmp.mem[i] %= osnov;        
    }
    int last_nozero = 0;
    for(int i = 0; i < tmp.max_ind; i++) {
        //std::cout << i << '\t' << tmp.mem[i] << std::endl;
        if(tmp.mem[i] != 0) {
            last_nozero = i;
        }
    }
    tmp.now_ind = last_nozero + 1;
    return tmp;
    
}
BigInt BigInt::operator-(const BigInt& other) const {
    if(this->sign == 1 && other.sign == -1) {
        BigInt tmp = other;
        tmp.sign = 1;
        return *this + tmp;
    }
    if(this->sign == -1 && other.sign == 1) {
        BigInt tmp = other;
        tmp.sign = -1;
        return *this + tmp;
    }
    
    if(other > *this) {
        BigInt tmp = other - *this;
        tmp.sign *= -1;
        return tmp;
    }
    BigInt tmp = *this;
    int carry = 0;
    for(int i = 0; i < other.now_ind || carry; i++) {
        tmp.mem[i] -= carry + (i < other.now_ind ? other.mem[i] : 0);
        carry = tmp.mem[i] < 0;
        if(carry) {
            tmp.mem[i] += osnov;
        }
    }
    int last_nozero = 0;
    for(int i = 0; i < tmp.max_ind; i++) {
        if(tmp.mem[i] != 0) {
            last_nozero = i;
        }
    }
    tmp.now_ind = last_nozero + 1;
    return tmp;
}
BigInt BigInt::operator-() const {
    BigInt tmp = *this;
    tmp.sign *= -1;
    return tmp;
}
std::ostream& operator<<(std::ostream& stream, const BigInt& num){
    if (num.sign == -1) {
        stream << '-';
    }
    stream << num.mem[num.now_ind - 1];
    for(int i = num.now_ind - 2; i >= 0; i--) {
        if(num.mem[i] < 10) {
            stream << '0';
        }
        stream << num.mem[i];
    }
    return stream;
}
