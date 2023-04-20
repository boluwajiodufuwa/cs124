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
#include "partition.h"

using namespace std;

void run_experiment() {
    // Open csv file
    ofstream myfile;
    myfile.open("even_crossover_experiment_before_opt.csv");

    // File header
    myfile << "Crossover,Conventional,Strassen,Dimension" << "\n";

    for (int i = 0; i < 50; i++) {
        vector<int64_t> sequence = getRandomSequence();


    }

    myfile.close();
}