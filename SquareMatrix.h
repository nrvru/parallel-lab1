//
// Created by nrvru on 03.10.15.
//

#ifndef PARALLEL_LAB1_SQUAREMATRIX_H
#define PARALLEL_LAB1_SQUAREMATRIX_H

#include <zconf.h>

enum InitType {EMPTY, RANDOM, MULTIPLY_IJ};

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
public:
    SquareMatrix();
    SquareMatrix(int matrix_size, InitType init_type = RANDOM);
    ~SquareMatrix();
    SquareMatrix *MatrixMultiply(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyParallel(SquareMatrix *multiplier);
    SquareMatrix *MatrixMultiplyParallelIKJ(SquareMatrix *multiplierMatrix);
    SquareMatrix *MatrixMultiplyParallelJKI(SquareMatrix *multiplierMatrix);
    SquareMatrix *MatrixMultiplyParallelJIK(SquareMatrix *multiplierMatrix);
    SquareMatrix *MatrixMultiplyParallelKIJ(SquareMatrix *multiplierMatrix);
    SquareMatrix *MatrixMultiplyParallelKJI(SquareMatrix *multiplierMatrix);
    int GetNorm();
    int** get_data();
    void set_data(int **data, int matrix_size);
    std::string ToString();
};


#endif //PARALLEL_LAB1_SQUAREMATRIX_H
