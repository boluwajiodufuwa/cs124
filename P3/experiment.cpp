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
    myfile.open("partition_experiment.csv");

    // File header
    myfile << "Karmarkar-Karp,Repeated Random,Hill Climbing,Simulated Annealing, Repeated Random Prepartitioned, Hill Climbing Prepartitioned, Simulated Annealing Prepartitioned" << "\n";

    for (int i = 0; i < 50; i++) {
        vector<int64_t> sequence = getRandomSequence();
        int64_t kk_residue = karmarkarKarp(sequence);
        int64_t rr_residue = repeatedRandom(sequence);
        int64_t hc_residue = hillClimbing(sequence);
        int64_t sa_residue = simulatedAnnealing(sequence);
        int64_t prr_residue = prepartRepeatRand(sequence);
        int64_t phc_residue = prepartHillClimbing(sequence);
        int64_t psa_residue = prepartSimAnneal(sequence);

        myfile << kk_residue << "," << rr_residue << "," << hc_residue << "," << sa_residue << prr_residue << "," << phc_residue << "," << psa_residue << "\n";
    }

    myfile.close();
}