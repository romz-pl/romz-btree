#pragma once

///
/// A class managing a dynamically sized array for arbitrary types
///
///
/// The DynamicArray class is a dynamic, resizable array. The internal memory
/// is released when the DynamicArray instance is destructed.
///
/// Unlike std::vector, the DynamicArray uses libc functions for constructing,
/// copying and initializing elements.
///


#include <romz/memory/Memory.h>
#include <cstdlib>
#include <cstdint>
#include <cstring>

namespace romz {

template<typename T>
struct DynamicArray {
    typedef T value_t;
    typedef T *pointer_t;

    DynamicArray(std::size_t size = 0)
        : _ptr(nullptr), _size(0), _own(true) {
        resize(size);
    }

    DynamicArray(std::size_t size, std::uint8_t fill_byte)
        : _ptr(nullptr), _size(0), _own(true) {
        resize(size);
        if (_ptr)
            std::memset(_ptr, fill_byte, sizeof(T) * _size);
    }

    ~DynamicArray() {
        clear();
    }

    void steal_from(DynamicArray &other) {
        clear();
        *this = other;
        other.disown();
        other.clear();
    }

    size_t append(const T *ptr, std::size_t size) {
        std::size_t old_size = _size;
        T *p = resize(_size + size);
        std::memcpy(p + old_size, ptr, sizeof(T) * size);
        return old_size;
    }

    void copy(const T *ptr, std::size_t size) {
        resize(size);
        std::memcpy(_ptr, ptr, sizeof(T) * size);
        _size = size;
    }

    void overwrite(std::uint32_t position, const T *ptr, std::size_t size) {
        std::memcpy(_ptr + position, ptr, sizeof(T) * size);
    }

    T *resize(std::size_t size) {
        if (size > _size) {
            _ptr = Memory::reallocate<T>(_ptr, sizeof(T) * size);
            _size = size;
        }
        return _ptr;
    }

    T *resize(std::size_t size, std::uint8_t fill_byte) {
        resize(size);
        if (_ptr)
            std::memset(_ptr, fill_byte, sizeof(T) * size);
        return _ptr;
    }

    std::size_t size() const {
        return _size;
    }

    void set_size(std::size_t size) {
        _size = size;
    }

    T *data() {
        return _ptr;
    }

    const T *data() const {
        return _ptr;
    }

    void assign(T *ptr, std::size_t size) {
        clear();
        _ptr = ptr;
        _size = size;
    }

    void clear(bool release_memory = true) {
        if (_ptr && _own && release_memory)
            Memory::release(_ptr);
        _ptr = nullptr;
        _size = 0;
    }

    bool is_empty() const {
        return _size == 0;
    }

    void disown() {
        _own = false;
    }

    // Pointer to the data
    T *_ptr;

    // The size of the array
    std::size_t _size;

    // True if the destructor should free the pointer
    bool _own;
};


}
