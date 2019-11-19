#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <exception> 

template<class T>
std::string args_to_str(T&& val)
{
    std::stringstream out;
    out << val;
    return out.str();
}

template<class... ArgsT>
std::string format(const std::string& str, ArgsT&&... argss) {
    std::vector<std::string> args = { args_to_str(std::forward<ArgsT>(argss))... };
    std::stringstream out;
    int size = str.size();
    int args_size = args.size();
    bool in = false;
    bool need_end = false;
    for(int i = 0; i < size; i++) {
        if(need_end) {
            if(str[i] == '}') {
                need_end = false;
                continue;
            } else {
                throw std::runtime_error(" ");
            }
        }
        if(in) {
            if(std::isdigit(str[i])) {
                if(str[i] - '0' < args_size) {
                    out << args[str[i] - '0'];
                    need_end = true;
                    in = false;
                } else {
                    throw std::runtime_error(" ");
                }
            } else {
                throw std::runtime_error(" ");
            }
        } else {
            if(str[i] == '{') {
                in = true;
            } else  if(str[i] == '}'){
                throw std::runtime_error(" ");
            } else {
                out << str[i];
            }
        }
    }
    if(in || need_end) {
        throw std::runtime_error(" ");
    }
    return out.str();
}