#include "simulator.h"
#include <iostream>
#include <queue>
#include <random>

using namespace std;

/**
 * λ = Average number of packets generated /arrived per second
 * L = Average length of a packet in bits
 * α = Average number of observer events per second
 * C = The transmission rate of the output link in bits per second
 * ρ = Utilization of the queue (= L λ/C)
 * E[N] = Average number of packets in the buffer/queue
 * E[T] = Average sojourn time (queuing delay + service time)
 * PIDLE = The proportion of time the server is idle
 * PLOSS = The packet loss probability (for M/M/1/K queue)
 */

int LAMDA = 0; 		
int L = 12000; 		
int ALPHA = 5; 		
int C = 1000000; 	
int RHO = 0; 		
int E_N = 0; 		
int E_T = 0; 		 
int P_IDLE = 0; 	
int P_LOSS = 0; 	

double randomNum() {
	return ((double) rand() / (RAND_MAX));
}

void simulator(const int T, const int K) {
	srand(time(NULL));
	printf("Queue Simulator\n");
	printf("---------------\n");
	printf("Running with paramters:\n");
	printf("    T: %d\n", T);
	printf("\n");

	std::queue<double> *observers = new std::queue<double>();
	std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> exponentialDistribution(ALPHA);

    /**
     * Generate a set of random observation times
     * according to a distribution with parameter
     * α (ALPHA)
     */
	for (double t = 0.0; t < ((double) T); t += exponentialDistribution(gen)) {
		observers->push(i);
	}


}