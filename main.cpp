#include <iostream>
#include <random>
#include <chrono>

#include "RandomVector.h"
#include "SquareMatrix.h"

using namespace std;

int REPETITIONS = 10;

int MAX_THREADS = 16;

void MaxVectorElement(int vector_size = 1000000000, bool is_print_vector = false);

void MatrixMultiplication(int matrix_size = 1000, bool is_print_matrices = false);

void ScalarProduct(int vector_size = 1000000000, bool is_print_vectors = false);

string vectorToString(int *vector, int size);
string vectorToString(float *vector, int size);
string pluralizeThreads(int threads_number);

int main() {
    cout << "Лабораторная работа №1" << endl;

    MaxVectorElement();

    MatrixMultiplication();

    ScalarProduct();

    return 0;
}

void MaxVectorElement(int vector_size, bool is_print_vector) {
    chrono::time_point<chrono::system_clock> start, end;
    int max = 0, elapsedTime, meanTime, i, z;

    int resultTimes[MAX_THREADS];
    float acceleration[MAX_THREADS];
    float efficiency[MAX_THREADS];

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
        cout << "Максимальное значение вектора равно: " + to_string(max) << endl;
    }
    meanTime /= REPETITIONS;
    resultTimes[0] = meanTime;
    acceleration[0] = efficiency[0] = 1;
    cout << endl << "Среднее время выполнения:" << meanTime << "ms" << endl;


    // Многопоточная версия
    int threads_number;
    for(z = 1; z < MAX_THREADS; z++) {
        threads_number = z + 1;

        meanTime = 0;
        for (i = 0; i < REPETITIONS; i++) {
            max = 0;
            start = chrono::system_clock::now();
            max = random_vector->MaxElement(threads_number);
            end = chrono::system_clock::now();
            meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
            cout << "Максимальное значение вектора равно (parallel): " + to_string(max) << endl;
        }
        meanTime /= REPETITIONS;

        resultTimes[z] = meanTime;
        acceleration[z] = (float)resultTimes[0]/resultTimes[z];
        efficiency[z] = acceleration[z]/threads_number;

        cout << endl << "Среднее время выполнения (" + pluralizeThreads(threads_number) +
                                "):" << meanTime << "ms" << endl;
    }

    cout << "Итоговые результаты:"  << endl;
    cout << "Время выполнения: " << vectorToString(resultTimes, MAX_THREADS) << endl;
    cout << "Коэффициент ускорения: " << vectorToString(acceleration, MAX_THREADS) << endl;
    cout << "Коэффициент эффективности: " << vectorToString(efficiency, MAX_THREADS) << endl;

    delete random_vector;
}

void MatrixMultiplication(int matrix_size, bool is_print_matrices){
    chrono::time_point<chrono::system_clock> start, end;
    int elapsedTime, meanTime, i, x, y, threads_number;
    SquareMatrix *result;

    int resultTimes[NUMBER_OF_MULTIPLY_TYPES][MAX_THREADS];
    float acceleration[NUMBER_OF_MULTIPLY_TYPES][MAX_THREADS];
    float efficiency[NUMBER_OF_MULTIPLY_TYPES][MAX_THREADS];

    cout << "Произведение матриц" << endl;

    start = chrono::system_clock::now();
    SquareMatrix * matrix1 = new SquareMatrix(matrix_size);
    SquareMatrix * matrix2 = new SquareMatrix(matrix_size);
    end = chrono::system_clock::now();
    elapsedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Время генерации матриц:" << elapsedTime << "ms" << endl << endl;

    if(is_print_matrices){
        cout << "Матрица А:\n" + matrix1->ToString() << endl;
        cout << "Матрица B:\n" + matrix2->ToString() << endl;
    }


    cout << "Однопоточная версия" << endl;
    for(x = 0; x < NUMBER_OF_MULTIPLY_TYPES; x++) {
        cout << "Выриант алгоритма " << getTextForMatrixMultiplyType(x) << endl;
        meanTime = 0;
        for (i = 0; i < REPETITIONS; i++) {
            start = chrono::system_clock::now();
            result = matrix1->MatrixMultiply(matrix2, MatrixMultiplyType(x));
            end = chrono::system_clock::now();
            meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
            cout << "Норма матрицы: " << result->GetNorm() << endl;
            delete result;
        }
        meanTime /= REPETITIONS;
        cout << "Среднее время выполнения однопоточного умножения " << getTextForMatrixMultiplyType(x) << ": " << meanTime << "ms" << endl << endl;
        resultTimes[x][0] = meanTime;
        acceleration[x][0] = efficiency[x][0] = 1;
    }

    cout << "Многопоточная версия" << endl;
    for(x = 0; x < NUMBER_OF_MULTIPLY_TYPES; x++) {
        cout << "Выриант алгоритма " << getTextForMatrixMultiplyType(x) << endl;
        for(y = 1; y < MAX_THREADS; y++) {
            threads_number = y + 1;
            meanTime = 0;
            for (i = 0; i < REPETITIONS; i++) {
                start = chrono::system_clock::now();
                result = matrix1->MatrixMultiplyParallel(matrix2, MatrixMultiplyType(x), threads_number);
                end = chrono::system_clock::now();
                meanTime += chrono::duration_cast<chrono::milliseconds>(end - start).count();
                cout << "Норма матрицы: " << result->GetNorm() << endl;
                if (is_print_matrices) {
                    cout << "Матрица C:\n" + result->ToString() << endl;
                }
                delete result;
            }
            meanTime /= REPETITIONS;
            cout << "Среднее время выполнения многопоточного умножения " << getTextForMatrixMultiplyType(x) <<
                    " (" << pluralizeThreads(threads_number) << "): " << meanTime << "ms" << endl << endl;

            resultTimes[x][y] = meanTime;
            acceleration[x][y] = (float)resultTimes[x][0]/resultTimes[x][y];
            efficiency[x][y] = acceleration[x][y]/threads_number;
        }
    }


    cout << "Результаты измерений" << endl;
    for(x = 0; x < NUMBER_OF_MULTIPLY_TYPES; x++){
        cout << "Вариант алгоритма " << getTextForMatrixMultiplyType(x) << endl;
        cout << "Время выполнения: " << vectorToString(resultTimes[x], MAX_THREADS) << endl;
        cout << "Коэффициент ускорения: " << vectorToString(acceleration[x], MAX_THREADS) << endl;
        cout << "Коэффициент эффективности: " << vectorToString(efficiency[x], MAX_THREADS) << endl;
    }

    delete matrix1;
    delete matrix2;
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

    delete vectorA;
    delete vectorB;
}

string vectorToString(int *vector, int size) {
    string str = "";
    for(int z = 0; z < size; z++){
        str += to_string(vector[z]) + ((z == size - 1) ? "" : ", ");
    }

    return str;
}

string vectorToString(float *vector, int size) {
    string str = "";
    for(int z = 0; z < size; z++){
        str += to_string(vector[z]) + ((z == size - 1) ? "" : ", ");
    }

    return str;
}

string pluralizeThreads(int threads_number){
    return  std::to_string(threads_number) +
    " поток" + (threads_number == 1 ? "" : (threads_number > 1 && threads_number < 5) ? "а" : "ов");
}