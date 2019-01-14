/*	
	Gotoh's Algorithm
	Implemented by Ian Wallace
	---------------------------
	Returns cost of optimal conversion using 
	gotoh's algorithm (fig. 1(B) in Miller-Myters (1988))
*/

#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define G 2.0  		// Gap start penalty
#define H 0.5  		// Gap continue penalty
#define MATCH 0.0 	// Cost of a match
#define MISMATCH 1.0  // Cost of a mismatch

double cost_table(char a, char b, double costs[][128]);
double gotoh_algorithm(char *sA, char *sB);
double min2(double a, double b);
double min3(double a, double b, double c);
void print_arrays(double *CC, double *DD, int N);

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

	printf("Cost is %.2lf", 
			gotoh_algorithm(sA, sB));

	return 0;
}

void print_arrays(double *CC, double *DD, int N)
{
	int i;

	printf("Array CC\n");
	for(i=0; i<N; i++)
	{
		if(i == N-1)
			printf("%.2lf\n", CC[i]);
		else
			printf("%.2lf ", CC[i]);
	}

	printf("Array DD\n");
	
	for(i=0; i<N; i++)
	{
		if(i == N-1)
			printf("%.2lf\n", DD[i]);
		else
			printf("%.2lf ", DD[i]);
	}
}

double gotoh_algorithm(char *sA, char *sB)
{
	double *CC, *DD;
	double costs[128][128];
	double e, c, s, t;
	int M, N;
	int i, j;

	M = strlen(sA) + 1;
	N = strlen(sB) + 1;

	CC = malloc(sizeof(double) * N);
	DD = malloc(sizeof(double) * N);

	// Begin Algorithm
	CC[0] = 0;
	t = G;

	for(j=1; j<N; j++)
	{
    t = t + H;
		CC[j] = t;
		DD[j] = t + G;
	}

	t = G;

	for(i=1; i<M; i++)
	{
		s = CC[0];

   		t = t + H;
    	c = t;

		CC[0] = c;

		e = t + G;

		for(j=1; j<N; j++)
		{
			e = min2(e, c + G) + H;
			DD[j] = min2(DD[j], CC[j] + G) + H;
			c = min3(DD[j], e, s + cost_table(sA[i-1], sB[j-1], costs));
			s =  CC[j];
			CC[j] = c;
		}
	}

	print_arrays(CC, DD, N);

	double result = CC[N-1];

	// Free memory for arrays
	free(CC);
	free(DD);

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