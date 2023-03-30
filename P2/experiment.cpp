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
        getRandomMatrix(dim, ma, -1, 2, 1);
        getRandomMatrix(dim, mb, -1, 2, 1);

        // Timing conventional multiplication
        auto start = high_resolution_clock::now();
        conventional(dim, ma, mb, result);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);
        time_c += (double)duration.count();

        // Timing Strassen's algorithm
        start = high_resolution_clock::now();
        strassen(dim, ma, mb, result, crossover);
        stop = high_resolution_clock::now();

        duration = duration_cast<seconds>(stop - start);
        time_s += (double)duration.count();

        delete[] ma;
        delete[] mb;
        delete[] result;
    }
    // Averaging out times
    time_c /= trials;
    time_s /= trials;

    return make_pair(time_c, time_s);
}

void runCrossOverExperiment() {
    // Open csv file
    ofstream myfile;
    myfile.open("crossover_experiment.csv");

    // Experimenting with granular values of n
    for (int dim = 1; dim < 100; dim++){
        for (int n0 = 1; n0 < 50; n0++){
            pair<double, double> times = crossOverExperimentTrial(dim, 4, n0);
            // Outputs following to csv file "multiplication_type, dimension, crossover, time"
            myfile << "Conventional" << "," << dim << "," << n0 << "," << times.first << "\n";
            printTrial("Conventional", dim, n0, times.first, "seconds");

            myfile << "Strassen" << "," << dim << "," << n0 << "," << times.first << "\n";
            printTrial("Strassen", dim, n0, times.first, "seconds");
        }
    }

    // Experimenting with larger values of n with a larger step
    for (int dim = 100; dim < 1100; dim += 200){
        for (int n0 = 1; n0 < 50; n0++){
            pair<double, double> times = crossOverExperimentTrial(dim, 4, n0);
            // Outputs following to csv file "multiplication_type, dimension, crossover, time"
            myfile << "Conventional" << "," << dim << "," << n0 << "," << times.first << "\n";
            printTrial("Conventional", dim, n0, times.first, "seconds");

            myfile << "Strassen" << "," << dim << "," << n0 << "," << times.first << "\n";
            printTrial("Strassen", dim, n0, times.first, "seconds");
        }
    }

    myfile.close();
}

// Counts the number of triangular paths in a graph A^3
int countTriangles(int dim, int *matrix) {
    int count = 0;
    for (int i = 0; i < dim; i++) {
        count += getMatrixValue(matrix, i, i, dim);
    }

    return (count / 6);
}

void runTriangleExperiment() {
    // Open csv file
    ofstream myfile;
    myfile.open("triangle_experiment.csv");

    // Running triangle experiment for different values of probability
    for (int prob = 1; prob < 6; prob++) {
        int *matrix = new int[1024*1024];
        int *result = new int[1024*1024];
        getRandomMatrix(1024, matrix, 0, 1, prob);

        strassen(dim, matrix, matrix, result); //A^2
        strassen(dim, result, matrix, result); //A^3

        int triangle_count = countTriangles(1024, result);

        // output to file "probability, number of triangles"
        myfile << prob << "," << triangle_count <<"\n";

        delete[] matrix;
        delete[] result;
    }

    myfile.close();
}
