template <class T>
class Allocator {
};

template <class T>
class Iterator {
};

template <class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;

private:
    Alloc alloc_;
};
