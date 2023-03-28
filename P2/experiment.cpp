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

void printTrial(string type, int dim, double time, string unit) {
    cout << 
        type + " Multiplication performed multiplication on dimension: " 
        + to_string(dim) + " in " + to_string(time) + " " + unit 
        << endl;
}

pair<double, double> experimentTrial(int dim, int trials) {
    double time_c = 0.0;
    double time_s = 0.0;
    for (int i = 0; i < trials; ++i) {
        int *ma = new int[dim*dim];
        int *mb = new int[dim*dim];
        int *result = new int[dim*dim];

        // Getting random matrices
        getRandomMatrix(dim, ma, -1, 2);
        getRandomMatrix(dim, mb, -1, 2);

        // Timing conventional multiplication
        auto start = high_resolution_clock::now();
        conventional(dim, ma, mb, result);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<seconds>(stop - start);
        time_c += (double)duration.count();

        // Timing Strassen's algorithm
        start = high_resolution_clock::now();
        // strassen(dim, ma, mb, result);
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

void runExperiment() {
    // Open csv file
    ofstream myfile;
    myfile.open("experiment.csv");

    // Experimenting with granular values of n
    for (int dim = 0; dim < 100; dim++){
        pair<double, double> times = experimentTrial(dim, 4);
        myfile << "Conventional" << "," << dim << "," << times.first << "\n";
        printTrial("Conventional", dim, times.first, "seconds");
        myfile << "Strassen" << "," << dim << "," << times.second << "\n";
        printTrial("Strassen", dim, times.first, "seconds");
    }

    // Experimenting with larger values of n with a larger step
    for (int dim = 100; dim < 1100; dim += 200){
        pair<double, double> times = experimentTrial(dim, 4); 
        myfile << "Conventional" << "," << dim << "," << times.first << "\n";
        printTrial("Conventional", dim, times.first, "seconds");
        myfile << "Strassen" << "," << dim << "," << times.second << "\n";
        printTrial("Strassen", dim, times.first, "seconds");   
    }

    myfile.close();
}