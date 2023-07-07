#include "lib/aes-128/matrix.hpp"
#include "gtest/gtest.h"

Matrix matrixA(2, 3);
Matrix matrixB(3, 2);
Matrix matrixC(2, 2);

class MatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        matrixA.data = {
            {1, 2, 3},
            {4, 5, 6},
        };

        matrixB.data = {
            {7, 8},
            {9, 10},
            {11, 12},
        };

        matrixC.data = {
            {58, 64},
            {139, 154}
        };
    }
};

TEST_F(MatrixTest, MatrixMultiplication) {
    Matrix result = matrixA * matrixB;
    EXPECT_EQ(result.data, matrixC.data);
}

TEST_F(MatrixTest, MatrixAssignment) {
    Matrix matrixD;
    matrixD = matrixC;
    EXPECT_EQ(matrixD.data, matrixC.data);
}
