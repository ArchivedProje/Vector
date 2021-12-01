// Copyright byteihq 2021 <kotov038@gmail.com>

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <memory>

#include <Iterator.h>
#include <Reverse_iterator.h>

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
private:
    T *data_;
    size_t size_;
    size_t capacity_;
    Allocator allocator_;
    using AllocTraits = std::allocator_traits<Allocator>;

    void try_construct(size_t start, size_t end, size_t arr_size, T *lhs, T *rhs) {
        size_t i = start;
        try {
            for (; i < end; ++i) {
                AllocTraits::construct(allocator_, lhs + i, rhs[i]);
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                AllocTraits::destroy(allocator_, lhs + j);
            }
            AllocTraits::deallocate(allocator_, lhs, arr_size);
            throw;
        }
    }

    void try_construct(size_t start, size_t end, size_t arr_size, T *lhs, const T &value) {
        size_t i = start;
        try {
            for (; i < end; ++i) {
                AllocTraits::construct(allocator_, lhs + i, value);
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                AllocTraits::destroy(allocator_, lhs + j);
            }
            AllocTraits::deallocate(allocator_, lhs, arr_size);
            throw;
        }
    }

    void try_construct(size_t start, size_t end, size_t arr_size, T *lhs, T &&value) {
        size_t i = start;
        try {
            for (; i < end; ++i) {
                AllocTraits::construct(allocator_, lhs + i, std::move(value));
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                AllocTraits::destroy(allocator_, lhs + j);
            }
            AllocTraits::deallocate(allocator_, lhs, arr_size);
            throw;
        }
    }

public:
    using iterator = Iterator<T>;

    using const_iterator = Iterator<const T>;

    using reverse_iterator = Reverse_iterator<T>;

    using const_reverse_iterator = Reverse_iterator<const T>;

    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    Vector(size_t _size, const T &value = T(), const Allocator &allocator = Allocator())
            : size_(_size), capacity_(_size), allocator_(allocator) {
        data_ = AllocTraits::allocate(allocator_, size_);
        try_construct(0, size_, capacity_, data_, value);
    }

    Vector(size_t _size, const T *_data)
            : data_(AllocTraits::allocate(allocator_, _size)), size_(_size), capacity_(_size) {
        std::copy(_data, _data + _size, data_);
    }

    Vector(const Vector &rhs)
            : size_(rhs.size_),
              capacity_(rhs.capacity_),
              allocator_(rhs.allocator_) {
        data_ = AllocTraits::allocate(allocator_, capacity_);
        try_construct(0, size_, capacity_, data_, rhs.data_);
    }

    Vector(Vector &&rhs) noexcept: data_(rhs.data_), size_(rhs.size_), capacity_(rhs.capacity_) {
        rhs.data_ = nullptr;
        rhs.size_ = 0;
        rhs.capacity_ = 0;
    }

    ~Vector() {
        for (size_t i = 0; i < size_; ++i) {
            pop_back();
        }
        AllocTraits::deallocate(allocator_, data_, capacity_);
    }

    Vector &operator=(const Vector &rhs) {
        if (this != &rhs) {
            ~Vector();
            if (AllocTraits::propagate_on_container_copy_assignment::value
                && allocator_ != rhs.allocator_) {
                allocator_ = rhs.allocator_;
            }
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
            data_ = AllocTraits::allocate(allocator_, capacity_);
            try_construct(0, size_, capacity_, data_, rhs.data_);
        }
        return *this;
    }

    Vector& operator=(Vector&& rhs)  noexcept {
        if (this != &rhs) {
            ~Vector();
            if (AllocTraits::propagate_on_container_copy_assignment::value
                && allocator_ != rhs.allocator_) {
                allocator_ = rhs.allocator_;
            }
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
            data_ = AllocTraits::allocate(allocator_, capacity_);
            try_construct(0, size_, capacity_, data_, std::move(rhs.data_));
        }
        return *this;
    }

    const T &operator[](size_t index) const { return data_[index]; }

    T &operator[](size_t index) { return data_[index]; }

    const T &at(size_t index) const {
        if (index < size_) {
            return data_[index];
        }
        throw std::out_of_range("Index out of range");
    }

    T &at(size_t index) {
        return const_cast<T &>(static_cast<const Vector<T> &>(*this).at(index));
    }

    const T &front() const { return data_[0]; }

    T &front() { return data_[0]; }

    const T &back() const { return data_[size_ - 1]; }

    T &back() { return data_[size_ - 1]; }

    const T *data() const { return data_; }

    T *data() { return data_; }

    size_t size() const noexcept { return size_; }

    bool empty() const noexcept { return size_ == 0; }

    size_t capacity() const noexcept { return capacity_; }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            AllocTraits::destroy(allocator_, data_ + i);
        }
        size_ = 0;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        T *tmp = AllocTraits::allocate(allocator_, new_capacity);
        try_construct(0, size_, new_capacity, tmp, data_);
        for (size_t i = 0; i < size_; ++i) {
            AllocTraits::destroy(allocator_, data_ + i);
        }
        AllocTraits::deallocate(allocator_, data_, capacity_);
        data_ = tmp;
        capacity_ = new_capacity;
    }

    constexpr iterator begin() noexcept { return iterator(data_); }

    constexpr iterator end() noexcept { return iterator(data_ + size_); }

    constexpr const_iterator cbegin() const noexcept { return const_iterator(data_); }

    constexpr const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(data_ + size_ - 1); }

    constexpr reverse_iterator rend() noexcept { return reverse_iterator(data_ - 1); }

    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(data_); }

    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(data_ + size_); }

    iterator insert(iterator &position, const T &value) {
        if (position >= Iterator(data_ + size_)) {
            throw std::out_of_range("Iterator out of range");
        }
        if (capacity_ == size_) {
            if (capacity_ == 0) ++capacity_;
            capacity_ *= 2;
        }
        T *tmp = AllocTraits::allocate(allocator_, capacity_);
        auto distance = static_cast<size_t>(std::distance(begin(), position));
        try_construct(0, distance, capacity_, tmp, data_);
        AllocTraits::construct(allocator_, tmp + distance, value);
        try_construct(distance + 1, size_, capacity_, tmp, data_);
        for (size_t i = 0; i < size_; ++i) {
            AllocTraits::destroy(allocator_, data_ + i);
        }
        AllocTraits::deallocate(allocator_, data_, capacity_);
        data_ = tmp;
        ++size_;
        return Iterator(data_ + distance);
    }

    constexpr void push_back(const T &value) {
        if (capacity_ == size_) {
            reserve(2 * capacity_);
        }
        AllocTraits::construct(allocator_, data_ + size_, value);
        ++size_;
    }

    constexpr void push_back(T &&value) {
        if (capacity_ == size_) {
            if (capacity_ == 0) ++capacity_;
            reserve(2 * capacity_);
        }
        AllocTraits::construct(allocator_, data_ + size_, std::move(value));
        ++size_;
    }

    template<typename ... Args>
    constexpr void emplace_back(Args &&... value) {
        push_back(T(std::forward<Args>(value)...));
    }

    constexpr void pop_back() {
        AllocTraits::destroy(allocator_, data_ + size_ - 1);
        --size_;
    }

    constexpr void swap(Vector<T> &rhs) {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
    }

    constexpr bool operator==(const Vector<T> &rhs) const {
        if (size_ != rhs.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != rhs.data_[i]) {
                return false;
            }
        }
        return true;
    }
};

template<typename T>
constexpr bool operator!=(const Vector<T> &lhs, const Vector<T> &rhs) {
    return !(lhs == rhs);
}

#endif //VECTOR_VECTOR_H
