#include "matrix.hpp"
#include <iostream>

Matrix::Matrix() : rows(0), cols(0) {}

Matrix::Matrix(unsigned int numRows, unsigned int numCols) : rows(numRows), cols(numCols) {
    data.resize(numRows, std::vector<MATRIX_TYPE>(numCols, 0));
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols != other.rows) {
        std::cout << "Matrix dimensions are incompatible for multiplication!" << std::endl;
        return Matrix();
    }

    Matrix result(rows, other.cols);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < other.cols; ++j) {
            for (unsigned int k = 0; k < cols; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }

    return result;
}

Matrix Matrix::operator^(const Matrix& other) const {
    if (cols != other.cols || rows != other.rows) {
        std::cout << "Matrix dimensions are incompatible for XOR!" << std::endl;
        return Matrix();
    }

    Matrix result(rows, cols);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            result.data[i][j] = data[i][j] ^ other.data[i][j];
        }
    }

    return result;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        data = other.data;
    }
    return *this;
}

void Matrix::setElement(unsigned int row, unsigned int col, unsigned int value) {
    if (row < rows && col < cols) {
        data[row][col] = value;
    } else {
        std::cout << "Invalid matrix index!" << std::endl;
    }
}

void Matrix::display() const {
    for (const auto& row : data) {
        for (unsigned int element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}
