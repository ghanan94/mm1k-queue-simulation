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

	int c;


	while ((c = getopt (argc, argv, "hA:C:K:L:T:Z:")) != -1) {
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
			case 'h':
			case '?':
				printf("LAB1 (ECE358) - Queue Simulation\n\n");
				printf("    Parameters:\n");
				printf("%8s%-4s %s\n", "", "A:", "(ALPHA) Average number of observer events per second");
				printf("%8s%-4s %s\n", "", "C:", "Transmission rate of the output link in bits per second");
				printf("%8s%-4s %s\n", "", "K:", "Queue size (0 is infinite)");
				printf("%8s%-4s %s\n", "", "L:", "Average length of packet in bits");
				printf("%8s%-4s %s\n", "", "T:", "Time to run simulation (s)");
				printf("%8s%-4s %s\n", "", "Z:", "(LAMDA) Average number of packets generated/arrived per second");
				exit(EXIT_SUCCESS);
		}
	}

	simulator(false, T, K, LAMDA, L, ALPHA, C);

	exit(EXIT_SUCCESS);
}