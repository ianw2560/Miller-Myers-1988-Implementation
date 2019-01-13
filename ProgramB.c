/*	
	Gotoh's Algorithm
	Implemented by Ian Wallace

	---------------------------
	Returns cost of optimal conversion using 
	gotoh's algorithm (fig. 1(B))
*/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define G 2.0  		// Gap start penalty
#define H 0.5  		// Gap continue penalty
#define MATCH 0.0 	// Cost of a match
#define MISMATCH 1.0  // Cost of a mismatch

double gotohAlgorithm_NSpace(char *seqA, char *seqB);
void printArrays(double *arrayCC, double *arrayDD, int N);
double min2(double a, double b);
double min3(double a, double b, double c);
double costTable(char a, char b, double costs[][128]);
double matchCost(char a, char b);

int main(int argc, char **argv)
{
	char *seqA, *seqB;

  if(argc != 3)
  {
    printf("error: %s sequence1 sequence2\n", argv[0]);
    return 1;
  }

	seqA = argv[1];
	seqB = argv[2];

	printf("Cost is %.2lf", 
			gotohAlgorithm_NSpace(seqA, seqB));

	return 0;
}

void printArrays(double *arrayCC, double *arrayDD, int N)
{
	int i;

	printf("Array CC\n");
	for(i=0; i<N; i++)
	{
		if(i == N-1)
			printf("%.2lf\n", arrayCC[i]);
		else
			printf("%.2lf ", arrayCC[i]);
	}
	//printf("\n");
	printf("Array DD\n");
	for(i=0; i<N; i++)
	{
		if(i == N-1)
			printf("%.2lf\n", arrayDD[i]);
		else
			printf("%.2lf ", arrayDD[i]);
	}
}

double gotohAlgorithm_NSpace(char *seqA, char *seqB)
{
	double *arrayCC, *arrayDD;
  double costs[128][128];
	double e, c, s, t;
	int M, N, i, j;

	M = strlen(seqA) + 1;
	N = strlen(seqB) + 1;

	arrayCC = malloc(sizeof(double) * N);
	arrayDD = malloc(sizeof(double) * N);

	// Begin Algorithm
	arrayCC[0] = 0;
	t = G;

	for(j=1; j<N; j++)
	{
    t = t + H;
		arrayCC[j] = t;
		arrayDD[j] = t + G;
	}

	t = G;

	for(i=1; i<M; i++)
	{
		s = arrayCC[0];

    t = t + H;
    c = t;
		arrayCC[0] = c;

		e = t + G;

		for(j=1; j<N; j++)
		{
			e = min2(e, c + G) + H;
			arrayDD[j] = min2(arrayDD[j], arrayCC[j] + G) + H;
			c = min3(arrayDD[j], e, s + costTable(seqA[i-1], seqB[j-1], costs));
			s =  arrayCC[j];
			arrayCC[j] = c;
		}
	}

	printArrays(arrayCC, arrayDD, N);

  double result = arrayCC[N-1];

	// Free memory for arrays
	free(arrayCC);
	free(arrayDD);

	return result;
}

double min2(double a, double b)
{
  return (a < b) ? a : b;
}

double min3(double a, double b, double c)
{
	return min2(min2(a, b), c);
}

double costTable(char a, char b, double costs[][128])
{
  int i, j;

	for(i=0; i<128; i++)
    for(j=0; j<128; j++)
      costs[i][j] = MISMATCH;

  for(i=0; i<128; i++)
    costs[i][i] = MATCH;


	return costs[a][b];
}