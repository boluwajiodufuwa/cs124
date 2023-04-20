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
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "tests.h"

using namespace std;

// Note that solutions of any form are just represented by regular vectors
// Inputs/sequences are vectors of 64 bit ints "int64_t"

// ----------------------------------------------------------------
// Miscellaneous functions
// ----------------------------------------------------------------

int MAX_ITER = 1000;

// Power function for large numbers
int64_t power(int64_t base, int64_t exponent) {
    int64_t result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Takes in a solution in the standard form (1s and -1s) and a sequence and returns residue
int64_t residue(vector<int64_t> solution, vector<int64_t> seq) {
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

// Generates random list of bits of length n to be used as potential solution for partition
vector<int64_t> random_bits(int64_t n) {
    vector<int64_t> bits(n);
    random_device rd;

    // Seed the random number generator with the random device
    mt19937 rng(rd()); 

    // Define a uniform distribution for generating random bits
    uniform_int_distribution<int> dist(0, 1); 

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
vector<int64_t> karmarkarKarp(vector<int64_t> nums) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, less<pair<int, int>>> max_heap;
    vector<int64_t> sign_mapping;

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
// Takes list of nonnegative integers and randomly generates/replaces solutions until we've reached last iteration
vector<int64_t> repeatedRandom(vector<int64_t> nums) {
    // Create a random solution 
    int n = nums.size();
    vector<int64_t> solution = random_bits(n);

    for (int i = 0; i < MAX_ITER; i++) {
        // Initialize a different random solution and substitute it if its residue is lower
        vector<int64_t> curr_rand = random_bits(n);
        if (residue(curr_rand, nums) < residue(solution, nums)) {
            solution = curr_rand;
        }

    }   

    return solution;
}


// Hill Climbing
// According to ed, max_iter should be 25k
// Takes list of nonnegative integers and randomly generates a solution and finds the most optimal neighboring solution until we've reached last iteration
int hillClimbing(const vector<int64_t>& nums) {
    // Randomized solution creation
    int n = nums.size();
    vector<int64_t> solution = random_bits(n);
    int best_sol = abs(accumulate(solution.begin(), solution.end(), 0));

    for (int iter = 0; iter < MAX_ITER; iter++) {
        int i1 = rand() % n;
        int i2 = i1;
        while (i2 == i1) {
            i2 = rand() % n;
        }

        int si1 = solution[i1];
        int si2 = solution[i2];

        int new_sol = best_sol - si1 - si1;

        if (static_cast<float>(rand()) / RAND_MAX < 0.5) {
            new_sol -= si2 + si2;
        }

        new_sol = abs(new_sol);

        if (new_sol < best_sol) {
            solution[i1] = -si1;
            if (static_cast<float>(rand()) / RAND_MAX < 0.5) {
                solution[i2] = -si2;
            }
        }
    }

    return best_sol;
}

bool anneal_prob(int res_Sp, int res_S, int iter) {
    double T_iter = std::pow(10, 10) * std::pow(0.8, std::floor((iter + 1) / 300.0));
    double exponent = -(static_cast<double>(res_Sp) - res_S) / T_iter;

    srand(std::time(0));
    return (static_cast<double>(std::rand()) / RAND_MAX) < std::exp(exponent);
}

int simulatedAnnealing(const std::vector<int64_t>& nums, int max_iter = 25000) {
    // Randomized solution creation
    int n = nums.size();
    vector<int64_t> solution = random_bits(n);

    int curr_sol = abs(accumulate(solution.begin(), solution.end(), 0));
    int best_sol = curr_sol;

    for (int i = 0; i < max_iter; i++) {
        int i1 = rand() % n;
        int i2 = i1;
        while (i2 == i1) {
            i2 = rand() % n;
        }

        int si1 = solution[i1];
        int si2 = solution[i2];

        int new_sol = curr_sol - si1 - si1;

        if (static_cast<float>(rand()) / RAND_MAX < 0.5) {
            new_sol -= si2 + si2;
        }

        new_sol = abs(new_sol);

        if (new_sol < curr_sol || anneal_prob(new_sol, curr_sol, i)) {
            S[i1] = -si1;
            if (static_cast<float>(rand()) / RAND_MAX < 0.5) {
                S[i2] = -si2;
            }
            curr_sol = new_sol;
        }

        best_sol = min(best_sol, new_sol);
    }

    return best_sol;
}

// Simulated Annealing
//
vector<int64_t> simulatedAnnealing(vector<int64_t> nums) {

}

// ----------------------------------------------------------------
// Prepartitioned Functions
// ----------------------------------------------------------------


// ----------------------------------------------------------------
// Main
// ----------------------------------------------------------------
int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int algorithm = atoi(argv[2]);
    char* inputfile = argv[3];

    if (do_tests == 1) {
        run_tests();
    }

    return 0;
}
