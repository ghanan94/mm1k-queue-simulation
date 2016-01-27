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
	double RHO = 0.0;
	bool showEachTimeStamp = false;

	simulator(showEachTimeStamp, T, K, LAMDA, L, ALPHA, C, RHO);

	exit(EXIT_SUCCESS);
}
