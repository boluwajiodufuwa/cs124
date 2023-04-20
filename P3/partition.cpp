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
std::vector<int64_t> random_bits(int64_t n) {
    std::vector<int64_t> bits(n);
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
int karmarkarKarp(vector<int64_t>& nums) {
    // Initialize priority queue for inputted list of numbers (a)   
    std::priority_queue<int64_t> maxHeap(nums.begin(), nums.end());

    int64_t max1 = 0, max2 = 0;

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
    }

    return max1;
}

// Repeated Random
// Takes list of nonnegative integers and randomly generates/replaces solutions until we've reached last iteration
int repeatedRandom(vector<int64_t>& nums, int max_iter = 25000) {
    // Create a random solution 
    int solution = accumulate(nums.begin(), nums.end(), 0);
    srand(std::time(0));

    for (int i = 0; i < max_iter; i++) {
        int residueSp = 0;
        for (int elt : nums) {
            residueSp += elt * (rand() % 2 == 0 ? 1 : -1);
        }
        residueSp = abs(residueSp);
        solution = min(solution, residueSp);
    }

    return solution;
}


// Hill Climbing
// According to ed, max_iter should be 25k
// Takes list of nonnegative integers and randomly generates a solution and finds the most optimal neighboring solution until we've reached last iteration
int hillClimbing(const vector<int64_t>& nums, int max_iter = 25000) {
    // Randomized solution creation
    int n = nums.size();
    vector<int64_t> solution = random_bits(n);
    int best_sol = abs(accumulate(solution.begin(), solution.end(), 0));

    for (int iter = 0; iter < max_iter; iter++) {
        int i1 = std::rand() % n;
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

// Prepartioned Repeat Random
int prepartRepeatRand(const std::vector<int64_t>& nums, int max_iter = 25000) {
    double best = std::numeric_limits<double>::infinity();

    srand(std::time(0));

    for (int i = 0; i < max_iter; ++i) {
        vector<int64_t> new_A = partition(nums);
        int sol = karmarkarKarp(new_A);
        best = min(best, static_cast<double>(sol));
    }

    return static_cast<int>(best);
}

// Prepartioned Hill Climbing
int prepartHillClimbing(const vector<int64_t>& nums, int max_iter = 25000) {
    int sz = nums.size();
    srand(std::time(0));
    
    std::vector<int64_t> P(sz);

    for (int i = 0; i < sz; ++i) {
        P[i] = std::rand() % sz;
    }

    std::vector<int64_t> numsPrime = partition(nums, P);
    int best_sol = karmarkarKarp(numsPrime);

    for (int idx = 0; idx < max_iter; ++idx) {
        int i, j;

        while (P[idx] == j) {
            i = std::rand() % sz;
            j = std::rand() % sz;
        }

        int temp = P[idx];
        P[idx] = j;

        numsPrime = partition(nums, P);
        int new_sol = karmarkarKarp(numsPrime);

        if (best_sol > new_sol) {
            best_sol = new_sol;
        } else {
            P[i] = temp;
        }
    }

    return best_sol;
}

// Prepartioned Simulated Annealing
int prepartSimAnneal(const std::vector<int64_t>& nums, int max_iter = 25000) {
    int sz = nums.size();

    std::srand(std::time(0));

    std::vector<int64_t> P(sz);
    for (int i = 0; i < sz; ++i) {
        P[i] = std::rand() % sz;
    }

    std::vector<int64_t> numsPrime = partition(nums, P);

    int curr_sol = karmarkarKarp(numsPrime);
    int best_sol = curr_sol;

    for (int it = 0; it < max_iter; ++it) {
        int i, j;
        while (P[i] == j) {
            i = rand() % sz;
            j = rand() % sz;
        }

        int temp = P[i];
        P[i] = j;

        numsPrime = partition(nums, P);
        int new_sol = karmarkarKarp(numsPrime);

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

    return 0;
}
