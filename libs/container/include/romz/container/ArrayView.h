#pragma once

#include <cstddef>

namespace romz {

template<typename T>
struct ArrayView
{
    ArrayView()
        : data(nullptr)
        , size(0) {
    }

    ArrayView(T *data_, size_t size_)
        : data(data_)
        , size(size_) {
    }

    ArrayView(const ArrayView &other)
        : data(other.data)
        , size(other.size) {
    }

    T &operator[]( std::size_t index) {
        return data[index];
    }

    const T &operator[]( std::size_t index) const {
        return data[index];
    }

    // Pointer to the data
    T *data;

    // The number of elements in the array
    std::size_t size;
};

}
