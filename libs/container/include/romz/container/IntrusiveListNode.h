#pragma once

#include <cstdint>

namespace romz {

template<typename T, std::size_t I = 1>
struct IntrusiveListNode  {
  IntrusiveListNode() {
    for (std::size_t i = 0; i < I; i++)
      previous[i] = next[i] = 0;
  }

  IntrusiveListNode(const IntrusiveListNode& ) = delete;

  T *previous[I];
  T *next[I];
};

}
