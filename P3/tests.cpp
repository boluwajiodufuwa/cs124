#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

void function_failed(string function, int line) {
    cout << function << " failed on line " << line << endl;
}

bool test_karmarkarKarp() {
}

int run_tests() {
    cout << "Running tests\n" << endl;

    bool all_tests_passed = true;
    all_tests_passed = all_tests_passed && test_karmarkarKarp();

    if (all_tests_passed) {
        cout << "All tests have been passed successfully!\n" << endl;
    }
}
