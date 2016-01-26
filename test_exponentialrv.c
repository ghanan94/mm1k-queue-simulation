#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double exponentialRV(const double random, const double lamda) {
	return log(1.0 - random)/(-1.0 * lamda);
}

//gcc test_exponentialrv.c -o test_exponentialrv
int main(int argc, char *argv[]) {
	srand(time(NULL));

	printf("Test Exponential RV\n");
	printf("-------------------\n");

	if (argc < 3) {
		printf("MISSING PARAMS (NEED LAMDA AND NUBMERS_GENERATED)\n\n");
		exit(1);
	}

	const double LAMDA = atoi(argv[1]);
	const int NUMBERS_GENERATED = atoi(argv[2]);

	printf("LAMDA: %f\n", LAMDA);
	printf("NUMBERS_GENERATED: %d\n", NUMBERS_GENERATED);

	const double expectedMean = 1.0 / LAMDA;
	const double expectedVariance = 1.0 / pow(((double) LAMDA), 2);

	double *values = (double *)malloc(NUMBERS_GENERATED * sizeof(double));
	double sum = 0.0;

	for (int i = 0; i < NUMBERS_GENERATED; i++) {
		double rv = exponentialRV(((double) rand() / (RAND_MAX)), LAMDA);
		values[i] = rv;
		sum += rv;
	}

	double actualMean = sum / NUMBERS_GENERATED;
	double variance = 0.0;

	for (int i = 0; i < NUMBERS_GENERATED; i++) {
		variance +=  pow((values[i] - actualMean), 2);
	}

	variance *= (1.0 / (NUMBERS_GENERATED - 1));

	free(values);
	values = NULL;

	printf("Expected mean: %f\n", expectedMean);
	printf("Expected variance: %f\n", expectedVariance);
	printf("Actual mean: %f\n", actualMean);
	printf("Actual variance: %f\n\n", variance);

	exit(EXIT_SUCCESS);
}
