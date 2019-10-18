#include <cstddef>
#include <cstdlib>
#include "allocator.hpp"

LinearAllocator::LinearAllocator(size_t maxSize) {
    memory_buffer = (char*) malloc(maxSize);
    memory_left = maxSize;
    current_position = 0;
}

char* LinearAllocator::alloc(size_t size) {
    if (size > memory_left) {
        return nullptr;
    }
    memory_left -= size;
    current_position += size;
    return memory_buffer + current_position - size;
}

void LinearAllocator::reset() {
    memory_left += current_position;
    current_position = 0;
}

LinearAllocator::~LinearAllocator() {
    free(memory_buffer);
}
