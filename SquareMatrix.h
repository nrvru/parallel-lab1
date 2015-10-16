//
// Created by nrvru on 03.10.15.
//

#ifndef PARALLEL_LAB1_SQUAREMATRIX_H
#define PARALLEL_LAB1_SQUAREMATRIX_H

#include <zconf.h>

enum InitType {EMPTY, RANDOM, MULTIPLY_IJ};

enum MatrixMultiplyType {IJK, IKJ, JKI, JIK, KIJ, KJI, NUMBER_OF_MULTIPLY_TYPES};
static const char * MatrixMultiplyTypeStrings[] = { "IJK", "IKJ", "JKI", "JIK", "KIJ", "KJI" };
const char * getTextForMatrixMultiplyType( int enumVal );

class SquareMatrix {
private:
    int** data;
    int matrix_size;
    static const int MIN_DISTRIBUTION_VALUE = 0;
    static const int MAX_DISTRIBUTION_VALUE = 1000;
    static const int TREADS_NUMBER = 8;
    void InitMatrix();
    void FillRandomData(int min_distribution_value, int max_distribution_value);
    void FillMultiplyIJData();
    int** createZeroMatrix(int matrix_size);
    SquareMatrix *MatrixMultiplyIJK(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyIKJ(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyJIK(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyJKI(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyKIJ(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyKJI(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyParallelIJK(SquareMatrix *multiplier, int threads_number = TREADS_NUMBER);
    SquareMatrix *MatrixMultiplyParallelIKJ(SquareMatrix *multiplier, int threads_number = TREADS_NUMBER);
    SquareMatrix *MatrixMultiplyParallelJKI(SquareMatrix *multiplier, int threads_number = TREADS_NUMBER);
    SquareMatrix *MatrixMultiplyParallelJIK(SquareMatrix *multiplier, int threads_number = TREADS_NUMBER);
    SquareMatrix *MatrixMultiplyParallelKIJ(SquareMatrix *multiplier, int threads_number = TREADS_NUMBER);
    SquareMatrix *MatrixMultiplyParallelKJI(SquareMatrix *multiplier, int threads_number = TREADS_NUMBER);
public:
    SquareMatrix();
    SquareMatrix(int matrix_size, InitType init_type = RANDOM);
    ~SquareMatrix();
    SquareMatrix *MatrixMultiply(SquareMatrix *multiplier, MatrixMultiplyType multiply_type = IJK);
    SquareMatrix *MatrixMultiplyParallel(SquareMatrix *multiplier, MatrixMultiplyType multiply_type = IJK, int threads_number = TREADS_NUMBER);
    int GetNorm();
    int** get_data();
    void set_data(int **data, int matrix_size);
    std::string ToString();
};


#endif //PARALLEL_LAB1_SQUAREMATRIX_H
