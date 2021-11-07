// Copyright byteihq 2021 <kotov038@gmail.com>

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <Iterator.h>
#include <Reverse_iterator.h>

template<class T>
class Vector {
private:
    T *data_;
    size_t size_;
    size_t capacity_;

    void update_capacity() {
        if (capacity_ == 0) {
            capacity_ = 1;
        } else {
            capacity_ *= 2;
        }
    }

public:
    using iterator = Iterator<T>;

    using const_iterator = Iterator<const T>;

    using reverse_iterator = Reverse_iterator<T>;

    using const_reverse_iterator = Reverse_iterator<const T>;

    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(size_t _size)
            : data_(new T[_size]), size_(_size), capacity_(_size) {}

    Vector(size_t _size, const T &value)
            : data_(new T[_size]), size_(_size), capacity_(_size) {
        std::fill(data_, data_ + size_, value);
    }

    Vector(size_t _size, const T *_data)
            : data_(new T[_size]), size_(_size), capacity_(_size) {
        std::copy(_data, _data + _size, data_);
    }

    Vector(const Vector &rhs)
            : data_(new T[rhs.capacity_]),
              size_(rhs.size_),
              capacity_(rhs.capacity_) {
        std::copy(rhs.data_, rhs.data_ + rhs.size_, data_);
    }

    Vector(Vector &&rhs) noexcept: data_(rhs.data_), size_(rhs.size_), capacity_(rhs.capacity_) {
        rhs.data_ = nullptr;
        rhs.size_ = 0;
        rhs.capacity_ = 0;
    }

    ~Vector() { delete[] data_; }

    Vector &operator=(const Vector &rhs) {
        if (this != &rhs) {
            delete[] data_;
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
            data_ = new T[capacity_];
            std::copy(rhs.data_, rhs.data_ + rhs.size_, data_);
        }
        return *this;
    }

    const T &operator[](const size_t index) const { return data_[index]; }

    T &operator[](const size_t index) { return data_[index]; }

    const T &at(const size_t index) const {
        if (index < size_) {
            return data_[index];
        }
        throw std::out_of_range("Index out of range");
    }

    T &at(const size_t index) {
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
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    void reserve(const size_t new_capacity) {
        if (new_capacity > capacity_) {
            T *tmp = new T[size_];
            std::copy(data_, data_ + size_, tmp);
            delete[] data_;
            data_ = new T[new_capacity];
            capacity_ = new_capacity;
            std::copy(tmp, tmp + size_, data_);
            delete[] tmp;
        }
    }

    constexpr iterator begin() noexcept { return iterator(data_); }

    constexpr iterator end() noexcept { return iterator(data_ + size_); }

    constexpr const_iterator cbegin() const noexcept { return const_iterator(data_); }

    constexpr const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

    constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(data_ + size_ - 1); }

    constexpr reverse_iterator rend() noexcept { return reverse_iterator(data_ - 1); }

    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(data_); }

    constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(data_ + size_); }

    constexpr iterator insert(iterator &position, const T &value) {
        if (position >= Iterator(data_ + size_)) {
            throw std::out_of_range("Iterator out of range");
        }
        if (capacity_ == size_) {
            update_capacity();
        }
        T *tmp = new T[capacity_];
        auto distance = std::distance(begin(), position);
        std::copy(data_, data_ + distance, tmp);
        tmp[distance] = value;
        std::copy(data_ + distance, data_ + size_, tmp + distance + 1);
        ++size_;
        delete[] data_;
        data_ = new T[capacity_];
        std::copy(tmp, tmp + size_, data_);
        delete[] tmp;
        return Iterator(data_ + distance);
    }

    constexpr void push_back(const T &value) {
        if (capacity_ == size_) {
            update_capacity();
            T *tmp = new T[size_];
            std::copy(data_, data_ + size_, tmp);
            delete[] data_;
            data_ = new T[capacity_];
            std::copy(tmp, tmp + size_, data_);
            delete[] tmp;
        }
        data_[size_] = value;
        ++size_;
    }

    constexpr void push_back(T&& value) {
        if (capacity_ == size_) {
            update_capacity();
            T *tmp = new T[size_];
            std::copy(data_, data_ + size_, tmp);
            delete[] data_;
            data_ = new T[capacity_];
            std::copy(tmp, tmp + size_, data_);
            delete[] tmp;
        }
        data_[size_] = std::move(value);
        ++size_;
    }

    template <typename ... Args>
    constexpr void emplace_back(Args&&... value) {
        push_back(T(std::forward<Args>(value)...));
    }

    constexpr void pop_back() { --size_; }

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
