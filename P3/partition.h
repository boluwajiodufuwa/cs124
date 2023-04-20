#ifndef PARTITION_H_
#define PARTITION_H_

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

int64_t karmarkarKarp(vector<int64_t>& nums);
int64_t repeatedRandom(vector<int64_t>& nums);
int64_t hillClimbing(const vector<int64_t>& nums);
int64_t simulatedAnnealing(const vector<int64_t>& nums);
int64_t prepartRepeatRand(const vector<int64_t>& nums);
int64_t prepartHillClimbing(const vector<int64_t>& nums);
int64_t prepartSimAnneal(const vector<int64_t>& nums);
vector<int64_t> getRandomSequence();
int64_t residue(vector<int64_t> solution, vector<int64_t> seq);

#endif