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

bool test_karmarkarKarp() {
    vector<int64_t> sequence = {10,8,7,6,5};
    int64_t correct_residue = 2;

    if(karmarkarKarp(sequence) != correct_residue) {
        function_failed("test_karmarkarKarp", __LINE__);
        cout << karmarkarKarp(sequence) << "\n";
        return false;
    }

    sequence = {345678901234, 987654321012, 123456789876, 987654322111, 456789012345};
    correct_residue = 12346677666;

    if(karmarkarKarp(sequence) != correct_residue) {
        function_failed("test_karmarkarKarp", __LINE__);
        cout << karmarkarKarp(sequence) << endl;
        return false;
    }

    return true;
}

int run_tests() {
    cout << "Running tests\n" << endl;

    bool all_tests_passed = true;
    all_tests_passed = all_tests_passed && test_karmarkarKarp();

    if (all_tests_passed) {
        cout << "All tests have been passed successfully!" << endl;
    }
}
