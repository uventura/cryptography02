#include "lib/math/matrix.hpp"
#include "lib/aes-128/defines.hpp"

#include "gtest/gtest.h"

Matrix matrixA(2, 3);
Matrix matrixB(3, 2);
Matrix matrixC(2, 2);
Matrix matrixD(2, 2);

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

        matrixD.data = {
            {'\x19', ' '},
            {'\x1c', '%'}
        };
    }
};

TEST_F(MatrixTest, MatrixMultiplication) {
    Matrix result = matrixA * matrixB;
    EXPECT_EQ(result.data, matrixC.data);
}

TEST_F(MatrixTest, MatrixXor) {
    Matrix result = matrixC ^ matrixD;
    result = result ^ matrixC;
    EXPECT_EQ(result.data, matrixD.data);
}

TEST_F(MatrixTest, MatrixAssignment) {
    Matrix matrixE;
    matrixE = matrixC;
    EXPECT_EQ(matrixE.data, matrixC.data);
}

TEST_F(MatrixTest, MatrixToVector) {
    auto vectorA = matrixA.vector();
    std::vector<MATRIX_TYPE> expected = {1,2,3,4,5,6};

    EXPECT_EQ(vectorA, expected);
}
