#include "simulator.h"
#include <iostream>
#include <queue>
#include <random>
#include <algorithm>

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

const int LAMDA = 3;
const int L = 12000;
const int ALPHA = 8;
const int C = 1000000;
const int RHO = 0;
const int E_N = 0;
const int E_T = 0;
const int P_IDLE = 0;
const int P_LOSS = 0;

struct Packet {
	double arrivalTime;
	double departureTime;
	int length;
};

struct Observer {
	double observeTime;
};

double randomNum() {
	return ((double) rand() / (RAND_MAX));
}

void simulator(const int T, const int K) {
	srand(time(NULL));
	printf("Queue Simulator\n");
	printf("---------------\n");
	printf("Running with paramters:\n");
	printf("    T: %d\n\n", T);

	std::vector<Observer *> *observers = new std::vector<Observer *>();
	std::vector<Packet *> *packets = new std::vector<Packet *>();

	std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> observerExponentialDistribution(ALPHA);

    /**
     * Generate a set of random observation times
     * according to a poisson distribution with parameter
     * α (ALPHA).
     */
	for (double t = 0.0; t < ((double) T); t += observerExponentialDistribution(gen)) {
		Observer *newObserver = new Observer;

		newObserver->observeTime = t;

		observers->push_back(newObserver);
	}

	/**
     * Generate a set of random packet arrivals
     * according to a poisson distribution with parameter
     * λ (LAMDA). Packet length is generated according to
     * an exponential distribution with parameter 1/L.
     * Departure time depends on how much system needs to
     * wait and its service time [L/C] (C is the link rate).
     */
    std::exponential_distribution<> packetArrivalExponentialDistribution(LAMDA);
    std::exponential_distribution<> packetLengthExponentialDistrubution(1.0 / ((double) L));

	for (double t = 0.0; t < ((double) T); t += packetArrivalExponentialDistribution(gen)) {
		Packet *newPacket = new Packet;

		int length = std::round(packetLengthExponentialDistrubution(gen));
		double departureTime = 0.0;
		double serviceTime = (((double) length) / ((double) C));

		// If there are already packets in the queue service
		// time starts after the last packet has been serviced
		if (packets->size()) {
			departureTime = std::max(t, packets->back()->departureTime) + serviceTime;
		} else {
			departureTime = t + serviceTime;
		}

		newPacket->arrivalTime = t;
		newPacket->departureTime = departureTime;
		newPacket->length = length;

		packets->push_back(newPacket);
	}

	/**
	 * N_A = Number of packet arrivals so far
	 * N_D = Number of packet departures so far
	 * N_O = Number of observations so far
	 */

	int N_A = 0;
	int N_D = 0;
	int N_O = 0;
}
