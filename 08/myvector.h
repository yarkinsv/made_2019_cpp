#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

template <class T>
class Allocator {
public:
    T* allocate(size_t n) {
        return reinterpret_cast<T*>(malloc(sizeof(T) * n));
    }

    void deallocate(T* p, size_t n) {
        free(p);
    }
};

template <class T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
    T* current_;
    bool reverse_;
public:
    Iterator(T* current, bool reverse) : current_(current), reverse_(reverse) { }

    bool operator==(const Iterator& other) const {
        return current_ == other.current_;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

    Iterator& operator++() {
        if (reverse_) {
            current_--;
        } else {
            current_++;
        }
        return *this;
    }

    Iterator& operator+(int n) {
        if (reverse_) {
            current_ -= n;
        } else {
            current_ += n;
        }
        return *this;
    }

    T operator*() const {
        return *current_;
    }

    T operator[](int n) {
        return *(this + n);
    }
};

template <class T, class Alloc = std::allocator>
class Vector {
public:
    using iterator = Iterator<T>;
    using size_type = size_t;
    using value_type = T;

    Vector(size_type count) : size_(count), buffer_(alloc_.allocate(count)), cur_(count) {
        for (size_type i = 0; i < count; i++) {
            new (buffer_ + i) value_type();
        }
    }

    Vector(std::initializer_list<T> init) : size_(init.size()), buffer_(alloc_.allocate(init.size())), cur_(init.size()) {
        int i = 0;
        for (T value : init) {
            buffer_[i++] = value;
        }
    }

    value_type& operator[](int n) {
        if (n < 0 || n >= cur_) {
            throw std::invalid_argument("Index out of range");
        }
        return buffer_[n];
    }

    void push_back(const T& value) {
        expand();
        buffer_[cur_++] = value;
    }

    void push_back(T&& value) {
        expand();
        buffer_[cur_++] = std::move(value);
    }

    void pop_back() {
        if (cur_ > 0) cur_--;
    }

    bool empty() const noexcept {
        return begin() == end();
    }

    size_type size() const noexcept {
        return size_;
    }

    void clear() noexcept {
        size_ = 0;
    }

    void resize(size_type count) {
        if (count == size_) {
            return;
        }
        value_type * new_buf = alloc_.allocate(count);

        for (size_type i = 0; i < std::min(cur_, count); i++) {
            new (new_buf + i) value_type(buffer_[i]);
        }

        for (size_type i = cur_; i < count; i++) {
            new (new_buf + i) value_type();
        }

        alloc_.deallocate(buffer_, size_);
        size_ = count;
        buffer_ = new_buf;
    }

    void reserve(size_type new_cap) {
        if (new_cap <= size_) {
            return;
        }
        resize(new_cap);
    }

    iterator begin() noexcept {
        return Iterator(buffer_, false);
    }

    iterator rbegin() noexcept {
        return Iterator(buffer_ + cur_ - 1, true);
    }

    iterator end() noexcept {
        return Iterator(buffer_ + cur_, false);
    }

    iterator rend() noexcept {
        return Iterator(buffer_ - 1, true);
    }

private:
    void expand() {
        if (cur_ == size_) {
            resize(size_ * 2);
        }
    }

    Alloc alloc_;
    value_type * buffer_;
    size_type size_;
    size_type cur_;
};
