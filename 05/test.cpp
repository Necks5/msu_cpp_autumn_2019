#include "serializer.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class T>
    Error serialize(T& serializer)
    {
        return serializer(a, b, c);
    }

};
void test1() {
    

    Data x { 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    serializer.save(x);

    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    assert(err == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
}
int main() {
    test1();
    return 0;
}