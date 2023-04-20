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
// #include "tests.h"

using namespace std;

// Note that solutions of any form are just represented by regular vectors
// Inputs/sequences are vectors of 64 bit ints "int64_t"

// ----------------------------------------------------------------
// Miscellaneous functions
// ----------------------------------------------------------------
// Constants and globals
int64_t MAX_ITER = 25000;
random_device rand_dev;
default_random_engine gen(rand_dev());
uniform_int_distribution<int64_t> randSeqInt(1, power(10, 12));
uniform_int_distribution<int64_t> bin_int(0, 1);


// Power function for large numbers
int64_t power(int64_t base, int64_t exponent) {
    int64_t result = 1;
    for (int64_t i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// Takes in a solution in the standard form (1s and -1s) and a sequence and returns residue
int64_t residue(vector<int64_t> solution, vector<int64_t> seq) {
    int64_t residue = 0;
    for (int64_t i = 0; i < solution.size(); i++) {
        residue += solution[i] * seq[i];
    }
    return residue;
}

// Creates a random sequence of 100 numbers in the range [0, 10^12]
vector<int64_t> getRandomSequence() {
    // unfirom random generators
    vector<int64_t> result;

    // Adding random values to result sequence
    for (int64_t i = 0; i < 100; i++) {
        int64_t val = randSeqInt(gen);
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
    uniform_int_distribution<int64_t> rand_int(1, sz-1);

    if (P.empty()) {
        P.resize(sz);
        for (size_t i = 0; i < sz; i++) {
            P[i] = rand_int(gen);
        }
    }

    vector<int64_t> A_prime(sz, 0);

    for (size_t j = 0; j < sz; j++) {
        A_prime[P[j]] += A[j];
    }

    return A_prime;
}


// Generates random list of bits of length n to be used as potential solution for partition
vector<int64_t> random_bits(int64_t n) {
    vector<int64_t> bits(n);

    for (int64_t i = 0; i < n; ++i) {
        bits[i] = bin_int(rand_dev) == 0 ? -1 : 1;
    }

    return bits;
}

// ----------------------------------------------------------------
// Heuristic Functions
// ----------------------------------------------------------------

// Karmar-Karp algorithm
int64_t karmarkarKarp(vector<int64_t>& nums) {
    // Initialize priority queue for inputted list of numbers (a)   
    priority_queue<int64_t> maxHeap(nums.begin(), nums.end());

    int64_t max1;
    int64_t max2;

    while (max2 != 0) { // Keep running until max2 is 0
        // Pop the two maximum values from the heap
        max1 = maxHeap.top();
        maxHeap.pop();
        max2 = maxHeap.top();
        maxHeap.pop();

        // Push the absolute difference of max1 and max2 back into the heap
        maxHeap.push(abs(max1 - max2));
        // Push 0 back into the heap
        maxHeap.push(0);

        cout << "Max: " << max1 << endl;
    }


    return max1;
}

// Repeated Random
// Takes list of nonnegative integers and randomly generates/replaces solutions until we've reached last iteration
int64_t repeatedRandom(vector<int64_t>& nums) {
    // Create a random solution 
    int64_t solution = accumulate(nums.begin(), nums.end(), 0);

    for (int64_t i = 0; i < MAX_ITER; i++) {
        int64_t residueSp = 0;
        for (int64_t elt : nums) {
            if (bin_int(gen) == 0) {
                residueSp -= elt;
            }
            elif (bin_int(gen) == 1) {
                residueSp += elt;
            }
        }
        residueSp = abs(residueSp);
        solution = min(solution, residueSp);
    }

    return solution;
}

int64t repeatedRandom(vector<int64 t»& nums) {
    // Create a random solution
    int64_t solution = accumulate(nums. begin(), nums.end(), 0) :
    srand(time(0));
    for (int64_t i = 0; i < MAX_ITER; i++) {
        int64_t residueSp = 0;
        for (int64_t elt : nums) {}
            residueSp += elt * (rand() % 2 == 0 ? 1 : -1);
        }
        residueSp = large_abs(residueSp);
        solution = min(solution, residueSp) < 0 ? residueSp : min (solution, residuesp);
}

// Hill Climbing
// According to ed, MAX_ITER should be 25k
// Takes list of nonnegative integers and randomly generates a solution and finds the most optimal neighboring solution until we've reached last iteration
int64_t hillClimbing(const vector<int64_t>& nums) {
    // Randomized solution creation
    int64_t n = nums.size();
    vector<int64_t> solution = random_bits(n);
    int64_t best_sol = abs(accumulate(solution.begin(), solution.end(), 0));

    uniform_int_distribution<int64_t> rand_int(1, n-1);

    for (int64_t iter = 0; iter < MAX_ITER; iter++) {
        int64_t i1 = rand_int(gen);
        int64_t i2 = i1;
        while (i2 == i1) {
            i2 = rand_int(gen);
        }

        int64_t si1 = solution[i1];
        int64_t si2 = solution[i2];

        int64_t new_sol = best_sol - si1 - si1;
        int64_t neg = bin_int(gen) < 0.5;

        if (neg) {
            new_sol -= si2 + si2;
        }

        new_sol = abs(new_sol);

        if (new_sol < best_sol) {
            solution[i1] = -si1;
            if (neg) {
                solution[i2] = -si2;
            }
        }
    }

    return best_sol;
}


bool anneal_prob(int64_t res_Sp, int64_t res_S, int64_t iter) {
    double T_iter = pow(10, 10) * pow(0.8, floor((iter + 1) / 300.0));
    double exponent = -(static_cast<double>(res_Sp) - res_S) / T_iter;

    return (bin_int(gen) / RAND_MAX) < exp(exponent);
}


int64_t simulatedAnnealing(const vector<int64_t>& nums) {
    // Randomized solution creation
    int64_t n = nums.size();
    vector<int64_t> solution = random_bits(n);
    uniform_int_distribution<int64_t> rand_int(1, n-1);

    int64_t curr_sol = abs(accumulate(solution.begin(), solution.end(), 0));
    int64_t best_sol = curr_sol;
    
    for (int64_t i = 0; i < MAX_ITER; i++) {
        int64_t i1 = rand_int(gen);
        int64_t i2 = i1;
        while (i2 == i1) {
            i2 = rand_int(gen);
        }
        
        int64_t si1 = solution[i1];
        int64_t si2 = solution[i2];

        int64_t new_sol = curr_sol - si1 - si1;
        int64_t neg = bin_int(gen) < 0.5;

        if (neg) {
            new_sol -= si2 + si2;
        }

        new_sol = abs(new_sol);

        if (new_sol < curr_sol || anneal_prob(new_sol, curr_sol, i)) {
            solution[i1] = -si1;
            if (neg) {
                solution[i2] = -si2;
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

// Prepartioned Repeat Random
int64_t prepartRepeatRand(const vector<int64_t>& nums) {
    double best = numeric_limits<double>::infinity();

    for (int64_t i = 0; i < MAX_ITER; ++i) {
        vector<int64_t> new_A = partition(nums);
        int64_t sol = karmarkarKarp(new_A);
        best = min(best, static_cast<double>(sol));
    }

    return static_cast<int64_t>(best);
}

// Prepartioned Hill Climbing
int64_t prepartHillClimbing(const vector<int64_t>& nums) {
    int64_t sz = nums.size();
    uniform_int_distribution<int64_t> rand_int(1, sz-1);
    
    vector<int64_t> P(sz);

    for (int64_t i = 0; i < sz; ++i) {
        P[i] = rand_int(gen);
    }

    vector<int64_t> numsPrime = partition(nums, P);
    int64_t best_sol = karmarkarKarp(numsPrime);

    for (int64_t idx = 0; idx < MAX_ITER; ++idx) {
        int64_t i, j;

        while (P[idx] == j) {
            i = rand_int(gen);
            j = rand_int(gen);
        }

        int64_t temp = P[idx];
        P[idx] = j;

        numsPrime = partition(nums, P);
        int64_t new_sol = karmarkarKarp(numsPrime);

        if (best_sol > new_sol) {
            best_sol = new_sol;
        } else {
            P[i] = temp;
        }
    }

    return best_sol;
}

// Prepartioned Simulated Annealing
int64_t prepartSimAnneal(const vector<int64_t>& nums) {
    int64_t sz = nums.size();
    uniform_int_distribution<int64_t> rand_int(1, sz-1);

    vector<int64_t> P(sz);
    for (int64_t i = 0; i < sz; ++i) {
        P[i] = rand_int(gen);
    }

    vector<int64_t> numsPrime = partition(nums, P);

    int64_t curr_sol = karmarkarKarp(numsPrime);
    int64_t best_sol = curr_sol;

    for (int64_t it = 0; it < MAX_ITER; ++it) {
        int64_t i, j;
        while (P[i] == j) {
            i = rand_int(gen);
            j = rand_int(gen);
        }

        int64_t temp = P[i];
        P[i] = j;

        numsPrime = partition(nums, P);
        int64_t new_sol = karmarkarKarp(numsPrime);

        if (new_sol < curr_sol || anneal_prob(new_sol, curr_sol, it)) {
            curr_sol = new_sol;
        } else {
            P[i] = temp;
        }

        best_sol = min(best_sol, curr_sol);
    }

    return best_sol;
}

// ----------------------------------------------------------------
// Main
// ----------------------------------------------------------------
int main(int argc, char *argv[]) {
    int do_tests = atoi(argv[1]);
    int algorithm = atoi(argv[2]);
    char* inputfile = argv[3];

    int64_t residue = 0;
    vector<int64_t> sequence = asciiToSequence(inputfile);

    // for (int i = 0; i < sequence.size(); i++) {
    //     cout << sequence[i] << endl;
    // }

    // if (do_tests == 1) {
    //     run_tests();
    // }

    if (algorithm == 0) {
        residue = karmarkarKarp(sequence);
    } else if (algorithm == 1) {
        residue = repeatedRandom(sequence);
    } else if (algorithm == 2) {
        residue = hillClimbing(sequence);
    } else if (algorithm == 3) {
        residue = simulatedAnnealing(sequence);
    } else if (algorithm == 11) {
        residue = prepartRepeatRand(sequence);
    } else if (algorithm == 12) {
        residue = prepartHillClimbing(sequence);
    } else if (algorithm ==  13) {
        residue = prepartSimAnneal(sequence);
    }

    cout << residue << endl;

    return 0;
}
