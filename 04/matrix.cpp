
#include <functional>


class Matrix
{
public:
    class RowAccessor {
    public:
        RowAccessor(const Matrix& matrix, int row) : matrix(matrix), row(row) {} ;

        int& operator [] (const int col) const {
            if (col < 0 || col >= matrix.cols) {
                throw std::out_of_range("");
            }
            return matrix.matrix_buffer[row * matrix.cols + col];
        }

    protected:
        const Matrix& matrix;
        const int row;
    };

    class RowAccessorConst : RowAccessor {
    public:
        // using RowAccessor::RowAccessor; почему то не сработало в make, хотя в idea работало
        RowAccessorConst(const Matrix& matrix, int row) : RowAccessor(matrix, row) {} ;

        const int& operator [] (const int col) const {
            return RowAccessor::operator[](col);
        }
    };

    Matrix(const int rows, const int cols) : rows(rows), cols(cols) {
        assert(rows > 0);
        assert(cols > 0);
        matrix_buffer = new int[rows * cols];
        for (int i = 0; i < rows * cols; i++) matrix_buffer[i] = 0;
    }

    Matrix(const int rows, const int cols, int* init_buffer) : rows(rows), cols(cols), matrix_buffer(init_buffer) {
        assert(rows > 0);
        assert(cols > 0);
    }

    inline int getRows() const {
        return rows;
    }

    inline int getColumns() const {
        return cols;
    }

    RowAccessorConst operator [] (const int row) const {
        if (row < 0 || row >= rows) {
            throw std::out_of_range("");
        }
        return RowAccessorConst(*this, row);
    }

    RowAccessor operator [] (const int row) {
        if (row < 0 || row >= rows) {
            throw std::out_of_range("");
        }
        return RowAccessor(*this, row);
    }

    void operator *= (const int multiplier) {
        for (int i = 0; i < rows * cols; i++) {
            matrix_buffer[i] *= multiplier;
        }
    }

    bool operator == (const Matrix& other) const {
        if (this == &other)
            return true;

        if (rows != other.getRows() || cols != other.getColumns()) {
            return false;
        }

        for (int i = 0; i < rows * cols; i++)
            if (matrix_buffer[i] != other.matrix_buffer[i])
                return false;

        return true;
    }

    bool operator != (const Matrix& other) const {
        return !(*this == other);
    }

    ~Matrix() {
        delete[] matrix_buffer;
    }

private:
    const int rows;
    const int cols;
    int* matrix_buffer;
};
