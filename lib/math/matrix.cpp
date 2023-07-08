#include "lib/math/matrix.hpp"
#include "lib/math/algebra.hpp"

#include <iostream>
#include <functional>   // std::modulus, std::bind2nd
#include <algorithm>    // std::transform

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
            std::cout << (unsigned char)(element) << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<MATRIX_TYPE> Matrix::vector()
{
    std::vector<MATRIX_TYPE> result;

    for(unsigned int i = 0; i < data.size(); ++i)
    {
        for(unsigned int j = 0; j < data[0].size(); ++j)
            result.push_back(data[i][j]);
    }

    return result;
}

void Matrix::shift(unsigned int row, int shift)
{
    std::vector<MATRIX_TYPE> row_data(cols);

    // std::transform (numbers, numbers+5, remainders, std::bind2nd(std::modulus<int>(),2));
    for(int i = 0; i < cols; ++i)
    {
        std::cout << cols << " Shift: " << ((int)(cols)) << "=>" << i << "\n";
        row_data[Algebra::mod((i + shift), cols)] = data[row][i];
    }
    data[row] = row_data;
}

Matrix Matrix::vector_to_matrix(std::vector<MATRIX_TYPE> vector, unsigned int num_rows, unsigned int num_cols)
{
    Matrix result(num_rows, num_cols);
    unsigned int c_row = 0, c_col = 0;

    for(unsigned int i = 0; i < vector.size(); ++i)
    {
        if(c_col >= num_cols)
        {
            c_row += 1;
            result.data[c_row][0] = vector[i];

            c_col = 1;
        }
        else
        {
            result.data[c_row][c_col] = vector[i];
            c_col += 1;
        }
    }

    return result;
}
