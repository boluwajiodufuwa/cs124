#ifndef STRASSEN_H_
#define STRASSEN_H_

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

void conventional(int dimension, int *ma, int *mb, int *result);
void strassen(int dim, int* ma, int* mb, int* result);
void instantiateMatrix(int *matrix, int instantiator[], int dimension);
bool compareMatrices(int *ma, int *mb, int dimension);
void getRandomMatrix(int dimension, int *result, int start_range, int end_range);

#endif
