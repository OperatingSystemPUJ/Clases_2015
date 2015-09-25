#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_M_SIZE 3

typedef struct _myArg
{
	int mat[MAX_M_SIZE][MAX_M_SIZE];
	int scalar;
} myArg;

void *thread(void *vargp)
{
	int i, j;
	myArg* data_out = (myArg*) vargp;
	for (i = 0; i < MAX_M_SIZE; ++i)
	{
		for (j = 0; j < MAX_M_SIZE; ++j)
		{
			data_out->mat[i][j] = data_out->mat[i][j] * data_out->scalar;
		}
	}
	pthread_exit((void*)data_out);
}

int main(int argc, char *argv[])
{
	myArg data_in;
	myArg* data_out;
	int i, j;
	pthread_t tid;

	/* User input */
	printf("Enter the numbers for the matrix\n");
	for (i = 0; i < MAX_M_SIZE; ++i)
	{
		for (j = 0; j < MAX_M_SIZE; ++j)
		{
			scanf("%d", &data_in.mat[i][j]);
		}
	}
	printf("Enter the number for the scalar: ");
	scanf("%d", &data_in.scalar);

	pthread_create(&tid, NULL, thread, (void *) &data_in);
	pthread_join(tid, (void **)&data_out);

	/* Print the resulting matrix */
	for (i = 0; i < MAX_M_SIZE; ++i)
	{
		for (j = 0; j < MAX_M_SIZE; ++j)
		{
			printf("%d ", data_out->mat[i][j]);
		}
		printf("\n");
	}
	return 0;
}
