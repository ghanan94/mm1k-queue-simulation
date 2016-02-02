#include "simulator.h"
#include <iostream>
#include <queue>
#include <random>
#include <algorithm>
#include <limits>

using namespace std;

/**
 * T  				= Time to run simulation
 * K				= Size of queue (0 = infinite)
 * λ (LAMDA) 		= Average number of packets generated /arrived per second
 * L 				= Average length of a packet in bits
 * α (ALPHA) 		= Average number of observer events per second
 * C 				= The transmission rate of the output link in bits per second
 * ρ (RHO) 			= Utilization of the queue (= L λ/C)
 * E[N] (E_N) 		= Average number of packets in the buffer/queue
 * E[T] (E_T) 		= Average sojourn time (queuing delay + service time)
 * PIDLE (P_IDLE) 	= The proportion of time the server is idle
 * PLOSS (P_LOSS) 	= The packet loss probability (for M/M/1/K queue)
 */

const double DOUBLE_MAX = std::numeric_limits<double>::max();

struct Packet {
	double arrivalTime;
	double departureTime;
	double serviceTime;
};

struct Observer {
	double observeTime;
};

double exponentialRV(const double random, const double lamda) {
	return std::log(1.0 - random)/(-1.0 * lamda);
}

void simulator(const bool showEachTimeStamp, const unsigned int T, const unsigned int K, unsigned int LAMDA, const unsigned int L, unsigned int ALPHA, const unsigned int C, double RHO) {
	if (RHO != 0.0) {
		LAMDA = (unsigned int)std::round(RHO * C / L);
		ALPHA = 3 * LAMDA;
	} else {
		RHO = (double) LAMDA * L / C;
	}

	srand(time(NULL));
	printf("Queue Simulator\n");
	printf("---------------\n");
	printf("Running with paramters:\n");
	printf("  %-9s %d\n", "T:", T);
	printf("  %-9s %d (0 = infinite)\n", "K:", K);
	printf("  %-9s %d\n", "LAMDA:", LAMDA);
	printf("  %-9s %d\n", "L:", L);
	printf("  %-9s %d\n", "ALPHA:", ALPHA);
	printf("  %-9s %d\n", "C:", C);
	printf("  %-9s %f\n\n", "RHO:", RHO);

	std::queue<Observer *> *observers = new std::queue<Observer *>();
	std::queue<Packet *> *packets = new std::queue<Packet *>();
	std::queue<Packet *> *departingPackets = new std::queue<Packet *>();

    /**
     * Generate a set of random observation times
     * according to a poisson distribution with parameter
     * α (ALPHA).
     */
	for (double t = exponentialRV(((double) rand() / (RAND_MAX)), ALPHA); t < ((double) T); t += exponentialRV(((double) rand() / (RAND_MAX)), ALPHA)) {
		Observer *newObserver = new Observer;

		newObserver->observeTime = t;

		observers->push(newObserver);
	}

	/**
     * Generate a set of random packet arrivals
     * according to a poisson distribution with parameter
     * λ (LAMDA). Packet length is generated according to
     * an exponential distribution with parameter 1/L.
     * Departure time depends on how much system needs to
     * wait and its service time of [L/C] (C is the link rate).
     */
    double packetLengthLamda = 1.0 / ((double) L);

	for (double t = exponentialRV(((double) rand() / (RAND_MAX)), LAMDA); t < ((double) T); t += exponentialRV(((double) rand() / (RAND_MAX)), LAMDA)) {
		Packet *newPacket = new Packet;

		int length = std::round(exponentialRV(((double) rand() / (RAND_MAX)), packetLengthLamda));
		double serviceTime = (((double) length) / ((double) C));

		newPacket->arrivalTime = t;
		newPacket->serviceTime = serviceTime;

		packets->push(newPacket);
	}

	/**
	 * N_A 		= Number of packet arrivals so far
	 * N_D 		= Number of packet departures so far
	 * N_O 		= Number of observations so far
	 * dropped 	= Number of packets dropped
	 */
	unsigned int N_A = 0;
	unsigned int N_D = 0;
	unsigned int N_O = 0;

	unsigned int dropped = 0;
	unsigned int idleObservations = 0;
	unsigned int queuedObservations = 0;
	unsigned int queueFullObservations = 0;

	double simulatedTime = std::min(observers->front()->observeTime, packets->front()->arrivalTime);

	while (true) {
		// Observer Event
		if (observers->size() && (observers->front()->observeTime == simulatedTime)) {
			showEachTimeStamp && printf("Observer Event @ %f\n", observers->front()->observeTime);

			if (departingPackets->empty()) {
				++idleObservations;
			}

			if ((K != 0) && ((N_A - N_D) == K)) {
				queueFullObservations += 1;
			}

			queuedObservations += N_A - N_D;

			++N_O;
			delete observers->front();
			observers->pop();
		}

		// Packet Arrival Event
		if (packets->size() && (packets->front()->arrivalTime == simulatedTime)) {
			showEachTimeStamp && printf("Arrival Event @ %f\n", packets->front()->arrivalTime);

			if ((K == 0) || ((N_A - N_D) < K)) {
				/**
				 * Add a departure event.
				 * Departure time depends on whether any other packets are in the queue.
				 */
				if (departingPackets->empty()) {
					packets->front()->departureTime = packets->front()->arrivalTime + packets->front()->serviceTime;
				} else {
					packets->front()->departureTime = departingPackets->back()->departureTime + packets->front()->serviceTime;
				}

				++N_A;
				departingPackets->push(packets->front());

				showEachTimeStamp && printf("-----Added Departure @ %f\n", packets->front()->departureTime);
			} else {
				++dropped;

				// Free arrival packet from memory only if it is
				// being dropped (not needed in departure event)
				delete packets->front();

				showEachTimeStamp && printf("-----Dropped packet\n");
			}

			packets->pop();
		}

		// Packet Departure Event
		if (departingPackets->size() && (departingPackets->front()->departureTime == simulatedTime)) {
			showEachTimeStamp && printf("Departure Event @ %f\n", departingPackets->front()->departureTime);

			++N_D;
			delete departingPackets->front();
			departingPackets->pop();
		}

		double nextObserverTime = DOUBLE_MAX;
		double nextArrivalTime = DOUBLE_MAX;
		double nextDepartureTime = DOUBLE_MAX;

		if (observers->size()) {
			nextObserverTime = observers->front()->observeTime;
			showEachTimeStamp && printf("Next Observer Time: %f\n", nextObserverTime);
		}

		if (packets->size()) {
			nextArrivalTime = packets->front()->arrivalTime;
			showEachTimeStamp && printf("Next Arrival Time: %f\n", nextArrivalTime);
		}

		if (departingPackets->size()) {
			nextDepartureTime = departingPackets->front()->departureTime;
			showEachTimeStamp && printf("Next Depart Time: %f\n", nextDepartureTime);
		}

		simulatedTime = std::min(nextDepartureTime, std::min(nextArrivalTime, nextObserverTime));

		if (simulatedTime == DOUBLE_MAX) {
			// No more events
			break;
		} else {
			showEachTimeStamp && printf("Next Simulated Event @ %f\n\n", simulatedTime);
		}
	}

	delete departingPackets;
	delete packets;
	delete observers;

	departingPackets = NULL;
	packets = NULL;
	observers = NULL;

	const double E_N = ((double) queuedObservations) / ((double) N_O);
	const double E_T = ((double) E_N) / LAMDA;
	const double P_IDLE = ((double) idleObservations) / ((double) N_O);
	const double P_LOSS = ((double) queueFullObservations) / ((double) N_O);

	printf("%-20s %d\n", "Arrived Packets:", N_A);
	printf("%-20s %d\n", "Departed Packets:", N_D);
	printf("%-20s %d\n", "Dropped Packets:", dropped);
	printf("%-20s %d\n", "Observers:", N_O);

	printf("%-20s %f\n", "E_N:", E_N);
	printf("%-20s %f\n", "E_T:", E_T);
	printf("%-20s %f\n", "P_IDLE:", P_IDLE);
	printf("%-20s %f\n", "P_LOSS:", P_LOSS);
}
