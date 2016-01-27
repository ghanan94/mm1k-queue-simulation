#include <iostream>
#include "simulator.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc < 5) {
		printf("lab1 <K> <RHO_MIN> <RHO_MAX> <RHO_STEP>\n");
		exit(1);
	}

	const int T = 10000;
	const int K = std::stoi(argv[1]);
	const int LAMDA = 5;
	const int L = 12000;
	const int ALPHA = 5;
	const int C = 1000000;
	const bool showEachTimeStamp = false;

	const double RHO_MIN = std::stod(argv[2]);
	const double RHO_MAX = std::stod(argv[3]);
	const double RHO_STEP = std::stod(argv[4]);


	for (double RHO = RHO_MIN; RHO <= RHO_MAX; RHO += RHO_STEP) {
		simulator(showEachTimeStamp, T, K, LAMDA, L, ALPHA, C, RHO);
	}

	exit(EXIT_SUCCESS);
}
