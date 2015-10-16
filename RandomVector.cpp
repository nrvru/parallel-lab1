#include <string>
#include <random>
#include <omp.h>

#include "RandomVector.h"

RandomVector::RandomVector(int vector_size, int min_distribution_value, int max_distribution_value) {
    this->min_distribution_value = min_distribution_value;
    this->max_distribution_value = max_distribution_value;
    this->vector = new int[vector_size];
    this->vector_size = vector_size;
    this->GenerateVector();
}

RandomVector::~RandomVector() {
    delete [] vector;
}

void RandomVector::GenerateVector() {
    std::minstd_rand generator;
    std::uniform_int_distribution<> distribution(min_distribution_value, max_distribution_value);

    for(int i = 0; i < vector_size; i++) {
        vector[i] = distribution(generator);
    }
}

int RandomVector::MaxElement() {
    int max = 0, i;

    for(i = 0; i < vector_size; i++) {
        if(vector[i] > max) {
            max = vector[i];
        }
    }
    return max;
}

int RandomVector::MaxElement(int num_threads) {
    int max = 0, i;
    int vector_size = this->vector_size;
    int* vector = this->vector;

    #pragma omp parallel num_threads(num_threads) shared(vector, vector_size, max) private(i)
    {
        #pragma omp for
        for (i = 0; i < vector_size; i++) {
            if (vector[i] > max) {
                max = vector[i];
            }
        }
    }
    return max;
}

long long RandomVector::ScalarProduct(RandomVector *multiplier) {
    long long product = 0;
    int *multiplier_vector = multiplier->get_vector();

    for(int i = 0; i < vector_size; i++){
        product += (long long)vector[i] * multiplier_vector[i];
    }

    return product;
}

long long RandomVector::ScalarProductReduction(RandomVector *multiplier) {
    long long product = 0;
    int *vector = this->vector;
    int *multiplier_vector = multiplier->get_vector();
    int vector_size = this->vector_size;

    omp_set_num_threads(DEFAULT_NUM_THREADS);
    #pragma omp parallel for shared(vector, vector_size, multiplier_vector) reduction(+:product)
    for (int i = 0; i < vector_size; i++) {
        product += (long long) vector[i] * multiplier_vector[i];
    }

    return product;
}

long long RandomVector::ScalarProductCascade(RandomVector *multiplier, int elementsInSeriesPerThread) {
    int *vector = this->vector;
    int *multiplier_vector = multiplier->get_vector();
    long long *temp = new long long[this->vector_size];
    int vector_size = this->vector_size;
    int step;

    omp_set_num_threads(DEFAULT_NUM_THREADS);
    #pragma omp parallel private(step) shared(vector_size)
    {
        int i, j, count, num;
        num = omp_get_thread_num();
        count = omp_get_num_threads();

        for(i = num * elementsInSeriesPerThread; i < vector_size; i += elementsInSeriesPerThread * count) {
            temp[i] = 0;
            for (j = i; j < (i + elementsInSeriesPerThread) && j < vector_size; j++) {
                temp[i] += vector[j] * multiplier_vector[j];
            }
        }

        for(step = elementsInSeriesPerThread; step < vector_size; step *= 2) {
            #pragma omp barrier
            #pragma omp for
            for(i = step; i < vector_size; i += 2*step) {
                temp[i-step] += temp[i];
            }

        }
    }

    return temp[0];
}

std::string RandomVector::ToString() {
    std::string str = "";
    for (int i = 0; i < vector_size; i++) {
        str += std::to_string(vector[i]) + ", ";
        if(i % 10 == 0 && i != 0){
            str += "\n";
        }
    }
    return str;
}


int *RandomVector::get_vector() {
    return this->vector;
}
