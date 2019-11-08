#include "allocator.h"

LinearAllocator::LinearAllocator(size_t maxSize) : max_size(maxSize), cur_size(0){
    memory = new char[max_size];
}
char* LinearAllocator::alloc(size_t size) {
    if(size + cur_size > max_size) {
        return nullptr;
    }
    char* temp = memory + cur_size;
    cur_size += size;
    return temp;
}
void LinearAllocator::reset(){
    cur_size = 0;
}
LinearAllocator::~LinearAllocator() {
    delete[] memory;
}

