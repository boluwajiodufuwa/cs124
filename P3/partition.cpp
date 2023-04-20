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

// Function used for prepartitioned functions
vector<int64_t> partition(const vector<int64_t>& A, vector<int64_t> P = {}) {
    size_t sz = A.size();

    if (P.empty()) {
        srand(std::time(0));
        P.resize(sz);
        for (size_t i = 0; i < sz; i++) {
            P[i] = std::rand() % sz;
        }
    }

    std::vector<int64_t> A_prime(sz, 0);

    for (size_t j = 0; j < sz; j++) {
        A_prime[P[j]] += A[j];
    }

    return A_prime;
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
int karmarkarKarp(vector<int64_t>& nums) {
    // Initialize priority queue for inputted list of numbers (a)   
    priority_queue<int64_t> max_heap;
    
    for (int i = 0; i < nums.size(); i++) {
        max_heap.push(nums[i]);
    }

    int64_t max1 = 0;
    int64_t max2 = 0;
 
    while (max2 != 0) {
        max1 = max_heap.pop();
        max2 = max_heap.pop();
        max_heap.push(abs(max1 - max2));
        max_heap.push(0);
    }

    return max1;
}

// Repeated Random
// Takes list of nonnegative integers and randomly generates/replaces solutions until we've reached last iteration
int repeatedRandom(vector<int64_t>& nums, int max_iter = 25000) {
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

    int solution = accumulate(nums.begin(), nums.end(), 0);
    srand(std::time(0));

    for (int i = 0; i < max_iter; i++) {
        int residueSp = 0;
        for (int elt : nums) {
            residueSp += elt * (rand() % 2 == 0 ? 1 : -1);
        }
        residueSp = abs(residueSp);
        best = min(best, residueSp);
    }

    return best;
}



int repeat_rand(const std::vector<int>& A, int max_iter = 25000) {
    int best = std::accumulate(A.begin(), A.end(), 0);
    std::srand(std::time(0));

    for (int i = 0; i < max_iter; i++) {
        int residueSp = 0;
        for (int elt : A) {
            residueSp += elt * (std::rand() % 2 == 0 ? 1 : -1);
        }
        residueSp = abs(residueSp);
        best = std::min(best, residueSp);
    }

    return best;
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

// ----------------------------------------------------------------
// Prepartitioned Functions
// ----------------------------------------------------------------
int prepartRepeatRand(const std::vector<int64_t>& A, int max_iter = 25000) {
    double best = std::numeric_limits<double>::infinity();

    std::srand(std::time(0));

    for (int i = 0; i < max_iter; ++i) {
        vector<int64_t> new_A = partition(A);
        int sol = karmarkarKarp(new_A);
        best = std::min(best, static_cast<double>(sol));
    }

    return static_cast<int>(best);
}

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
