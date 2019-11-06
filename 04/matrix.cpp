
#include <functional>


class Matrix
{
public:
    class RowAccessor {
    public:
        RowAccessor(const Matrix& matrix, size_t row) : matrix(matrix), row(row) {} ;

        const int& operator [] (const int col) const {
            if (col < 0 || col >= matrix.cols) {
                throw std::out_of_range("");
            }
            return matrix.matrix_buffer[row * matrix.cols + col];
        }

        int& operator [] (const int col) {
            if (col < 0 || col >= matrix.cols) {
                throw std::out_of_range("");
            }
            return matrix.matrix_buffer[row * matrix.cols + col];
        }

    protected:
        const Matrix& matrix;
        const size_t row;
    };

    Matrix(const int rows, const int cols) : rows(rows), cols(cols) {
        matrix_buffer = new int[rows * cols];
        for (int i = 0; i < rows * cols; i++) matrix_buffer[i] = 0;
    }

    Matrix(const int rows, const int cols, int* init_buffer) : rows(rows), cols(cols), matrix_buffer(init_buffer) {}

    int getRows() const {
        return rows;
    }

    int getColumns() const {
        return cols;
    }

    const RowAccessor operator [] (const int row) const {
        if (row < 0 || row >= rows) {
            throw std::out_of_range("");
        }
        return RowAccessor(*this, row);
    }

    RowAccessor operator [] (const int row) {
        if (row < 0 || row >= rows) {
            throw std::out_of_range("");
        }
        return RowAccessor(*this, row);
    }

    Matrix& operator *= (const int multiplier) {
        for (int i = 0; i < rows * cols; i++) {
            matrix_buffer[i] *= multiplier;
        }
        return *this;
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
