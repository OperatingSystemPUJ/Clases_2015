#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//      ___      ___                     
//     /\  \    /\  \                    
//    /::\  \  /::\  \                   
//   /:/\:\__\/::\:\__\                  
//   \:\ \/__/\/\::/  /                  
//    \:\__\    /:/  /                   
//     \/__/    \/__/                    
//      ___      ___      ___      ___   
//     /\  \    /\__\    /\  \    /\  \  
//    /::\  \  /:/ _/_  /::\  \  /::\  \ 
//   /:/\:\__\/::-"\__\/::\:\__\/\:\:\__\
//   \:\:\/__/\;:;-",-"\/\::/  /\:\:\/__/
//    \::/  /  |:|  |    /:/  /  \::/  / 
//     \/__/    \|__|    \/__/    \/__/  



// gcc -o Scalar_multiply_Matrix Scalar_multiply_Matrix.c -lpthread

#define NROWS 5
#define NCOLS 5
#define SCALAR 2

int matrix[NROWS][NCOLS];

void *mult(void *tid) 
{
	int i, j, *mytid, ctid;
	
	mytid = (int *) tid;
	
	printf ("Thread %d so multiply row number: %d\n",*mytid,*mytid); 
	
	for (i=0; i < NCOLS ; i++) {
		matrix[*mytid][i] = matrix[*mytid][i] * SCALAR;	
	}

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	
	int i, j;		
		
	printf("This a defined matrix %d*%d (rows*cols) so enter your matrix: \n", NROWS,NCOLS);
	for(i=0;i<NROWS;i++)
	{
		for(j=0;j<NCOLS;j++)
		  {  
		    scanf("%d",&matrix[i][j]);  
		  }  
	}
	printf("Your matrix was built as follows: \n");
	for(i = 0; i < NROWS; ++i)
	{
	   for(j = 0; j < NCOLS ; ++j) 
	   {
	      printf("%d\t",matrix[i][j]);
	   }
	printf("\n");
	}	

	printf("The scalar is defined as %d\n", SCALAR);	
	
	int tids[NROWS];
	pthread_t threads[NROWS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (i=0; i<NROWS; i++) {
		tids[i] = i;
		pthread_create(&threads[i], &attr, mult, (void *) &tids[i]);
	}

	for (i=0; i<NROWS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("The result matrix is:\n");
	for(i = 0; i < NROWS; ++i)
	{
	   for(j = 0; j < NCOLS ; ++j) 
	   {
	      printf("%d\t",matrix[i][j]);
	   }
	printf("\n");
	}
	return 0;
}