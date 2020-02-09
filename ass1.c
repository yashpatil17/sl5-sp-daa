#include <stdio.h>
#include <stdlib.h>

int A[] = {0, 0, 0, 0, 0, 0, 0, 0};
int count = 1;

void printQ(int n)
{
	int i, j;
	printf("\n\nSolution %d :\n\n", count); //funbction for printing positions and no. of solutions.
	count++;
	for (i = 1; i <= n; ++i)
		printf("\t%d", i);

	for (i = 1; i <= n; ++i)
	{
		printf("\n\n%d", i);
		for (j = 1; j <= n; ++j) //for nxn board
			if (A[i] == j)
				printf("\tQ"); //queen at i,j position
			else
				printf("\t-"); //empty slot
	}
}

int place(int k, int i)
{
	int j;
	for (j = 1; j <= k - 1; j++)
		if (A[j] == i) //returns false if in same column , condition for row is checked through array
			return 0;
		else if (abs(A[j] - i) == abs(j - k)) //returns false when in diagonal
			return 0;
	return 1;
}

void nQueens(int k, int n)
{
	int i;
	for (i = 1; i <= n; i++)
		if (place(k, i))
		{
			A[k] = i;
			if (k == n)
				printQ(n); // when all queens get their position
			else
				nQueens(k + 1, n); //recursion of fuction for next queen
		}
}

void main()
{
	int n;
	printf("Enter no. of queens \n");
	scanf("%d", &n);

	nQueens(1, n);

	printf("\n\n");
}
