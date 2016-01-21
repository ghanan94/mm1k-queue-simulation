#include <iostream>
#include "simulator.h"
using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("LAB1 (ECE358) - Queue Simulation\n\n");
		printf("    Syntax: %s <T> <K>\n", argv[0]);
		printf("%8s%-4s %s\n", "", "T:", "Time to run simulation (s)");
		printf("%8s%-4s %s\n", "", "K:", "Queue size (0 is infinite)");
		exit(0);
	}

	const int T = std::stoi(argv[1]);
	const int K = std::stoi(argv[2]);
	simulator(T, K);

	return 0;
}