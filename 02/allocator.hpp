#include <cstddef>

class LinearAllocator
{
public:
    LinearAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();

    ~LinearAllocator();

private:
    char* memory_buffer;
    size_t current_position;
    size_t memory_left;
};
