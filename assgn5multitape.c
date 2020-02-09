#include <stdio.h>
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void sort(int arr[100], int n)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n - 1; j++)
		{
			if (arr[j + 1] < arr[j])
			{
				swap(&arr[j + 1], &arr[j]);
			}
		}
}

void optimal_storage(int arr[100], int n, int tapes[100], int C)
{
	int i = 0, sum = 0, j = 0;
	sort(arr, n);
	int counter[C]={0}, temp[C]={0}, no_of_item[C]={0};
	for (i = 0; i < n; i++)
	{
		int iter = 0;
		while (1)
		{
			if (temp[j] + arr[i] <= tapes[j])
			{
				temp[j] += arr[i];
				counter[j] += temp[j];
				no_of_item[j] = no_of_item[j] + 1;
				j = (j + 1) % C;
				break;
			}
			iter++;
			if (iter == C)
			{
				printf("\n\nNo more space available for current data items ! \n\n");
				break;
			}
			j = (j + 1) % C;
		}
	}
	for (j = 0; j < C; j++)
	{
		float mrt = (float)counter[j] / no_of_item[j];
		printf("\nMean retreival time for tape  %d is %f\n", j + 1, mrt);
	}
}

int main()
{
	int i, C, tapes[100];
	printf("\nEnter the no. of tapes : \n");
	scanf("%d", &C);
	printf("\nEnter the capacity for each tape : \n");
	for (i = 0; i < C; i++)
	{
		scanf("%d", &tapes[i]);
	}
	printf("\n\nEnter the no. of disk items : ");
	int n;
	scanf("%d", &n);
	int arr[n];
	for (i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	optimal_storage(arr, n, tapes, C);
}
