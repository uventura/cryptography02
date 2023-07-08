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

    void setElement(unsigned int row, unsigned int col, unsigned int value);
    void display() const;

    std::vector<MATRIX_TYPE> vector();

    void shift(unsigned int row, int shift);
    static Matrix vector_to_matrix(std::vector<MATRIX_TYPE> vector, unsigned int num_rows, unsigned int num_cols);
};

#endif  // MATRIX_H
