#include <iostream>


using BigIntBufferType = uint64_t;

template <int n>
struct max_v;

template <>
struct max_v<0> {
    static constexpr BigIntBufferType value = 1;
};

// Сами нас этому научили, вот решил попробовать )
template <int n>
struct max_v {
    static constexpr BigIntBufferType value = 10 * max_v<n - 1>::value;
};

const BigIntBufferType BIG_INT_MAX_BUFFER_VALUE = max_v<std::numeric_limits<BigIntBufferType>::digits10 - 2>::value;

class BigInt {
public:
    BigInt() : buffer(new BigIntBufferType[1]), size(0), sign(true) {}
    BigInt(const BigIntBufferType& value) : sign(value >= 0) {
        if (value >= BIG_INT_MAX_BUFFER_VALUE) {
            buffer = new BigIntBufferType[2] {value % BIG_INT_MAX_BUFFER_VALUE, value / BIG_INT_MAX_BUFFER_VALUE};
            size = 2;
        } else {
            buffer = new BigIntBufferType[1] {value};
            size = 1;
        }
    }
    BigInt(const int& value) : buffer(new uint64_t[1] {static_cast<uint64_t>(abs(value))}), size(1), sign(value >= 0) {}

    BigInt(const BigInt& value) : buffer(new BigIntBufferType[value.size]), size(value.size), sign(value.sign) {
        memcpy(buffer, value.buffer, sizeof(BigIntBufferType) * value.size);
    }

    BigInt(BigIntBufferType* buffer, int size, bool sign) : buffer(buffer), size(size), sign(sign) {}

    BigInt operator+(const BigInt& rhs);

    BigInt operator-() const;

    BigInt operator-(const BigInt& rhs);

    bool operator==(const BigInt& rhs) const;

    bool operator!=(const BigInt& rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const BigInt& rhs) const;

    bool operator>(const BigInt& rhs) const {
        return rhs < *this;
    }

    bool operator<=(const BigInt& rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const BigInt& rhs) const {
        return !(*this < rhs);
    }

    friend std::ostream &operator<<(std::ostream& os, const BigInt& anInt) {
        if (!anInt.sign) {
            os << '-';
        }
        for (size_t i = 0; i < anInt.size; i++) {
            size_t index = anInt.size - i - 1;
            if (index < anInt.size - 1) {
                BigIntBufferType leading_zero = BIG_INT_MAX_BUFFER_VALUE / 10;
                while (leading_zero > anInt.buffer[index]) {
                    os << '0';
                    leading_zero /= 10;
                }
            }
            os << anInt.buffer[index];
        }
        return os;
    }

    ~BigInt() {
        delete[] buffer;
    }

private:
    BigIntBufferType* buffer;
    // Знак числа, 1 - положительное, 0 - отрицательное
    bool sign;
    // Размер выделленого буфера
    size_t size;
};
