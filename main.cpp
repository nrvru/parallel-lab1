#include <iostream>
#include <random>
#include <chrono>

#include "RandomVector.h"
#include "SquareMatrix.h"

using namespace std;

int REPETITIONS = 10;

void MaxVectorElement(int vector_size = 1000000000, bool is_print_vector = false);

void MatrixMultiplication(int matrix_size = 1000, bool is_print_matrices = false);

void ScalarProduct(int vector_size = 1000000000, bool is_print_vectors = false);

int main() {
    cout << "Лабораторная работа №1" << endl;

    MaxVectorElement();

    MatrixMultiplication();

    ScalarProduct();

    return 0;
}

void MaxVectorElement(int vector_size, bool is_print_vector) {
    chrono::time_point<chrono::system_clock> start, end;
    int max = 0, elapsedTime, meanTime, i;

    int threads_numbers[5] = {1, 2, 4, 8, 16};

    cout << "Поиск максимального значения вектора" << endl;

    start = chrono::system_clock::now();
    RandomVector *random_vector = new RandomVector(vector_size);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Время генерации вектора:" << elapsedTime << "ms" << endl;

    if(is_print_vector) {
        cout << "Сгенерированный вектор:\n" + random_vector->ToString() << endl;
    }

    // Однопоточная версия
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        max = 0;
        start = chrono::system_clock::now();
        max = random_vector->MaxElement();
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;

    cout << endl << "Среднее время выполнения:" << meanTime << "ms" << endl;
    cout << "Максимальное значение вектора равно: " + to_string(max) << endl;


    // Многопоточная версия
    for(int z = 0; z < 5; z++) {
        meanTime = 0;
        for (i = 0; i < REPETITIONS; i++) {
            max = 0;
            start = chrono::system_clock::now();
            max = random_vector->MaxElement(threads_numbers[z]);
            end = chrono::system_clock::now();
            meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
        }
        meanTime /= REPETITIONS;

        cout << endl << "Среднее время выполнения (" + std::to_string(threads_numbers[z]) +
                " поток" + (threads_numbers[z] == 1 ? "" : (threads_numbers[z] == 2 || threads_numbers[z] == 4) ? "а" : "ов") +
                "):" << meanTime << "ms" << endl;
        cout << "Максимальное значение вектора равно (parallel): " + to_string(max) << endl;
    }

    delete random_vector;
}

void MatrixMultiplication(int matrix_size, bool is_print_matrices){
    chrono::time_point<chrono::system_clock> start, end;
    int elapsedTime, meanTime, i;
    SquareMatrix *result;

    cout << "Произведение матриц" << endl;

    start = chrono::system_clock::now();
    SquareMatrix * matrix1 = new SquareMatrix(matrix_size);
    SquareMatrix * matrix2 = new SquareMatrix(matrix_size);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Время генерации матриц:" << elapsedTime << "ms" << endl;

    if(is_print_matrices){
        cout << "Матрица А:\n" + matrix1->ToString() << endl;
        cout << "Матрица B:\n" + matrix2->ToString() << endl;
    }

    // Однопоточная версия
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiply(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения умножения: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;



    // Многопоточная версия IJK
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiplyParallel(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения многопоточного умножения IJK: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;

    if(is_print_matrices){
        cout << "Матрица C:\n" + result->ToString() << endl;
    }


    // Многопоточная версия IKJ
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiplyParallelIKJ(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения многопоточного умножения IKJ: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;

    if(is_print_matrices){
        cout << "Матрица C:\n" + result->ToString() << endl;
    }

    // Многопоточная версия KIJ
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiplyParallelKIJ(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения многопоточного умножения KIJ: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;

    if(is_print_matrices){
        cout << "Матрица C:\n" + result->ToString() << endl;
    }

    // Многопоточная версия JKI
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiplyParallelJKI(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения многопоточного умножения JKI: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;

    if(is_print_matrices){
        cout << "Матрица C:\n" + result->ToString() << endl;
    }

    // Многопоточная версия JIK
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiplyParallelJIK(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения многопоточного умножения JIK: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;

    if(is_print_matrices){
        cout << "Матрица C:\n" + result->ToString() << endl;
    }

    // Многопоточная версия KJI
    meanTime = 0;
    for(i = 0; i < REPETITIONS; i++) {
        start = chrono::system_clock::now();
        result = matrix1->MatrixMultiplyParallelKJI(matrix2);
        end = chrono::system_clock::now();
        meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
    meanTime /= REPETITIONS;
    cout << endl << "Среднее время выполнения многопоточного умножения KJI: " << meanTime << "ms" << endl;
    cout << "Норма матрицы: " << result->GetNorm() << endl;

    if(is_print_matrices){
        cout << "Матрица C:\n" + result->ToString() << endl;
    }

    delete matrix1;
    delete matrix2;
    delete result;
}

void ScalarProduct(int vector_size, bool is_print_vectors) {
    chrono::time_point<chrono::system_clock> start, end;
    long long product = 0;
    int elapsedTime, meanTime, i;
    RandomVector *vectorA, *vectorB;

    cout << "Скалярное произведение векторов" << endl;

    start = chrono::system_clock::now();
    vectorA = new RandomVector(vector_size, 0, 1000);
    vectorB = new RandomVector(vector_size, 0, 1000);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Время генерации векторов:" << elapsedTime << "ms" << endl;

    if(is_print_vectors) {
        cout << "Сгенерированный вектор А:\n" + vectorA->ToString() << endl;
        cout << "Сгенерированный вектор B:\n" + vectorB->ToString() << endl;
    }

    // Однопоточная версия
    start = chrono::system_clock::now();
    product = vectorA->ScalarProduct(vectorB);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Время расчета скалярного произведения:" << elapsedTime << "ms" << endl;
    cout << "Скалярное произведение равно: " + std::to_string(product) << endl;

    //Многопоточная версия
    start = chrono::system_clock::now();
    product = vectorA->ScalarProductReduction(vectorB);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Время параллельного расчета скалярного произведения (reduction):" << elapsedTime << "ms" << endl;
    cout << "Скалярное произведение равно: " + std::to_string(product) << endl;

    start = chrono::system_clock::now();
    product = vectorA->ScalarProductCascade(vectorB);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << endl << "Время параллельного расчета скалярного произведения (cascade):" << elapsedTime << "ms" << endl;
    cout << "Скалярное произведение равно: " + std::to_string(product) << endl;
}