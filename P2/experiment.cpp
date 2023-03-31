#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "strassen.h"

using namespace std;

using namespace std::chrono;

void printTrial(string type, int dim, int crossover, double time, string unit) {
    cout << 
        type + " Multiplication performed multiplication on dimension: " 
        + to_string(dim) + " in " + to_string(time) + " " + unit + " with crossover: " +
        to_string(crossover)
        << endl;
}

pair<double, double> crossOverExperimentTrial(int dim, int trials, int crossover) {
    double time_c = 0.0;
    double time_s = 0.0;
    for (int i = 0; i < trials; ++i) {
        int *ma = new int[dim*dim];
        int *mb = new int[dim*dim];
        int *result = new int[dim*dim];

        // Getting random matrices
        getRandomMatrix(dim, ma, -1, 2, 0);
        getRandomMatrix(dim, mb, -1, 2, 0);

        // Timing conventional multiplication
        auto start = high_resolution_clock::now();
        conventional(dim, ma, mb, result);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        time_c += (double)duration.count();

        // Timing Strassen's algorithm
        start = high_resolution_clock::now();
        strassen(dim, ma, mb, result, crossover);
        stop = high_resolution_clock::now();

        duration = duration_cast<microseconds>(stop - start);
        time_s += (double)duration.count();

        delete[] ma;
        delete[] mb;
        delete[] result;
    }
    // Averaging out times
    time_c /= (1000 * trials);
    time_s /= (1000 * trials);

    return make_pair(time_c, time_s);
}

void runCrossOverExperiment() {
    // EVEN DIMENSION EXPERIMENT

    // Open csv file
    ofstream myfile;
    myfile.open("even_crossover_experiment_before_opt.csv");

    // File header
    myfile << "Crossover,Conventional,Strassen,Dimension" << "\n";

    // Experimenting with larger values of n with a larger step
    for (int dim = 300; dim < 801; dim += 200){
        for (int n0 = 10; n0 < 55; n0 += 3){
            pair<double, double> times = crossOverExperimentTrial(dim, 4, n0);
            // Outputs following to csv file "crossover,conventional,strassen,dimension"
            printTrial("Conventional", dim, n0, times.first, "milliseconds");
            printTrial("Strassen", dim, n0, times.second, "milliseconds");

            myfile << n0 << "," << times.first << "," << times.second << "," << dim << "\n";
        }
    }

    myfile.close();


    // ODD DIMENSION EXPERIMENT

    // File header
    myfile << "Crossover,Conventional,Strassen,Dimension" << "\n";

    // Experimenting with larger values of n with a larger step
    for (int dim = 301; dim < 900; dim += 200){
        for (int n0 = 10; n0 < 55; n0 += 3){
            pair<double, double> times = crossOverExperimentTrial(dim, 4, n0);
            // Outputs following to csv file "crossover,conventional,strassen,dimension"
            printTrial("Conventional", dim, n0, times.first, "milliseconds");
            printTrial("Strassen", dim, n0, times.second, "milliseconds");

            myfile << n0 << "," << times.first << "," << times.second << "," << dim << "\n";
        }
    }

    myfile.close();
}

int nChoosek(int n, int k) {
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}

// Counts the number of triangular paths in a graph A^3
int countTriangles(int dim, int *matrix) {
    int count = 0;
    for (int i = 0; i < dim; i++) {
        count += getMatrixValue(matrix, i, i, dim);
        cout << getMatrixValue(matrix, i, i, dim) << endl;
    }

    return (count / 6);
}

void runTriangleExperiment() {
    // Open csv file
    ofstream myfile;
    myfile.open("triangle_experiment.csv");

    // file header
    myfile << "probability,number_triangles,expected_number" << "\n";

    int dim = 1024;
    int opt_crossover = 12;

    // Running triangle experiment for different values of probability
    for (int prob = 1; prob < 6; prob++) {
        int triangle_count = 0;
        int trials = 7;
        cout << "PROBABILITY: " << prob << "\n";
        for (int i = 0; i < trials; i++) {       
            int *matrix = new int[dim*dim];
            int *result = new int[dim*dim];
            getRandomMatrix(1024, matrix, 0, 1, prob);

            strassen(dim, matrix, matrix, result, opt_crossover); //A^2
            strassen(dim, result, matrix, result, opt_crossover); //A^3

            triangle_count += countTriangles(1024, result);

            delete[] matrix;
            delete[] result;
        }
        triangle_count /= trials;
        int coefficient = nChoosek(dim,3);

        // output to file "probability, number of triangles, expected number of triangles"
        myfile << prob << "," << triangle_count << "," << (coefficient * 0.000001 * prob * prob * prob) << "\n";
    }

    myfile.close();
}
