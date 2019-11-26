#include <iostream>
#include "bigint.hpp"


BigInt BigInt::operator+(const BigInt& rhs) {
    // Тут наверно неэффективно в плане копирования, но хз как лучше
    if (sign != rhs.sign) {
        if (sign) {
            return *this - -rhs;
        } else {
            return -(*this - -rhs);
        }
    }

    size_t new_size = std::max(size, rhs.size);
    if (size == rhs.size && buffer[size - 1] + rhs.buffer[size - 1] >= BIG_INT_MAX_BUFFER_VALUE) new_size++;
    auto new_buffer = new BigIntBufferType[new_size];
    BigIntBufferType carry = 0;
    for (int i = 0; i < std::max(size, rhs.size); i++) {
        new_buffer[i] = (i < size ? buffer[i] : 0) + (i < rhs.size ? rhs.buffer[i] : 0) + carry;
        if (new_buffer[i] >= BIG_INT_MAX_BUFFER_VALUE) {
            carry = new_buffer[i] / BIG_INT_MAX_BUFFER_VALUE;
            new_buffer[i] = new_buffer[i] % BIG_INT_MAX_BUFFER_VALUE;
        } else {
            carry = 0;
        }
    }
    if (carry > 0) {
        new_buffer[new_size - 1] = carry;
    }
    return BigInt(new_buffer, new_size, true);
}

BigInt BigInt::operator-() const {
    BigInt result = BigInt(*this);
    result.sign = !result.sign;
    return result;
}

BigInt BigInt::operator-(const BigInt& rhs) {
    if (sign != rhs.sign) {
        if (sign) {
            return *this + -rhs;
        } else {
            return -(-*this + rhs);
        }
    }
    if (*this == rhs) {
        return BigInt(0);
    }
    size_t new_size = std::max(size, rhs.size);
    auto new_buffer = new BigIntBufferType[new_size];
    bool new_sign = true;

    BigIntBufferType* left_buffer;
    BigIntBufferType* right_buffer;

    if (sign) {
        left_buffer = (*this > rhs ? *this : rhs).buffer;
        right_buffer = (*this < rhs ? *this : rhs).buffer;
        new_sign = *this > rhs;
    } else {
        left_buffer = (*this < rhs ? *this : rhs).buffer;
        right_buffer = (*this > rhs ? *this : rhs).buffer;
        new_sign = *this < rhs;
    }
    BigIntBufferType carry = 0;
    for (int i = 0; i < std::max(size, rhs.size); i++) {
        if (left_buffer[i] >= right_buffer[i] + carry) {
            new_buffer[i] = left_buffer[i] - (i < std::min(size, rhs.size) ? right_buffer[i] : 0) - carry;
            carry = 0;
        } else {
            new_buffer[i] = BIG_INT_MAX_BUFFER_VALUE - (i < std::min(size, rhs.size) ? right_buffer[i] : BIG_INT_MAX_BUFFER_VALUE) + left_buffer[i] - carry;
            carry = 1;
        }
    }

    if (new_buffer[new_size - 1] == 0) {
        while (new_buffer[new_size - 1] == 0) {
            new_size--;
        }
        auto tmp_buffer = new BigIntBufferType[new_size];
        memcpy(tmp_buffer, new_buffer, sizeof(BigIntBufferType) * new_size);
        delete[] new_buffer;
        new_buffer = tmp_buffer;
    }

    return BigInt(new_buffer, new_size, new_sign);;
}

bool BigInt::operator==(const BigInt& rhs) const {
    if (sign != rhs.sign || size != rhs.size) return false;
    for (size_t i = 0; i < rhs.size; i++) {
        if (buffer[i] != rhs.buffer[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator<(const BigInt& rhs) const {
    if (sign != rhs.sign) {
        return !sign;
    }
    if (size != rhs.size) {
        return size < rhs.size;
    }
    BigIntBufferType* left_buffer = sign ? buffer : rhs.buffer;
    BigIntBufferType* right_buffer = sign ? rhs.buffer : buffer;
    for (int i = 0; i < size; i++) {
        if (left_buffer[i] != right_buffer[i]) {
            return left_buffer[i] < right_buffer[i];
        }
    }
    return false;
}
