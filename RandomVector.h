//
// Created by ruslan on 02.10.15.
//

#ifndef PARALLEL_LAB1_RANDOMVECTOR_H
#define PARALLEL_LAB1_RANDOMVECTOR_H

#include <string>

class RandomVector {
private:
    int min_distribution_value;
    int max_distribution_value;
    int DEFAULT_NUM_THREADS = 8;
    int* vector;
    int vector_size;
    void GenerateVector();
public:
    RandomVector(int vector_size, int min_distribution_value = 0, int max_distribution_value = 2000000000);
    ~RandomVector();
    int *get_vector();
    int MaxElement();
    int MaxElement(int num_threads);
    long long ScalarProduct(RandomVector *multiplier);
    long long ScalarProductReduction(RandomVector *multiplier);
    long long ScalarProductCascade(RandomVector *multiplier, int elementsInSeriesPerThread = 1000000);

    std::string ToString();
};


#endif //PARALLEL_LAB1_RANDOMVECTOR_H
