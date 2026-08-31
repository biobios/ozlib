#pragma once

#include <bits/namespace.hpp>

#include <bits/allocator.hpp>
#include <bits/allocator_traits.hpp>
#include <bits/move.hpp>

namespace OZLIB_NAMESPACE {
namespace impl {
template <typename T, typename Allocator = allocator<T>>
class vector {
T* data_;
size_t size_;
size_t capacity_;
[[no_unique_address]]Allocator allocator_;
using allocator_traits = allocator_traits<Allocator>;
public:
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = T*;
    using const_pointer = const T*;

    vector() : data_(nullptr), size_(0), capacity_(0), allocator_() {}
    ~vector() {
        if (data_) {
            for (size_t i = 0; i < size_; ++i) {
                allocator_traits::destroy(allocator_, data_ + i);
            }
            allocator_traits::deallocate(allocator_, data_, capacity_);
        }
    }

    iterator begin() { return data_; }
    const_iterator begin() const { return data_; }
    iterator end() { return data_ + size_; }
    const_iterator end() const { return data_ + size_; }
    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    constexpr void push_back(const T& value) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            T* new_data = allocator_traits::allocate(allocator_, new_capacity);
            for (size_t i = 0; i < size_; ++i) {
                allocator_traits::construct(allocator_, new_data + i, move(data_[i]));
                allocator_traits::destroy(allocator_, data_ + i);
            }
            if (data_) {
                allocator_traits::deallocate(allocator_, data_, capacity_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
        allocator_traits::construct(allocator_, data_ + size_, value);
        ++size_;
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            T* new_data = allocator_traits::allocate(allocator_, new_capacity);
            for (size_t i = 0; i < size_; ++i) {
                allocator_traits::construct(allocator_, new_data + i, move(data_[i]));
                allocator_traits::destroy(allocator_, data_ + i);
            }
            if (data_) {
                allocator_traits::deallocate(allocator_, data_, capacity_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
        allocator_traits::construct(allocator_, data_ + size_, forward<Args>(args)...);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            allocator_traits::destroy(allocator_, data_ + size_);
        }
    }

    constexpr iterator erase(const_iterator pos) {
        size_type index = pos - data_;
        if (index < size_) {
            allocator_traits::destroy(allocator_, data_ + index);
            for (size_type i = index; i < size_ - 1; ++i) {
                allocator_traits::construct(allocator_, data_ + i, move(data_[i + 1]));
                allocator_traits::destroy(allocator_, data_ + i + 1);
            }
            --size_;
        }
        return data_ + index;
    }

    constexpr void clear() {
        for (size_t i = 0; i < size_; ++i) {
            allocator_traits::destroy(allocator_, data_ + i);
        }
        size_ = 0;
    }

    value_type& operator[](size_type index) {
        return data_[index];
    }

    const value_type& operator[](size_type index) const {
        return data_[index];
    }
};

}
}