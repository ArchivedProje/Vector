// Copyright byteihq 2021 <kotov038@gmail.com>

#ifndef VECTOR_REVERSE_ITERATOR_H
#define VECTOR_REVERSE_ITERATOR_H

#include <iterator>

template<typename T>

class Reverse_iterator : public std::iterator<std::random_access_iterator_tag, T> {
private:
    T *it_;

public:
    explicit Reverse_iterator(T *it) : it_(it) {}

    Reverse_iterator(const Reverse_iterator &it) : it_(it.it_) {}

    Reverse_iterator &operator=(const Iterator<T> &rhs) {
        if (this != &rhs) {
            it_ = rhs.it_;
        }
        return *this;
    }

    inline constexpr Reverse_iterator &operator++() noexcept {
        --it_;
        return *this;
    }

    inline constexpr Reverse_iterator operator++(int) noexcept {
        Iterator<T> old(it_);
        --it_;
        return old;
    }

    inline constexpr Reverse_iterator &operator--() noexcept {
        ++it_;
        return *this;
    }

    inline constexpr Reverse_iterator &operator+=(size_t value) noexcept {
        it_ -= value;
        return *this;
    }

    inline constexpr Reverse_iterator &operator-=(size_t value) noexcept {
        it_ -= value;
        return *this;
    }

    inline constexpr Iterator<T> base() const {
        Iterator<T> it(it_);
        return ++it;
    }

    inline constexpr typename Reverse_iterator::reference operator[](size_t index) {
        return *base()[-index - 1];
    }

    inline constexpr typename Reverse_iterator::reference operator[](size_t index) const {
        return *base()[-index - 1];
    }

    inline constexpr typename Reverse_iterator::reference operator*() const { return *base(); }

    inline constexpr typename Reverse_iterator::reference operator*() { return *base(); }

    inline constexpr typename Reverse_iterator::pointer operator->() const { return base(); }

    inline constexpr bool operator<(const Reverse_iterator<T> &rhs) const noexcept {
        return base() < rhs.base();
    }

    inline constexpr bool operator==(const Reverse_iterator<T> &rhs) const noexcept {
        return base() == rhs.base();
    }
};

template<typename T>
inline constexpr auto
operator-(const Reverse_iterator<T> &lhs, const Reverse_iterator<T> &rhs) -> decltype(rhs.base() - lhs.base()) {
    return rhs.base() - lhs.base();
}

template<typename T>
inline constexpr bool operator<=(const Reverse_iterator<T> &lhs, const Reverse_iterator<T> &rhs) {
    return !(lhs > rhs);
}

template<typename T>
inline constexpr bool operator!=(const Reverse_iterator<T> &lhs, const Reverse_iterator<T> &rhs) {
    return !(lhs == rhs);
}

template<typename T>
inline constexpr Reverse_iterator<T> operator+(const Reverse_iterator<T> &lhs, const size_t value) {
    return Reverse_iterator<T>(lhs.base() - value);
}

template<typename T>
inline constexpr bool operator>(const Reverse_iterator<T> &lhs, const Reverse_iterator<T> &rhs) {
    return rhs < lhs;
}

template<typename T>
inline constexpr bool operator>=(const Reverse_iterator<T> &lhs, const Reverse_iterator<T> &rhs) {
    return rhs <= lhs;
}

#endif //VECTOR_REVERSE_ITERATOR_H
