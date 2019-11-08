#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>

class Exception
{
    const std::string error;
public:
    Exception(std::string const & error) : error(error) {}
    const char* get_error() { 
        return error.c_str(); 
    }
};

class Calc
{
    const std::string str;
    int result;
    bool calculated;
    char get_char(std::stringstream &ss) {
        char c;
        while((ss >> c) && (c == ' ')) {}
        return c;
    }
    void calc_res() {
        std::vector <int> adding;
        std::stringstream ss(str); 
        if(ss.eof()) {
            return;
        }
        int k;
        ss >> k;
        adding.push_back(k);
        char c;
        while(!ss.eof()) {
            c = get_char(ss);
            if(ss.peek() == -1) {
                throw Exception("incorrect input");
            }
            ss >> k;
            while(c == '*' || c == '/') {
                if(c == '*') {
                    adding[adding.size() - 1] *= k;
                } else {
                    adding[adding.size() - 1] /= k;
                }
                if(ss.eof()) {
                    break;
                }
                c = get_char(ss);
                if(ss.peek() == -1) {
                    throw Exception("incorrect input");
                }
                ss >> k;
            }
            if(c == '+') {
                adding.push_back(k);
            }
            if(c == '-') {
                adding.push_back(-1 * k);
            }
        }
        for(const int &x: adding) {
            result += x;
        }
    }
public:
    Calc(std::string const & input_string) : str(input_string), result(0), calculated(false) {}
    int get_result() {
        if(!calculated){
            calc_res();
        }
        return result;
    }
};

int main(int argc, char ** argv) {
    try {
        if(argc != 2) {
            throw Exception("argument error");
        }
        Calc c(argv[1]);
        std::cout << c.get_result();
    }
    catch(Exception &ex) {
        std::cout << ex.get_error() << std::endl;
        return 1;
    }
    return 0;
}
