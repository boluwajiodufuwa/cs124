#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
// #include "tests.h"
// #include "experiment.h"

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
// int prob should always be 1 unless this func is being used for triangle experiment
void getRandomMatrix(int dimension, int *result, int start_range, int end_range, int prob) {
    // unfirom random generators
    random_device rand_dev;
    default_random_engine gen(rand_dev());
    uniform_int_distribution<int> rand_int(start_range, end_range);
    uniform_real_distribution<double> rand_pct(0.0, 100.0);

    for (int r = 0; r < dimension; r++) {
        for (int c = 0; c < dimension; c++) {
            if (prob == 0) {
                // Adding random int value to matrix
                int val = rand_int(gen);
                setMatrixValue(result, r, c, dimension, val);
            } else {
                // Adding edges between nodes with a specific probability (used for triangle experiment)
                int val = rand_pct(gen) <= prob ? 1 : 0;
                setMatrixValue(result, r, c, dimension, val);
            }
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

// Helper function to add two square matrices
int* matrixAdd(int dim, int* ma, int* mb, int* result, bool add = true) {
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
    return result;
}

// Helper function used to add an additional rightmost column and additional bottom row to odd-sized matrices
int* padMatrix(int* matrix, int* padded, int dim) {
    // Copy values from original matrix to padded matrix
    for (int i = 0; i < dim+1; i++) {
        for (int j = 0; j < dim+1; j++) {
            if (i == dim || j == dim) {
                padded[i*(dim+1) + j] = 0;
            }
            else {
                padded[i*(dim+1) + j] = matrix[i*dim + j];
            }
        }
    }
    return padded;
}

// Helper function used to add an additional rightmost column and additional bottom row to odd-sized matrices
int* unpadMatrix(int* matrix, int* unpadded, int dim) {
    // Copy values from original matrix to padded matrix
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            unpadded[i*(dim) + j] = matrix[i*(dim+1) + j];
        }
    }
    return unpadded;
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

// Strassens algorithm that takes in a crossover
void strassen(int dim, int* ma, int* mb, int* result, int crossover) {
    // Base Case
    if (dim <= crossover) {
        conventional(dim, ma, mb, result);
    }
    else if (dim % 2 != 0) {
        // ODD DIMENSION CASE
        // Initialize space for  new padded matrices
        int* pad_ma = new int[(dim+1) * (dim+1)];
        int* pad_mb = new int[(dim+1) * (dim+1)];

        // Pad matrices using padding function
        pad_ma = padMatrix(ma, pad_ma, dim);
        pad_mb = padMatrix(mb, pad_mb, dim);

        int half_dim = (dim+1) / 2;
        int* qa1 = new int[half_dim * half_dim];
        int* qa2 = new int[half_dim * half_dim];
        int* qa3 = new int[half_dim * half_dim];
        int* qa4 = new int[half_dim * half_dim];
        int* qb1 = new int[half_dim * half_dim];
        int* qb2 = new int[half_dim * half_dim];
        int* qb3 = new int[half_dim * half_dim];
        int* qb4 = new int[half_dim * half_dim];
        
        
        // Copy the elements of the input matrices to each submatrix
        for (int i = 0; i < half_dim; i++) {
            for (int j = 0; j < half_dim; j++) {
                // MATRIX MA
                // Copy elements to the first submatrix (top left)
                qa1[i * half_dim + j] = pad_ma[i * (dim+1) + j];
                
                // Copy elements to the second submatrix (bottom left)
                qa3[i * half_dim + j] = pad_ma[i * (dim+1) + half_dim + j];
                
                // Copy elements to the third submatrix (top right)
                qa2[i * half_dim + j] = pad_ma[(half_dim + i) * (dim+1) + j];
                
                // Copy elements to the fourth submatrix (bottom right)
                qa4[i * half_dim + j] = pad_ma[(half_dim + i) * (dim+1) + half_dim + j];

                // MATRIX MB
                // Copy elements to the first submatrix (top left)
                qb1[i * half_dim + j] = pad_mb[i * (dim+1) + j];
                
                // Copy elements to the second submatrix (bottom left)
                qb3[i * half_dim + j] = pad_mb[i * (dim+1) + half_dim + j];
                
                // Copy elements to the third submatrix (top right)
                qb2[i * half_dim + j] = pad_mb[(half_dim + i) * (dim+1) + j];
                
                // Copy elements to the fourth submatrix (bottom right)
                qb4[i * half_dim + j] = pad_mb[(half_dim + i) * (dim+1) + half_dim + j];
            }
        }

        delete[] pad_mb;
        delete[] pad_ma;
        
        // Initialize result matrices
        int* p1 = new int[half_dim * half_dim];
        int* p2 = new int[half_dim * half_dim];
        int* p3 = new int[half_dim * half_dim];
        int* p4 = new int[half_dim * half_dim];
        int* p5 = new int[half_dim * half_dim];
        int* p6 = new int[half_dim * half_dim];
        int* p7 = new int[half_dim * half_dim];
        int* tmp0 = new int[half_dim * half_dim];
        int* tmp1 = new int[half_dim * half_dim];
        int* tmp2 = new int[half_dim * half_dim];
        int* tmp3 = new int[half_dim * half_dim];
        int* tmp4 = new int[half_dim * half_dim];
        int* tmp5 = new int[half_dim * half_dim];
        int* tmp6 = new int[half_dim * half_dim];
        int* tmp7 = new int[half_dim * half_dim];
        int* tmp8 = new int[half_dim * half_dim];
        int* tmp9 = new int[half_dim * half_dim];
        int* tmp10 = new int[half_dim * half_dim];
        int* tmp11 = new int[half_dim * half_dim];
        int* tmp12 = new int[half_dim * half_dim];
        int* tmp13 = new int[half_dim * half_dim];
        
        // Initialize result quarter matrices
        int* result1 = new int[half_dim * half_dim];
        int* result2 = new int[half_dim * half_dim];
        int* result3 = new int[half_dim * half_dim];
        int* result4 = new int[half_dim * half_dim];

        // Perform intermediate multiplications and recursive calls
        strassen(half_dim, qa1, matrixAdd(half_dim, qb2, qb4, tmp0, false), p1, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa1, qa2, tmp1), qb4, p2, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa3, qa4, tmp2), qb1, p3, crossover);
        strassen(half_dim, qa4, matrixAdd(half_dim, qb3, qb1, tmp3, false), p4, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa1, qa4, tmp4), matrixAdd(half_dim, qb1, qb4, tmp5), p5, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa2, qa4, tmp6, false), matrixAdd(half_dim, qb3, qb4, tmp7), p6, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa1, qa3, tmp8, false), matrixAdd(half_dim, qb1, qb2, tmp9), p7, crossover);
        
        
        // Calculate the values of the submatrices that make up the result
        matrixAdd(half_dim, matrixAdd(half_dim, p5, p4, tmp10), matrixAdd(half_dim, p2, p6, tmp11, false), result1, false);
        matrixAdd(half_dim, p1, p2, result2);
        matrixAdd(half_dim, p3, p4, result3);
        matrixAdd(half_dim, matrixAdd(half_dim, p1, p5, tmp12), matrixAdd(half_dim, p3, p7, tmp13), result4, false);


        delete[] qa1;
        delete[] qa2;
        delete[] qa3;
        delete[] qa4;
        delete[] qb1;
        delete[] qb2;
        delete[] qb3;
        delete[] qb4;
        delete[] p1;
        delete[] p2;
        delete[] p3;
        delete[] p4;
        delete[] p5;
        delete[] p6;
        delete[] p7;
        delete[] tmp0;
        delete[] tmp1;
        delete[] tmp2;
        delete[] tmp3;
        delete[] tmp4;
        delete[] tmp5;
        delete[] tmp6;
        delete[] tmp7;
        delete[] tmp8;
        delete[] tmp9;
        delete[] tmp10;
        delete[] tmp11;
        delete[] tmp12;
        delete[] tmp13;

        int* newMatrix = new int[(dim+1) * (dim+1)];
        
        // Copy the submatrices back into the output matrix
        for (int i = 0; i < half_dim; i++) {
            for (int j = 0; j < half_dim; j++) {
                newMatrix[i * (dim+1) + j] = result1[i * half_dim + j];
                newMatrix[i * (dim+1) + half_dim + j] = result3[i * half_dim + j];
                newMatrix[(half_dim + i) * (dim+1) + j] = result2[i * half_dim + j];
                newMatrix[(half_dim + i) * (dim+1) + half_dim + j] = result4[i * half_dim + j];
            }
        }
        unpadMatrix(newMatrix, result, dim);

        delete[] newMatrix;
        delete[] result1;
        delete[] result2;
        delete[] result3;
        delete[] result4;
    }
    else {
        // EVEN DIMENSION CASE
        // STEP 1: Splits the two n by n matrices into 4 quarter submatrices (currently functional for n=2^k)
        // Calculate the size of each submatrix (same dim for both matrices)
        // Initialize quarter matrices
        int half_dim = dim / 2;
        int* qa1 = new int[half_dim * half_dim];
        int* qa2 = new int[half_dim * half_dim];
        int* qa3 = new int[half_dim * half_dim];
        int* qa4 = new int[half_dim * half_dim];
        int* qb1 = new int[half_dim * half_dim];
        int* qb2 = new int[half_dim * half_dim];
        int* qb3 = new int[half_dim * half_dim];
        int* qb4 = new int[half_dim * half_dim];
        
        // Copy the elements of the input matrices to each submatrix
        for (int i = 0; i < half_dim; i++) {
            for (int j = 0; j < half_dim; j++) {
                // MATRIX MA
                // Copy elements to the first submatrix (top left)
                qa1[i * half_dim + j] = ma[i * dim + j];
                
                // Copy elements to the second submatrix (bottom left)
                qa3[i * half_dim + j] = ma[i * dim + half_dim + j];
                
                // Copy elements to the third submatrix (top right)
                qa2[i * half_dim + j] = ma[(half_dim + i) * dim + j];
                
                // Copy elements to the fourth submatrix (bottom right)
                qa4[i * half_dim + j] = ma[(half_dim + i) * dim + half_dim + j];

                // MATRIX MB
                // Copy elements to the first submatrix (top left)
                qb1[i * half_dim + j] = mb[i * dim + j];
                
                // Copy elements to the second submatrix (bottom left)
                qb3[i * half_dim + j] = mb[i * dim + half_dim + j];
                
                // Copy elements to the third submatrix (top right)
                qb2[i * half_dim + j] = mb[(half_dim + i) * dim + j];
                
                // Copy elements to the fourth submatrix (bottom right)
                qb4[i * half_dim + j] = mb[(half_dim + i) * dim + half_dim + j];
            }
        }
        

        // Initialize result matrices
        int* p1 = new int[half_dim * half_dim];
        int* p2 = new int[half_dim * half_dim];
        int* p3 = new int[half_dim * half_dim];
        int* p4 = new int[half_dim * half_dim];
        int* p5 = new int[half_dim * half_dim];
        int* p6 = new int[half_dim * half_dim];
        int* p7 = new int[half_dim * half_dim];
        int* tmp0 = new int[half_dim * half_dim];
        int* tmp1 = new int[half_dim * half_dim];
        int* tmp2 = new int[half_dim * half_dim];
        int* tmp3 = new int[half_dim * half_dim];
        int* tmp4 = new int[half_dim * half_dim];
        int* tmp5 = new int[half_dim * half_dim];
        int* tmp6 = new int[half_dim * half_dim];
        int* tmp7 = new int[half_dim * half_dim];
        int* tmp8 = new int[half_dim * half_dim];
        int* tmp9 = new int[half_dim * half_dim];
        int* tmp10 = new int[half_dim * half_dim];
        int* tmp11 = new int[half_dim * half_dim];
        int* tmp12 = new int[half_dim * half_dim];
        int* tmp13 = new int[half_dim * half_dim];
        
        // Initialize result quarter matrices
        int* result1 = new int[half_dim * half_dim];
        int* result2 = new int[half_dim * half_dim];
        int* result3 = new int[half_dim * half_dim];
        int* result4 = new int[half_dim * half_dim];

        // Perform intermediate multiplications and recursive calls
        strassen(half_dim, qa1, matrixAdd(half_dim, qb2, qb4, tmp0, false), p1, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa1, qa2, tmp1), qb4, p2, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa3, qa4, tmp2), qb1, p3, crossover);
        strassen(half_dim, qa4, matrixAdd(half_dim, qb3, qb1, tmp3, false), p4, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa1, qa4, tmp4), matrixAdd(half_dim, qb1, qb4, tmp5), p5, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa2, qa4, tmp6, false), matrixAdd(half_dim, qb3, qb4, tmp7), p6, crossover);
        strassen(half_dim, matrixAdd(half_dim, qa1, qa3, tmp8, false), matrixAdd(half_dim, qb1, qb2, tmp9), p7, crossover);
        
        // Calculate the values of the submatrices that make up the result
        matrixAdd(half_dim, matrixAdd(half_dim, p5, p4, tmp10), matrixAdd(half_dim, p2, p6, tmp11, false), result1, false);
        matrixAdd(half_dim, p1, p2, result2);
        matrixAdd(half_dim, p3, p4, result3);
        matrixAdd(half_dim, matrixAdd(half_dim, p1, p5, tmp12), matrixAdd(half_dim, p3, p7, tmp13), result4, false);
        
        delete[] qa1;
        delete[] qa2;
        delete[] qa3;
        delete[] qa4;
        delete[] qb1;
        delete[] qb2;
        delete[] qb3;
        delete[] qb4;
        delete[] p1;
        delete[] p2;
        delete[] p3;
        delete[] p4;
        delete[] p5;
        delete[] p6;
        delete[] p7;
        delete[] tmp0;
        delete[] tmp1;
        delete[] tmp2;
        delete[] tmp3;
        delete[] tmp4;
        delete[] tmp5;
        delete[] tmp6;
        delete[] tmp7;
        delete[] tmp8;
        delete[] tmp9;
        delete[] tmp10;
        delete[] tmp11;
        delete[] tmp12;
        delete[] tmp13;

        // Copy the submatrices back into the output matrix
        for (int i = 0; i < half_dim; i++) {
            for (int j = 0; j < half_dim; j++) {
                result[i * dim + j] = result1[i * half_dim + j];
                result[i * dim + half_dim + j] = result3[i * half_dim + j];
                result[(half_dim + i) * dim + j] = result2[i * half_dim + j];
                result[(half_dim + i) * dim + half_dim + j] = result4[i * half_dim + j];
            }
        }

        delete[] result1;
        delete[] result2;
        delete[] result3;
        delete[] result4;
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
void asciiToProduct(char* fn, int dim, int *result) {
    pair<int*, int*> matrices = asciiToMatrices(fn, dim);
    int opt_crossover = 10;

    strassen(dim, matrices.first, matrices.second, result, opt_crossover);

    for (int i = 0; i < dim; i++) {
        cout << getMatrixValue(result, i, i, dim) << "\n";
    }
}

// ----------------------------------------------------------------
// Main
// ----------------------------------------------------------------

int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int dimension = atoi(argv[2]);
    char* inputfile = argv[3];

    int* result = new int[dimension * dimension];

    if (do_tests == 1) {
        // runTriangleExperiment();
    } else {
        asciiToProduct(inputfile, dimension, result);
    }

    delete[] result;

    return 0;
}

