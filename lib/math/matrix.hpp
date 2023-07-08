#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include "lib/aes-128/defines.hpp"

class Matrix {
private:
    unsigned int rows;
    unsigned int cols;

public:
	std::vector<std::vector<MATRIX_TYPE>> data;

    Matrix();
    Matrix(unsigned int numRows, unsigned int numCols);

    Matrix operator*(const Matrix& other) const;
	Matrix operator^(const Matrix& other) const;
    Matrix& operator=(const Matrix& other);

    void set_element(unsigned int row, unsigned int col, unsigned char value);
    void display() const;
    inline void resize(unsigned int n_rows, unsigned int n_cols){rows = n_rows, cols = n_cols;};

    std::vector<MATRIX_TYPE> vector();

    void shift(unsigned int row, int shift);
    static Matrix vector_to_matrix(std::vector<MATRIX_TYPE> vector, unsigned int num_rows, unsigned int num_cols);
};

#endif  // MATRIX_H
