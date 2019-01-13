/*	
	Gotoh's Algorithm
	Implemented by Ian Wallace
	---------------------------
	Returns cost of optimal conversion using 
	gotoh's algorithm (fig. 1(A) in Miller-Myters (1988))
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

double cost_table(char a, char b, double costs[][128]);
double gotoh_algorithm(char *sA, char *sB);
double **init_2d_array(double **array, int M, int N);
double min2(double a, double b);
double min3(double a, double b, double c);
void print_2d_array(double **array, int M, int N);

int main(int argc, char **argv)
{
	char *sA, *sB;

	if(argc != 3)
	{
		printf("error: %s sequence1 sequence2\n", argv[0]);
		return 1;
	}

	sA = argv[1];
	sB = argv[2];

	printf("Cost is %.2lf", gotoh_algorithm(sA, sB));

	return 0;
}

void print_2d_arrays(double **C, double **D, double **I, int M, int N)
{
	printf("Array C\n");
	print_2d_array(C, M, N); printf("\n");
	printf("Array D\n");
	print_2d_array(D, M, N); printf("\n");
	printf("Array I\n");
	print_2d_array(I, M, N);
}


double gotoh_algorithm(char *sA, char *sB)
{
	double **C, **D, **I;
	double costs[128][128];
	double t;
	int M, N, i, j;


	M = strlen(sA) + 1;
	N = strlen(sB) + 1;

	C = init_2d_array(C, M, N);
	D = init_2d_array(D, M, N);
	I = init_2d_array(I, M, N);

	// Begin Algorithm

	C[0][0] = 0;

	for(j=1; j < N; j++)
	{
		t = t + H;
		C[0][j] = t;
		D[0][j] = t + G;
	}

	t = G;

	for(i=1; i < M; i++)
	{
		t = t + H;
		C[i][0] = t;
		I[i][0] = t + G;

		for(j=1; j < N; j++)
		{
			I[i][j] = min2(I[i][j-1], C[i][j-1] + G) + H;
			D[i][j] = min2(D[i-1][j], C[i-1][j] + G) + H;

			C[i][j] = min3(D[i][j], I[i][j], 
								C[i-1][j-1] + cost_table(sA[i-1], sB[j-1], costs));
		}
	}

	print_2d_arrays(C, D, I, M, N);
	
	double cost = C[M-1][N-1];

	// Free memory for arrays
	for(i=0; i<M; i++)
	{
		free(C[i]);
		free(D[i]);
		free(I[i]);
	}

	free(C);
	free(D);
	free(I);
	
	return cost;
}

double **init_2d_array(double **array, int M, int N)
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

void print_2d_array(double **array, int M, int N) 
{
	int i, j;

	for(i=0; i<M; i++)
	{
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
}

double min2(double a, double b)
{
	return (a < b) ? a : b;
}

double min3(double a, double b, double c)
{
	return min2(min2(a, b), c);
}

double cost_table(char a, char b, double costs[][128])
{
	int i, j;

	for(i=0; i<128; i++)
    	for(j=0; j<128; j++)
      		costs[i][j] = MISMATCH;

  	for(i=0; i<128; i++)
    	costs[i][i] = MATCH;

	return costs[a][b];
}


