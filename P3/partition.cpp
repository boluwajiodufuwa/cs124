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

// Karmar-Karp algorithm
// Takes in input of a list of nonnegative integers and outputs a list of signs to represent the partition that results in the minimum guaranteed partition
vector<int> karmarkarKarp(vector<int> nums) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, less<pair<int, int>>> max_heap;
    vector <int> sign_mapping;

    // Initialize priority queue for inputted list of numbers (a)
    for (int i = 0; i < nums.size(); i++) {
        max_heap.push({nums[i], i});
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

    // Return list of signs (1 or -1) for corresponding partitions
    return sign_mapping;
}

// Repeated Random
//
vector<int> repeatedRandom(vector<int> nums) {

}

// Hill Climbing
//
vector<int> hillClimbing(vector<int> nums) {

}

// Simulated Annealing
//
vector<int> simulatedAnnealing(vector<int> nums) {

}

// ----------------------------------------------------------------
// Main
// ----------------------------------------------------------------
int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int algorithm = atoi(argv[2]);
    char* inputfile = argv[3];

    return 0;
}
