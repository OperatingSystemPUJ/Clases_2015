#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Globals for the results */
int average = 0;
int minimum = 0;
int maximum = 0;
int numbers_length = 0;

void* f_average(void * numbers)
{
	int count = 0, i;
	for (i = 0; i < numbers_length; ++i)
	{
		count += ((int*)numbers)[i];
	}

	average = count / numbers_length;
	pthread_exit(NULL);
}

void* f_minimum(void * numbers)
{
	int local_minimum = ((int*)numbers)[0];
	int current = 0, i;
	for (i = 0; i < numbers_length; ++i)
	{
		current = ((int*)numbers)[i];
		if (current < local_minimum)
		{
			local_minimum = current;
		}
	}

	minimum = local_minimum;
	pthread_exit(NULL);
}

void* f_maximum(void * numbers)
{
	int local_maximum = ((int*)numbers)[0];
	int current = 0, i;
	for (i = 0; i < numbers_length; ++i)
	{
		current = ((int*)numbers)[i];
		if (current > local_maximum)
		{
			local_maximum = current;
		}
	}

	maximum = local_maximum;
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t t_average, t_minimum, t_maximum;
	int *numbers;
	int i;
	/* User input */
	printf("Enter the number of numbers to process: \n");
	scanf("%d", &numbers_length);
	numbers = (int*) malloc	(numbers_length * sizeof(int));

	printf("Enter the numbers: \n");
	for (i = 0; i < numbers_length; ++i)
	{
		scanf("%d", numbers+i);
	}

	pthread_create(&t_average, NULL, f_average, (void *) numbers);
	pthread_create(&t_minimum, NULL, f_minimum, (void *) numbers);
	pthread_create(&t_maximum, NULL, f_maximum, (void *) numbers);
	
	pthread_join(t_average, NULL);
	pthread_join(t_minimum, NULL);
	pthread_join(t_maximum, NULL);

	printf("The average is: %d\n", average);
	printf("The minimum is: %d\n", minimum);
	printf("The maximum is: %d\n", maximum);

	free(numbers);
	return 0;
}
