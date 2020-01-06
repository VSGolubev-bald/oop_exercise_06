
#ifndef LAB6_MY_ALLOCATOR_H
#define LAB6_MY_ALLOCATOR_H

#include <cstdlib>
#include <cstdint>
#include <memory>
#include "queue.h"

template <typename T, size_t ALLOC_SIZE>
class my_allocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using is_always_equal = std::false_type;

    my_allocator(const my_allocator&) = delete;
    my_allocator(my_allocator&&) = delete;

    template<class U>
    struct rebind {
        using other = my_allocator<U, ALLOC_SIZE>;
    };

    my_allocator() {
        size_t object_count = ALLOC_SIZE / sizeof(T);
        memory = reinterpret_cast<char*>(operator new(sizeof(T) * object_count));
        for (size_t i = 0; i < object_count; ++i) {
            free_blocks.push(memory + sizeof(T) * i);
        }
    }

    ~my_allocator() {
        operator delete(memory);
    }

    T* allocate(size_t size) {
        if (size > 1) {
            throw std::logic_error("no way to allocate");
        }
        if (free_blocks.empty()) {
            throw std::bad_alloc();
        }
        T* temp = reinterpret_cast<T*>(free_blocks.top());
        free_blocks.pop();
        return temp;

    }

    void deallocate(T* ptr, size_t size) {
        if (size != 1) {
            throw std::logic_error("Can't do that");
        }
        free_blocks.push(reinterpret_cast<char*>(ptr));
    }

private:
    cntr::queue<char*> free_blocks;
    char* memory;
};
#endif //LAB6_MY_ALLOCATOR_H
