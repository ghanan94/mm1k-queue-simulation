#include <iostream>
#include "simulator.h"

using namespace std;

int main(int argc, char *argv[]) {
	int T = 10000;
	int K = 0;
	int LAMDA = 5;
	int L = 12000;
	int ALPHA = 5;
	int C = 1000000;
	bool showEachTimeStamp = false;

	int c;


	while ((c = getopt (argc, argv, "shA:C:K:L:T:Z:")) != -1) {
		switch (c) {
			case 'A':
				ALPHA = std::stoi(optarg);
				break;
			case 'C':
				C = std::stoi(optarg);
				break;
			case 'K':
				K = std::stoi(optarg);
				break;
			case 'L':
				L = std::stoi(optarg);
				break;
			case 'T':
				T = std::stoi(optarg);
				break;
			case 'Z':
				LAMDA = std::stoi(optarg);
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
				printf("%8s%-10s %s\n", "", "-T <arg>", "Time to run simulation (s)");
				printf("%8s%-10s %s\n", "", "-Z <arg>", "(LAMDA) Average number of packets generated/arrived per second");
				printf("%8s%-10s %s\n", "", "-s", "Show timestamps for every event");
				printf("%8s%-10s %s\n", "", "-h", "Help");
				printf("\n");
				exit(EXIT_SUCCESS);
		}
	}

	simulator(showEachTimeStamp, T, K, LAMDA, L, ALPHA, C);

	exit(EXIT_SUCCESS);
}