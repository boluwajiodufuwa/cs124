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
    
    return true;
    delete[] ma;
    delete[] mb;
    delete[] test_solution;
    delete[] result;
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

    strassen(dim, ma, mb, result);

    if (!compareMatrices(result, test_solution, dim)) {
        cout << result[0] << endl;
        cout << test_solution[0] << endl;  
        function_failed("test_strassen", __LINE__);
        return false;
    }

    instantiateMatrix(ma, (int[]) {0,6,2,200}, dim);
    instantiateMatrix(mb, (int[]) {4,9,6,-8}, dim);
    instantiateMatrix(test_solution, (int[]) {18,1824,-16,-1564}, dim);

    strassen(dim, ma, mb, result);

    if (!compareMatrices(result, test_solution, dim)) {
        cout << result[1] << endl;
        cout << test_solution[1] << endl;  
        function_failed("test_strassen", __LINE__);
        return false;
    }
    
    return true;
    delete[] ma;
    delete[] mb;
    delete[] test_solution;
    delete[] result;
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
