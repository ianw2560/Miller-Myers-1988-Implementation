/*	
	Gotoh's Algorithm
	Implemented by Ian Wallace

	---------------------------
	Returns cost of optimal conversion using 
	gotoh's algorithm (fig. 1(A))
*/

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define G 2   		// Gap start penalty
#define H 0.5  		// Gap continue penalty
#define MATCH 0 	// Cost of a match
#define MISMATCH 1  // Cost of a mismatch
#define UNDEFINED -(DBL_MAX) 

double gotohAlgorithm(char *seqA, char *seqB);
double **init2DArray(double **array, int m, int n);
void printArray(double **array, int m, int n);
double min2(double a, double b);
double min3(double a, double b, double c);
double costTable(char a, char b, double costs[][128]);

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

	printf("Cost is %.2lf", gotohAlgorithm(seqA, seqB));

	return 0;
}

void printArrays(double **arrayC, double **arrayD, double **arrayI,
				 int M, int N)
{
	printf("Array C\n");
	printArray(arrayC, M, N); printf("\n");
	printf("Array D\n");
	printArray(arrayD, M, N); printf("\n");
	printf("Array I\n");
	printArray(arrayI, M, N);
}


double gotohAlgorithm(char *seqA, char *seqB)
{
	double **arrayC, **arrayD, **arrayI;
	double costs[128][128];
	double t;
	int M, N, i, j;


	M = strlen(seqA) + 1;
	N = strlen(seqB) + 1;

	arrayC = init2DArray(arrayC, M, N);
	arrayD = init2DArray(arrayD, M, N);
	arrayI = init2DArray(arrayI, M, N);

	// Begin Algorithm

	arrayC[0][0] = 0;
	//arrayC[0][1] = G;
	//arrayC[1][0] = G;
	/*
	for(i=1; i < M; i++)
		arrayC[i][0] = arrayC[i-1][0] + H;
	for(j=1; j < N; j++)
		arrayC[0][j] = arrayC[0][j-1] + H;
	*/
	t = G;

	for(j=1; j < N; j++)
	{
		t = t + H;
		arrayC[0][j] = t;
		arrayD[0][j] = t + G;
	}

	t = G;

	for(i=1; i < M; i++)
	{
		t = t + H;
		arrayC[i][0] = t;
		arrayI[i][0] = t + G;

		for(j=1; j < N; j++)
		{
			arrayI[i][j] = min2(arrayI[i][j-1], arrayC[i][j-1] + G) + H;
			arrayD[i][j] = min2(arrayD[i-1][j], arrayC[i-1][j] + G) + H;

			arrayC[i][j] = min3(arrayD[i][j], arrayI[i][j], 
								arrayC[i-1][j-1] + costTable(seqA[i-1], seqB[j-1], costs));
		}
	}

	printArrays(arrayC, arrayD, arrayI, M, N);
	
	double cost = arrayC[M-1][N-1];

	// Free memory for arrays
	for(i=0; i<M; i++)
	{
		free(arrayC[i]);
		free(arrayD[i]);
		free(arrayI[i]);
	}

	free(arrayC);
	free(arrayD);
	free(arrayI);
	
	return cost;
}

double **init2DArray(double **array, int M, int N)
{
	int i, j;
	array = malloc(sizeof(double *) * M);

	for(i=0; i<M; i++)
		array[i] = malloc(sizeof(double) * N);

	for(int i=0; i<M; i++)
		for(int j=0; j<N; j++)
			array[i][j] = UNDEFINED;

	return array;
}

void printArray(double **array, int M, int N) 
{
	int i, j;
	for(i=0; i<M; i++)
		for(j=0; j<N; j++)
		{
			if(j == N-1)
			{
				if(array[i][j] == UNDEFINED)
					printf("undef\n");
				else
					printf("%.2lf\n", array[i][j]);
			}
			else
			{
				if(array[i][j] == UNDEFINED)
					printf("undef\t");
				else
					printf("%.2lf\t", array[i][j]);
			}
		}
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


