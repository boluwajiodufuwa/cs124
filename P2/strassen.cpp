#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "tests.h"

using namespace std;

/*
Note that Matrices are represented in the following form "int matrix[width*height];"
where each value in the matrix is an int. See this link for more info:
https://stackoverflow.com/questions/256297/best-way-to-represent-a-2-d-array-in-c-with-size-determined-at-run-time

For example, the array with dimension 2 {0,1,2,3} appears like the following:
0 2
1 3
*/

// ----------------------------------------------------------------
// Matrix Helper functions
// ----------------------------------------------------------------

int getMatrixValue(int *matrix, int row, int col, int width) {
  return matrix[row + col*width];
}

void instantiateMatrix(int *matrix, int instantiator[], int dimension) {
    for (int i = 0; i < dimension*dimension; i++) {
        matrix[i] = instantiator[i];
        cout << "Matrix value: " << matrix[i] << endl;
    }
}

void setMatrixValue(int *matrix, int row, int col, int width, int val) {
  matrix[row + col*width] = val;
}

bool compareMatrices(int *ma, int *mb, int dimension) {
    for (int i = 0; i < dimension*dimension; i++) {
        if (ma[i] != mb[i]) {
            return false;
        }
    }
    return true;
}

void getRandomMatrix(int dimension, int *result) {

}

// ----------------------------------------------------------------
// Matrix Multiplication functions
// ----------------------------------------------------------------

// Conventional matrix multiplication algorithm
void conventional(int dimension, int *ma, int *mb, int *result) {
    for (int r = 0; r < dimension; r++) {
        for (int c = 0; c < dimension; c++) {
            setMatrixValue(result, r, c, dimension, 0);
            for (int k = 0; k < dimension; k++) {
                int prod = getMatrixValue(ma, r, k, dimension) * getMatrixValue(mb, k, c, dimension);
                int curr_val = getMatrixValue(result, r, c, dimension);
                setMatrixValue(result, r, c, dimension, curr_val+prod);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int dimension = atoi(argv[2]);
    int inputfile = atoi(argv[3]);

    if (do_tests == 1) {
        run_tests();
    }

    return 0;
}