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

// Generates a random matrix of given dimension into result matrix. start and end range are range
// of values that will be in matrix
void getRandomMatrix(int dimension, int *result, int start_range, int end_range) {
    // unfirom random generatord
    random_device rand_dev;
    default_random_engine gen(rand_dev());
    uniform_int_distribution<int> rand_int(start_range, end_range);

    for (int r = 0; r < dimension; r++) {
        for (int c = 0; c < dimension; c++) {
            // Adding random int value to matrix
            int val = rand_int(gen);
            setMatrixValue(result, r, c, dimension, val);
        }
    }
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

// Helper function to add two square matrices
void matrixAdd(int dim, int* ma, int* mb, int* result, bool add = true) {
    if (add == true) {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int idx = i * dim + j;
                result[idx] = ma[idx] + mb[idx];
            }
        }
    }
    else {
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                int idx = i * dim + j;
                result[idx] = ma[idx] - mb[idx];
            }
        }
    }

    return result
}


void strassen(int dim, int* ma, int* mb, int* result) {
    // Base Case
    if (dim == 1) {
        *result = *ma * *mb;
        return
    }

    // STEP 1: Splits the two n by n matrices into 4 quarter submatrices (currently functional for n=2^k)
    // Calculate the size of each submatrix (same dim for both matrices)
    int half_dim = dim / 2;
    int quarter_dim = dim / 4;

    // Initialize quarter matrices
    int* qa1 = new int[quarter_dim * quarter_dim];
    int* qa2 = new int[quarter_dim * quarter_dim];
    int* qa3 = new int[quarter_dim * quarter_dim];
    int* qa4 = new int[quarter_dim * quarter_dim];
    int* qb1 = new int[quarter_dim * quarter_dim];
    int* qb2 = new int[quarter_dim * quarter_dim];
    int* qb3 = new int[quarter_dim * quarter_dim];
    int* qb4 = new int[quarter_dim * quarter_dim];
    
    // Copy the elements of the input matrices to each submatrix
    for (int i = 0; i < half_dim; i++) {
        for (int j = 0; j < half_dim; j++) {
            // MATRIX MA
            // Copy elements to the first submatrix (top left)
            qa1[i * quarter_dim + j] = ma[i * n + j];
            
            // Copy elements to the second submatrix (top right)
            qa2[i * quarter_dim + j] = ma[i * n + half_dim + j];
            
            // Copy elements to the third submatrix (bottom left)
            qa3[i * quarter_dim + j] = ma[(half_dim + i) * n + j];
            
            // Copy elements to the fourth submatrix (bottom right)
            qa4[i * quarter_dim + j] = ma[(half_dim + i) * n + half_dim + j];

            // MATRIX MB
            // Copy elements to the first submatrix (top left)
            qb1[i * quarter_dim + j] = mb[i * n + j];
            
            // Copy elements to the second submatrix (top right)
            qb2[i * quarter_dim + j] = mb[i * n + half_dim + j];
            
            // Copy elements to the third submatrix (bottom left)
            qb3[i * quarter_dim + j] = mb[(half_dim + i) * n + j];
            
            // Copy elements to the fourth submatrix (bottom right)
            qb4[i * quarter_dim + j] = mb[(half_dim + i) * n + half_dim + j];
        }
    }

    // Initialize result matrices
    int* p1 = new int[quarter_dim * quarter_dim];
    int* p2 = new int[quarter_dim * quarter_dim];
    int* p3 = new int[quarter_dim * quarter_dim];
    int* p4 = new int[quarter_dim * quarter_dim];
    int* p5 = new int[quarter_dim * quarter_dim];
    int* p6 = new int[quarter_dim * quarter_dim];
    int* p7 = new int[quarter_dim * quarter_dim];
    int* tmp1 = new int[quarter_dim * quarter_dim];
    int* tmp2 = new int[quarter_dim * quarter_dim];
    int* tmp3 = new int[quarter_dim * quarter_dim];
    int* tmp4 = new int[quarter_dim * quarter_dim];
    int* tmp5 = new int[quarter_dim * quarter_dim];
    
    // Initialize result quarter matrices
    int* result1 = new int[quarter_dim * quarter_dim];
    int* result2 = new int[quarter_dim * quarter_dim];
    int* result3 = new int[quarter_dim * quarter_dim];
    int* result4 = new int[quarter_dim * quarter_dim];

    // Perform intermediate multiplications and recursive calls
    strassen(half_dim, qa1, matrixAdd(quarter_dim, qb3, qb4, p1, false), p1);
    strassen(half_dim, matrixAdd(quarter_dim, qa1, qa2, tmp3), qb4, p2);
    strassen(half_dim, matrixAdd(quarter_dim, qa3, qa4, tmp3), qb1, p3);
    strassen(half_dim, qa4, matrixAdd(quarter_dim, qb3, qb1, tmp3, false), p4);
    strassen(half_dim, matrixAdd(quarter_dim, qa1, qa4, tmp3), matrixAdd(quarter_dim, qb1, qb4, tmp3), p5);
    strassen(half_dim, matrixAdd(quarter_dim, qa2, qa4, tmp3, false), matrixAdd(quarter_dim, qb3, qb4, tmp3), p6);
    strassen(half_dim, matrixAdd(quarter_dim, qa1, qa3, tmp3, false), matrixAdd(quarter_dim, qb1, qb2, tmp3), p7);
    
    // Calculate the values of the submatrices that make up the result
    //TODO
    
    // Copy the submatrices back into the output matrix
    for (int i = 0; i < half_dim; i++) {
        for (int j = 0; j < half_dim; j++) {
            result[i * dim + j] = result1[i * quarter_dim + j];
            result[i * dim + half_dim + j] = result2[i * quarter_dim + j];
            result[(half_dim + i) * dim + j] = result3[i * quarter_dim + j];
            result[(half_dim + i) * dim + half_dim + j] = result4[i * quarter_dim + j];
        }
    }
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

    // if (inputfile[0] != '\0') {
    //     printMatrix(asciiToMatrices(inputfile, dimension).first, dimension);
    //     cout << "\n\n";
    //     printMatrix(asciiToMatrices(inputfile, dimension).second, dimension);
    // }

    int *result = new int[dimension * dimension];
    getRandomMatrix(dimension, result, 0, 2);
    printMatrix(result, dimension);
    delete[] result;

    return 0;
}
