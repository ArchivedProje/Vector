// Copyright byteihq 2021 <kotov038@gmail.com>

#ifndef VECTOR_ITERATOR_H
#define VECTOR_ITERATOR_H

#include <iterator>

template<typename T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
private:
    T *it_;

public:
    explicit Iterator(T *it) : it_(it) {}

    Iterator(const Iterator &it) : it_(it.it_) {}

    Iterator &operator=(const Iterator<T> &rhs) {
        if (this != &rhs) {
            it_ = rhs.it_;
        }
        return *this;
    }

    inline constexpr Iterator &operator++() noexcept {
        ++it_;
        return *this;
    }

    inline constexpr Iterator operator++(int) noexcept {
        Iterator<T> old(it_);
        ++it_;
        return old;
    }

    inline constexpr Iterator &operator--() noexcept {
        --it_;
        return *this;
    }

    inline constexpr Iterator &operator+=(size_t value) noexcept {
        it_ += value;
        return *this;
    }

    inline constexpr Iterator &operator-=(size_t value) noexcept {
        it_ -= value;
        return *this;
    }

    inline constexpr typename Iterator::difference_type  operator-(const Iterator &rhs) { return it_ - rhs.it_; }

    inline constexpr typename Iterator::reference operator[](size_t index) {
        return *(it_ + index);
    }

    inline constexpr typename Iterator::reference operator[](size_t index) const {
        return *(it_ + index);
    }

    inline constexpr typename Iterator::reference operator*() const { return *it_; }

    inline constexpr typename Iterator::reference operator*() { return *it_; }

    inline constexpr typename Iterator::pointer operator->() const { return it_; }

    inline constexpr bool operator<(const Iterator<T> &rhs) const noexcept {
        return it_ < rhs.it_;
    }

    inline constexpr bool operator==(const Iterator<T> &rhs) const noexcept {
        return it_ == rhs.it_;
    }
};

template<typename T>
inline constexpr bool operator<=(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return !(lhs > rhs);
}

template<typename T>
inline constexpr bool operator!=(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T>
inline constexpr Iterator<T> operator+(const Iterator<T> &lhs, const size_t value) {
    Iterator<T> tmp(lhs);
    tmp += value;
    return tmp;
}

template<typename T>
inline constexpr Iterator<T> operator-(const Iterator<T> &lhs, const size_t value) {
    Iterator<T> tmp(lhs);
    tmp -= value;
    return tmp;
}

template<typename T>
inline constexpr bool operator>(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return rhs < lhs;
}

template<typename T>
inline constexpr bool operator>=(const Iterator<T> &lhs, const Iterator<T> &rhs) {
    return rhs <= lhs;
}

#endif //VECTOR_ITERATOR_H
