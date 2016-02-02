#include <iostream>
#include <unistd.h>
#include "simulator.h"

using namespace std;

int main(int argc, char *argv[]) {
	unsigned int T = 10000;
	unsigned int K = 0;
	unsigned int LAMDA = 5;
	unsigned int L = 12000;
	unsigned int ALPHA = 5;
	unsigned int C = 1000000;
	double RHO = 0.0;
	bool showEachTimeStamp = false;

	int c;


	while ((c = getopt (argc, argv, "shA:C:K:L:R:T:Z:")) != -1) {
		switch (c) {
			case 'A':
				ALPHA = (unsigned int)std::stoi(optarg);
				break;
			case 'C':
				C = (unsigned int)std::stoi(optarg);
				break;
			case 'K':
				K = (unsigned int)std::stoi(optarg);
				break;
			case 'L':
				L = (unsigned int)std::stoi(optarg);
				break;
			case 'R':
				RHO = std::stod(optarg);;
				break;
			case 'T':
				T = (unsigned int)std::stoi(optarg);
				break;
			case 'Z':
				LAMDA = (unsigned int)std::stoi(optarg);
				break;
			case 's':
				showEachTimeStamp = true;
				break;
			case 'h':
			case '?':
				printf("LAB1 (ECE358) - Queue Simulation\n\n");
				printf("    Parameters:\n");
				printf("%8s%-10s %s\n", "", "-A <arg>", "(ALPHA) Average number of observer events per second");
				printf("%8s%-10s %s\n", "", "-C <arg>", "Transmission rate of the output link in bits per second");
				printf("%8s%-10s %s\n", "", "-K <arg>", "Queue size (0 is infinite)");
				printf("%8s%-10s %s\n", "", "-L <arg>", "Average length of packet in bits");
				printf("%8s%-10s %s\n", "", "-R <arg>", "Utilization of the queue (= L*Z/C) (This will override Z value)");
				printf("%8s%-10s %s\n", "", "-T <arg>", "Time to run simulation (s)");
				printf("%8s%-10s %s\n", "", "-Z <arg>", "(LAMDA) Average number of packets generated/arrived per second");
				printf("%8s%-10s %s\n", "", "-s", "Show timestamps for every event");
				printf("%8s%-10s %s\n", "", "-h", "Help");
				printf("\n");
				exit(EXIT_SUCCESS);
		}
	}

	simulator(showEachTimeStamp, T, K, LAMDA, L, ALPHA, C, RHO);

	exit(EXIT_SUCCESS);
}
