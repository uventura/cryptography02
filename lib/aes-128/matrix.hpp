#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix {
private:
    std::vector<std::vector<unsigned int>> data;
    unsigned int rows;
    unsigned int cols;

public:
    Matrix();
    Matrix(unsigned int numRows, unsigned int numCols);
    Matrix operator*(const Matrix& other) const;
    Matrix& operator=(const Matrix& other);
    void setElement(unsigned int row, unsigned int col, unsigned int value);
    void display() const;
};

#endif  // MATRIX_H
