#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <cstdint>

using namespace std;

// Note that solutions of any form are just represented by regular vectors
// Inputs/sequences are vectors of 64 bit ints "int64_t"

// ----------------------------------------------------------------
// Miscellaneous functions
// ----------------------------------------------------------------

// Power function for large numbers
int64_t power(int64_t base, int64_t exponent) {
    int64_t result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Takes in a solution in the standard form (1s and -1s) and a sequence and returns residue
int64_t residue(vector<int> solution, vector<int64_t> seq) {
    int64_t residue = 0;
    for (int i = 0; i < solution.size(); i++) {
        residue += solution[i] * seq[i];
    }
    return residue;
}

// Creates a random sequence of 100 numbers in the range [0, 10^12]
vector<int64_t> getRandomSequence() {
    // unfirom random generators
    random_device rand_dev;
    default_random_engine gen(rand_dev());
    uniform_int_distribution<int64_t> rand_int(1, power(10, 12));

    vector<int64_t> result;

    // Adding random values to result sequence
    for (int i = 0; i < 100; i++) {
        int val = rand_int(gen);
        result.push_back(val);
    }

    return result;
}

vector<int64_t> asciiToSequence(char* fn) {
    // Open the file using the filename from the command line argument
    ifstream file(fn);

    // Check that the file was opened successfully
    if (!file.is_open()) {
        cerr << "Error: could not open file " << fn << "\n";
    }

    // Result sequence
    vector<int64_t> result;

    // Read content of files into matrices
    string line;
    while (getline(file, line)) {
        int64_t val = stoll(line);
        result.push_back(val);
    }

    // Close the file
    file.close();

    return result;

}

std::vector<int> random_bits(int n) {
    std::vector<int> bits(n);
    std::random_device rd;

    // Seed the random number generator with the random device
    std::mt19937 rng(rd()); 

    // Define a uniform distribution for generating random bits
    std::uniform_int_distribution<int> dist(0, 1); 

    for (int i = 0; i < n; ++i) {
        bits[i] = dist(rng) == 0 ? -1 : 1;
    }

    return bits;
}

// ----------------------------------------------------------------
// Heuristic Functions
// ----------------------------------------------------------------

// Karmar-Karp algorithm
// Takes in input of a list of nonnegative integers and outputs a list of signs to represent the partition that results in the minimum guaranteed partition
vector<int> karmarkarKarp(vector<int64_t> nums) {
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
// Takes list of nonnegative integers and randomly replaces lis
vector<int> repeatedRandom(vector<int64_t> nums) {
    // Create a random solution 
    int n = nums.size();
    vector<int> solution = random_bits(n);

    // Test max_iteration (TODO; WHAT IS MAX_ITER)
    int max_iter = 1000;

    for (int i = 0; i < max_iter; i++) {
        // Initialize a different random solution and substitute it if its residue is lower
        vector<int> curr_rand = random_bits(n);
        if (residue(curr_rand, nums) < residue(solution, nums)) {
            solution = curr_rand;
        }

    }   

    return solution;
}

// Hill Climbing
//
vector<int> hillClimbing(vector<int64_t> nums) {

}

// Simulated Annealing
//
vector<int> simulatedAnnealing(vector<int64_t> nums) {

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
