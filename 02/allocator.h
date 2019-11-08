#include <cstddef>

class LinearAllocator {
	size_t max_size, cur_size;
    char* memory;
public:
	LinearAllocator(size_t maxSize);
	char* alloc(size_t size);
	void reset();
	~LinearAllocator();
};