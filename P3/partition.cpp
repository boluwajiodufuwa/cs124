#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// Note that solutions of any form and inputs/sequences are just represented by regular vectors

// ----------------------------------------------------------------
// Miscellaneous functions
// ----------------------------------------------------------------

// Takes in a solution in the standard form (1s and -1s) and a sequence and returns residue
int residue(vector<int> solution, vector<int> seq) {
    int residue = 0;
    for (int i = 0; i < solution.size(); i++) {
        residue += solution[i] * seq[i];
    }
    return residue;
}

// ----------------------------------------------------------------
// Heuristic Functions
// ----------------------------------------------------------------

// ----------------------------------------------------------------
// Main
// ----------------------------------------------------------------

int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int algorithm = atoi(argv[2]);
    char* inputfile = argv[3];

    return 0;
}
