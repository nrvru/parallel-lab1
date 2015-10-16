#include <random>
#include <omp.h>

#include "SquareMatrix.h"

SquareMatrix::SquareMatrix() {}

SquareMatrix::SquareMatrix(int matrix_size, InitType init_type) {
    this->matrix_size = matrix_size;

    InitMatrix();
    switch (init_type){
        case EMPTY:
            break;
        case RANDOM:
            FillRandomData(0, 10000);
            break;
        case MULTIPLY_IJ:
            FillMultiplyIJData();
    }
}

SquareMatrix::~SquareMatrix() {
    for(int i = 0; i < matrix_size; i++)
    {
        delete[] data[i];
    }
    delete [] data;
}

SquareMatrix * SquareMatrix::MatrixMultiply(SquareMatrix *multiplierMatrix, MatrixMultiplyType multiply_type) {
    SquareMatrix *result_matrix;

    switch (multiply_type){
        case IJK:
            result_matrix = this->MatrixMultiplyIJK(multiplierMatrix);
            break;
        case IKJ:
            result_matrix = this->MatrixMultiplyIKJ(multiplierMatrix);
            break;
        case JKI:
            result_matrix = this->MatrixMultiplyJKI(multiplierMatrix);
            break;
        case JIK:
            result_matrix = this->MatrixMultiplyJIK(multiplierMatrix);
            break;
        case KIJ:
            result_matrix = this->MatrixMultiplyKIJ(multiplierMatrix);
            break;
        case KJI:
            result_matrix = this->MatrixMultiplyKJI(multiplierMatrix);
            break;
    }

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyIJK(SquareMatrix *multiplierMatrix) {
    int i, j, k, **result;
    int **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    for(i = 0; i < matrix_size; i++)
        for (j = 0; j < matrix_size; j++)
            for (k = 0; k < matrix_size; k++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyIKJ(SquareMatrix *multiplierMatrix) {
    int i, j, k, **result;
    int **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    for(i = 0; i < matrix_size; i++)
        for (k = 0; k < matrix_size; k++)
            for (j = 0; j < matrix_size; j++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyJIK(SquareMatrix *multiplierMatrix) {
    int i, j, k, **result;
    int **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    for (j = 0; j < matrix_size; j++)
        for(i = 0; i < matrix_size; i++)
            for (k = 0; k < matrix_size; k++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyJKI(SquareMatrix *multiplierMatrix) {
    int i, j, k, **result;
    int **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    for (j = 0; j < matrix_size; j++)
        for (k = 0; k < matrix_size; k++)
            for(i = 0; i < matrix_size; i++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyKIJ(SquareMatrix *multiplierMatrix) {
    int i, j, k, **result;
    int **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    for (k = 0; k < matrix_size; k++)
        for(i = 0; i < matrix_size; i++)
            for (j = 0; j < matrix_size; j++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyKJI(SquareMatrix *multiplierMatrix) {
    int i, j, k, **result;
    int **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    for (k = 0; k < matrix_size; k++)
        for (j = 0; j < matrix_size; j++)
            for(i = 0; i < matrix_size; i++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix *SquareMatrix::MatrixMultiplyParallel(SquareMatrix *multiplier, MatrixMultiplyType multiply_type,
                                                   int threads_number) {
    SquareMatrix *result_matrix;

    switch (multiply_type){
        case IJK:
            result_matrix = this->MatrixMultiplyParallelIJK(multiplier, threads_number);
            break;
        case IKJ:
            result_matrix = this->MatrixMultiplyParallelIKJ(multiplier, threads_number);
            break;
        case JKI:
            result_matrix = this->MatrixMultiplyParallelJKI(multiplier, threads_number);
            break;
        case JIK:
            result_matrix = this->MatrixMultiplyParallelJIK(multiplier, threads_number);
            break;
        case KIJ:
            result_matrix = this->MatrixMultiplyParallelKIJ(multiplier, threads_number);
            break;
        case KJI:
            result_matrix = this->MatrixMultiplyParallelKJI(multiplier, threads_number);
            break;
    }

    return result_matrix;
}

SquareMatrix * SquareMatrix::MatrixMultiplyParallelIJK(SquareMatrix *multiplierMatrix, int threads_number) {
    int i, j, k, **result;
    int matrix_size = this->matrix_size;
    int **data = this->data, **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    omp_set_num_threads(threads_number);
#pragma omp parallel for shared(matrix_size, data, multiplier, result) private(i, j, k)
    for(i = 0; i < matrix_size; i++)
        for (j = 0; j < matrix_size; j++)
            for (k = 0; k < matrix_size; k++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix * SquareMatrix::MatrixMultiplyParallelIKJ(SquareMatrix *multiplierMatrix, int threads_number) {
    int i, j, k, **result;
    int matrix_size = this->matrix_size;
    int **data = this->data, **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    omp_set_num_threads(threads_number);
#pragma omp parallel for shared(matrix_size, data, multiplier, result) private(i, j, k)
    for(i = 0; i < matrix_size; i++)
        for (k = 0; k < matrix_size; k++)
            for (j = 0; j < matrix_size; j++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix * SquareMatrix::MatrixMultiplyParallelKIJ(SquareMatrix *multiplierMatrix, int threads_number) {
    int i, j, k, **result;
    int matrix_size = this->matrix_size;
    int **data = this->data, **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    omp_set_num_threads(threads_number);
#pragma omp parallel for shared(matrix_size, data, multiplier, result) private(i, j, k)
    for (k = 0; k < matrix_size; k++)
        for(i = 0; i < matrix_size; i++)
            for (j = 0; j < matrix_size; j++) {
#pragma omp atomic
                result[i][j] += (data[i][k] * multiplier[k][j]);
            }

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix * SquareMatrix::MatrixMultiplyParallelJKI(SquareMatrix *multiplierMatrix, int threads_number) {
    int i, j, k, **result;
    int matrix_size = this->matrix_size;
    int **data = this->data, **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    omp_set_num_threads(threads_number);
#pragma omp parallel for shared(matrix_size, data, multiplier, result) private(i, j, k)
    for (j = 0; j < matrix_size; j++)
        for (k = 0; k < matrix_size; k++)
            for(i = 0; i < matrix_size; i++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix * SquareMatrix::MatrixMultiplyParallelJIK(SquareMatrix *multiplierMatrix, int threads_number) {
    int i, j, k, **result;
    int matrix_size = this->matrix_size;
    int **data = this->data, **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    omp_set_num_threads(threads_number);
#pragma omp parallel for shared(matrix_size, data, multiplier, result) private(i, j, k)
    for (j = 0; j < matrix_size; j++)
        for(i = 0; i < matrix_size; i++)
            for (k = 0; k < matrix_size; k++)
                result[i][j] += (data[i][k] * multiplier[k][j]);

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

SquareMatrix * SquareMatrix::MatrixMultiplyParallelKJI(SquareMatrix *multiplierMatrix, int threads_number) {
    int i, j, k, **result;
    int matrix_size = this->matrix_size;
    int **data = this->data, **multiplier = multiplierMatrix->get_data();

    result = createZeroMatrix(matrix_size);

    omp_set_num_threads(threads_number);
#pragma omp parallel for shared(matrix_size, data, multiplier, result) private(i, j, k)
    for (k = 0; k < matrix_size; k++)
        for (j = 0; j < matrix_size; j++)
            for(i = 0; i < matrix_size; i++) {
#pragma omp atomic
                result[i][j] += (data[i][k] * multiplier[k][j]);
            }

    SquareMatrix *result_matrix = new SquareMatrix();
    result_matrix->set_data(result, matrix_size);

    return result_matrix;
}

int SquareMatrix::GetNorm() {
    int temp = 0, norm_m = 0, i, j;

    for (i = 0; i < matrix_size; i++) {
        temp = 0;
        for (j = 0; j < matrix_size; j++)
            temp += abs(data[i][j]);

        if (temp > norm_m)
            norm_m = temp;
    }

    return norm_m;
}

void SquareMatrix::InitMatrix() {
    data = createZeroMatrix(matrix_size);
}

void SquareMatrix::FillRandomData(
        int min_distribution_value = MIN_DISTRIBUTION_VALUE,
        int max_distribution_value = MAX_DISTRIBUTION_VALUE
) {
    int i, j;
    std::minstd_rand generator;
    std::uniform_int_distribution<> distribution(min_distribution_value, max_distribution_value);

    for(i = 0; i < matrix_size; i++) {
        for(j = 0; j < matrix_size; j++){
            data[i][j] = distribution(generator);
        }
    }
}

void SquareMatrix::FillMultiplyIJData() {
    int i, j;

    for(i = 0; i < matrix_size; i++) {
        for(j = 0; j < matrix_size; j++){
            data[i][j] = i*j;
        }
    }
}

std::string SquareMatrix::ToString() {
    int i, j;
    std::string result = "";
    for(i = 0; i < matrix_size; i++) {
        for(j = 0; j < matrix_size; j++){
            result += std::to_string(data[i][j]) + ", ";
        }
        result += "\n";
    }
    return result;
}

int **SquareMatrix::get_data() {
    return data;
}

void SquareMatrix::set_data(int **data, int matrix_size) {
    this->data = data;
    this->matrix_size = matrix_size;
}


int **SquareMatrix::createZeroMatrix(int matrix_size) {
    int i, j;
    int **zeroMatrix = new int*[matrix_size];

    for(i = 0; i < matrix_size; i++) {
        zeroMatrix[i] = new int[matrix_size];
        for(j = 0; j < matrix_size; j++) {
            zeroMatrix[i][j] = 0;
        }
    }
    return zeroMatrix;
}

const char * getTextForMatrixMultiplyType( int enumVal ) { return MatrixMultiplyTypeStrings[enumVal]; }

