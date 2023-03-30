#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "strassen.h"

void function_failed(string function, int line) {
    cout << function << " failed on line " << line << endl;
}

bool test_conventional() {
    int dim = 2;
    int *ma = new int[dim*dim];
    instantiateMatrix(ma, (int[]) {0,1,2,3}, dim);
    int *mb = new int[dim*dim];
    instantiateMatrix(mb, (int[]) {4,5,6,7}, dim);
    int *test_solution = new int[dim*dim];
    instantiateMatrix(test_solution, (int[]) {10,19,14,27}, dim);
    int *result = new int[dim*dim];

    conventional(dim, ma, mb, result);

    if (!compareMatrices(result, test_solution, dim)) {
        cout << result[0] << endl;
        cout << test_solution[0] << endl;  
        function_failed("test_conventional", __LINE__);
        return false;
    }

    instantiateMatrix(ma, (int[]) {0,6,2,200}, dim);
    instantiateMatrix(mb, (int[]) {4,9,6,-8}, dim);
    instantiateMatrix(test_solution, (int[]) {18,1824,-16,-1564}, dim);

    conventional(dim, ma, mb, result);

    if (!compareMatrices(result, test_solution, dim)) {
        cout << result[1] << endl;
        cout << test_solution[1] << endl;  
        function_failed("test_conventional", __LINE__);
        return false;
    }

    delete[] ma;
    delete[] mb;
    delete[] test_solution;
    delete[] result;

    dim = 4;
    int *ma2 = new int[dim*dim];
    int *mb2 = new int[dim*dim];
    int *result2 = new int[dim*dim];
    int *test_solution2 = new int[dim*dim];
    instantiateMatrix(ma2, (int[]) {3,4,0,2,3,4,31,2,9,-1,22,3,4,1,2,3}, dim);
    instantiateMatrix(mb2, (int[]) {8,3,4,2,9,22,3,-33,6,11,1,900,4,1,2,3}, dim);
    instantiateMatrix(test_solution2, (int[]) {77,42,185,40,-12,88,682,-28,3660,967,2163,2737,45,21,81,25}, dim);

    conventional(dim, ma2, mb2, result2);

    if (!compareMatrices(result2, test_solution2, dim)) {
        printMatrix(ma2, dim);
        printMatrix(mb, dim);
        printMatrix(result, dim);

        function_failed("test_conventional", __LINE__);
        return false;
    }

    delete[] ma2;
    delete[] mb2;
    delete[] test_solution2;
    delete[] result2;

    return true;
}

bool test_strassen() {
    int dim = 2;
    int *ma = new int[dim*dim];
    instantiateMatrix(ma, (int[]) {0,1,2,3}, dim);
    int *mb = new int[dim*dim];
    instantiateMatrix(mb, (int[]) {4,5,6,7}, dim);
    int *test_solution = new int[dim*dim];
    instantiateMatrix(test_solution, (int[]) {10,19,14,27}, dim);
    int *result = new int[dim*dim];

    strassen(dim, ma, mb, result, 1);

    if (!compareMatrices(result, test_solution, dim)) {
        cout << result[0] << endl;
        cout << test_solution[0] << endl;  
        function_failed("test_strassen", __LINE__);
        return false;
    }

    instantiateMatrix(ma, (int[]) {0,6,2,200}, dim);
    instantiateMatrix(mb, (int[]) {4,9,6,-8}, dim);
    instantiateMatrix(test_solution, (int[]) {18,1824,-16,-1564}, dim);

    strassen(dim, ma, mb, result,1);

    if (!compareMatrices(result, test_solution, dim)) {
        cout << result[1] << endl;
        cout << test_solution[1] << endl;  
        function_failed("test_strassen", __LINE__);
        return false;
    }

    delete[] ma;
    delete[] mb;
    delete[] test_solution;
    delete[] result;

    dim = 4;
    int *ma2 = new int[dim*dim];
    int *mb2 = new int[dim*dim];
    int *result2 = new int[dim*dim];
    int *test_solution2 = new int[dim*dim];
    instantiateMatrix(ma2, (int[]) {3,4,0,2,3,4,31,2,9,-1,22,3,4,1,2,3}, dim);
    instantiateMatrix(mb2, (int[]) {8,3,4,2,9,22,3,-33,6,11,1,900,4,1,2,3}, dim);
    instantiateMatrix(test_solution2, (int[]) {77,42,185,40,-12,88,682,-28,3660,967,2163,2737,45,21,81,25}, dim);

    strassen(dim, ma2, mb2, result2,1);

    if (!compareMatrices(result2, test_solution2, dim)) {
        printMatrix(ma2, dim);
        printMatrix(mb, dim);
        printMatrix(result, dim);

        function_failed("test_strassen", __LINE__);
        return false;
    }

    delete[] ma2;
    delete[] mb2;
    delete[] test_solution2;
    delete[] result2;

    return true;
}

int run_tests() {
    cout << "Running tests\n" << endl;

    bool all_tests_passed = true;
    all_tests_passed = all_tests_passed && test_conventional();
    all_tests_passed = all_tests_passed && test_strassen();

    if (all_tests_passed) {
        cout << "All tests have been passed successfully!\n" << endl;
    }
}
