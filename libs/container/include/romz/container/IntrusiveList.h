#pragma once

#include <cstdint>
#include <cassert>

namespace romz {

template<typename T, std::size_t I = 0>
struct IntrusiveList {
    IntrusiveList() {
        clear();
    }

    T *head() const {
        return head_;
    }

    T *tail() const {
        return tail_;
    }

    bool is_empty() const {
        return size_ == 0;
    }

    std::size_t size() const {
        return size_;
    }

    void put(T *t) {
        t->list_node.previous[I] = nullptr;
        t->list_node.next[I] = nullptr;
        if (head_) {
            t->list_node.next[I] = head_;
            head_->list_node.previous[I] = t;
        }
        head_ = t;
        if (!tail_)
            tail_ = t;
        size_++;
    }

    void append(T *t) {
        t->list_node.previous[I] = 0;
        t->list_node.next[I] = 0;
        if (!head_) {
            assert(tail_ == 0);
            head_ = t;
            tail_ = t;
        }
        else {
            tail_->list_node.next[I] = t;
            tail_ = t;
            if (!head_)
                head_ = t;
        }
        size_++;
    }


    void del(T *t) {
        assert(has(t));

        if (t == tail_)
            tail_ = t->list_node.previous[I];
        if (t == head_) {
            T *next = head_->list_node.next[I];
            if (next)
                next->list_node.previous[I] = nullptr;
            head_ = next;
        }
        else {
            T *next = t->list_node.next[I];
            T *prev = t->list_node.previous[I];
            if (prev)
                prev->list_node.next[I] = next;
            if (next)
                next->list_node.previous[I] = prev;
        }
        t->list_node.next[I] = nullptr;
        t->list_node.previous[I] = nullptr;
        size_--;
    }

    bool has(const T *t) const {
        return t->list_node.previous[I] != nullptr
                || t->list_node.next[I] != nullptr
                || t == head_;
    }

    void clear() {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    T *head_;
    T *tail_;
    std::size_t size_;
};

}
