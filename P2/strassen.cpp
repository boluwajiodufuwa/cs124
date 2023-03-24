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
Note that Matrices are represented in the following form "int *matrix = new int[width*height];"
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

void printMatrix(int *matrix, int dim) {
    for (int r = 0; r < dim; r++) {
        for (int c = 0; c < dim; c++) {
            cout << getMatrixValue(matrix, r, c, dim) << " ";
        }
        cout << endl;
    }
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


// ----------------------------------------------------------------
// Miscellaneous functions
// ----------------------------------------------------------------

// Reads an Ascii file with 2d^2 lines and creates two d by d matrices
pair<int*, int*> asciiToMatrices(char* fn, int dim) {
    // Open the file using the filename from the command line argument
    ifstream file(fn);

    // Check that the file was opened successfully
    if (!file.is_open()) {
        cerr << "Error: could not open file " << fn << "\n";
    }

    // Create matrices
    int *ma = new int[dim*dim];
    int *mb = new int[dim*dim];

    // Read content of files into matrices
    string line;
    int counter = 0;
    while (getline(file, line)) {
        int val = stoi(line);
        int col = counter % dim;
        int row = (counter - col) / dim;
        if (counter < dim*dim) {
            setMatrixValue(ma, row, col, dim, val);
        } else {
            setMatrixValue(mb, row - dim, col, dim, val);
        }
        counter++;
    }

    // Close the file
    file.close();

    return make_pair(ma,mb);
}

// Reads an Ascii file with 2d^2 lines and returns the product of the resulting matrices
void asciiToProduct(string fn, int dim, int *result) {

}

// ----------------------------------------------------------------
// Main
// ----------------------------------------------------------------

int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int dimension = atoi(argv[2]);
    char* inputfile = argv[3];

    if (do_tests == 1) {
        run_tests();
    }

    if (inputfile[0] != '\0') {
        printMatrix(asciiToMatrices(inputfile, dimension).first, dimension);
        cout << "\n\n";
        printMatrix(asciiToMatrices(inputfile, dimension).second, dimension);
    }

    return 0;
}