#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <queue>
#include <map>

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

vector<int> karmarkarKarp(const vector<int>& a) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, less<pair<int, int>>> max_heap;
    map<int, int> sign_mapping;

    // Initialize priority queue for inputted list of numbers (a)
    for (int i = 0; i < a.size(); i++) {
        max_heap.push({a[i], i});
        sign_mapping[i] = 1;
    }

    // Perform Karmar-Karp algorithm on priority queue
    while (max_heap.size() > 1) {
        auto largest = max_heap.top();
        max_heap.pop();
        auto second_largest = max_heap.top();
        max_heap.pop();

        int diff = largest.first - second_largest.first;
        int largest_idx = largest.second;
        int second_largest_idx = second_largest.second;

        // Set the signs for the corresponding partitions
        sign_mapping[largest_idx] = -sign_mapping[second_largest_idx];
        max_heap.push({diff, largest_idx});
    }

    // Initialize list to return the numbers with their corresponding sign
    vector<int> result(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = a[i] * sign_mapping[i];
    }

    return result;
}

int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int algorithm = atoi(argv[2]);
    char* inputfile = argv[3];

    return 0;
}
