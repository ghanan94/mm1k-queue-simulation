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

void simulator(const bool showEachTimeStamp, const int T, const int K, const int LAMDA, const int L, const int ALPHA, const int C) {
	srand(time(NULL));
	printf("Queue Simulator\n");
	printf("---------------\n");
	printf("Running with paramters:\n");
	printf("  %-9s %d\n", "T:", T);
	printf("  %-9s %d (0 = infinite)\n", "K:", K);
	printf("  %-9s %d\n", "LAMDA:", LAMDA);
	printf("  %-9s %d\n", "L:", L);
	printf("  %-9s %d\n", "ALPHA:", ALPHA);
	printf("  %-9s %d\n\n", "C:", C);

	std::queue<Observer *> *observers = new std::queue<Observer *>();
	std::queue<Packet *> *packets = new std::queue<Packet *>();
	std::queue<Packet *> *departingPackets = new std::queue<Packet *>();

	std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> observerExponentialDistribution(ALPHA);

    /**
     * Generate a set of random observation times
     * according to a poisson distribution with parameter
     * α (ALPHA).
     */
	for (double t = observerExponentialDistribution(gen); t < ((double) T); t += observerExponentialDistribution(gen)) {
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
    std::exponential_distribution<> packetArrivalExponentialDistribution(LAMDA);
    std::exponential_distribution<> packetLengthExponentialDistrubution(1.0 / ((double) L));

	for (double t = packetArrivalExponentialDistribution(gen); t < ((double) T); t += packetArrivalExponentialDistribution(gen)) {
		Packet *newPacket = new Packet;

		int length = std::round(packetLengthExponentialDistrubution(gen));
		double serviceTime = (((double) length) / ((double) C));

		newPacket->arrivalTime = t;
		newPacket->serviceTime = serviceTime;

		packets->push(newPacket);
	}

	/**
	 * N_A 		= Number of packet arrivals so far
	 * N_D 		= Number of packet departures so far
	 * N_O 		= Number of observations so far
	 * queued 	= Number of packets currently in queue
	 */
	int N_A = 0;
	int N_D = 0;
	int N_O = 0;

	int queued = 0;
	int idleObservations = 0;
	int queuedObservations = 0;

	double simulatedTime = std::min(observers->front()->observeTime, packets->front()->arrivalTime);

	while (true) {
		// Observer Event
		if (observers->size() && (observers->front()->observeTime == simulatedTime)) {
			showEachTimeStamp && printf("Observer Event @ %f\n", observers->front()->observeTime);

			if (departingPackets->empty()) {
				++idleObservations;
			}

			queuedObservations += queued;

			++N_O;
			delete observers->front();
			observers->pop();
		}

		// Packet Arrival Event
		if (packets->size() && (packets->front()->arrivalTime == simulatedTime)) {
			showEachTimeStamp && printf("Arrival Event @ %f\n", packets->front()->arrivalTime);

			if ((K == 0) || (queued < K)) {
				++queued;

				/**
				 * Add a departure event.
				 * Departure time depends on whether any other packets are in the queue.
				 */
				if (departingPackets->empty()) {
					packets->front()->departureTime = packets->front()->arrivalTime + packets->front()->serviceTime;
				} else {
					packets->front()->departureTime = departingPackets->back()->departureTime + packets->front()->serviceTime;
				}

				departingPackets->push(packets->front());

				showEachTimeStamp && printf("-----Added Departure @ %f\n", packets->front()->departureTime);
			} else {
				// Free arrival packet from memory only if it is
				// being dropped (not needed in departure event)
				delete packets->front();

				showEachTimeStamp && printf("-----Dropped packet\n");
			}

			++N_A;
			packets->pop();
		}

		// Packet Departure Event
		if (departingPackets->size() && (departingPackets->front()->departureTime == simulatedTime)) {
			showEachTimeStamp && printf("Departure Event @ %f\n", departingPackets->front()->departureTime);

			--queued;
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

	const int dropped = N_A - N_D;
	const double E_N = ((double) queuedObservations) / ((double) N_O);
	const double E_T = LAMDA * E_N;
	const double P_IDLE = ((double) idleObservations) / ((double) N_O);
	const double P_LOSS = ((double) dropped) / ((double) N_A);

	printf("%-20s %d\n", "Arrived Packets:", N_A);
	printf("%-20s %d\n", "Departed Packets:", N_D);
	printf("%-20s %d\n", "Dropped Packets:", dropped);
	printf("%-20s %d\n", "Observers:", N_O);

	printf("%-20s %f\n", "E_N:", E_N);
	printf("%-20s %f\n", "E_T:", E_T);
	printf("%-20s %f\n", "P_IDLE:", P_IDLE);
	printf("%-20s %f\n", "P_LOSS:", P_LOSS);
}
