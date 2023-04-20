#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "partition.h"

void function_failed(string function, int line) {
    cout << function << " failed on line " << line << endl;
}

bool test_residue() {
    vector<int64_t> sequence = {345678901234, 987654321012, 123456789876, 987654322111, 456789012345};
    vector<int64_t> solution = {1,1,1,-1,-1};
    int64_t correct_residue = 12346677666;

    if(residue(solution, sequence) != correct_residue) {
        function_failed("test_residue", __LINE__);
    }
}

bool test_karmarkarKarp() {
    vector<int64_t> sequence = {10,8,7,6,5};
    int64_t correct_residue = 2;

    if(residue(karmarkarKarp(sequence), sequence) != correct_residue) {
        function_failed("test_karmarkarKarp", __LINE__);
    }

    sequence = {345678901234, 987654321012, 123456789876, 987654322111, 456789012345};
    correct_residue = 529134463345;

    if(residue(karmarkarKarp(sequence), sequence) != correct_residue) {
        function_failed("test_karmarkarKarp", __LINE__);
    }
}

int run_tests() {
    cout << "Running tests\n" << endl;

    bool all_tests_passed = true;
    all_tests_passed = all_tests_passed && test_karmarkarKarp();

    if (all_tests_passed) {
        cout << "All tests have been passed successfully!\n" << endl;
    }
}
