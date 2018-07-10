/*
Find determinant of given NxN matrix
Author: Hei Tung Kan
Student ID: 301235768
Course: CMPT300
Date: July 8, 2018
Modified: July 10, 2018
File: FindDet.c
Compile Instruction: gcc FindDet.c -o FindDet -lpthread
Assignment2 Question 2
*/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>

struct detData 
{
    int sizeN;
    int** mtx;
    int detAns;
    pthread_t tid;
};

void* det(void *args)
{
    struct detData *temp = (struct detData*) args;
    int i = 0,j = 0,m = 0,submtx_i = 0, submtx_y = 0;
    int detAns = 0, ret = 0;
    int sign = 1;
    int tempN = temp->sizeN;
    
    if (tempN == 2)
    {
        /* Return the determinant of a 2x2 matrix */
        detAns = (temp->mtx[0][0] * temp->mtx[1][1])-(temp->mtx[0][1] * temp->mtx[1][0]);
    }
    else
    {
        /* Separate the original matrix in to sub matrix for easlier calculation */
        struct detData subData[tempN];
        
        for (m = 0; m < tempN; m++)
        {
            /* Make temp matrix (sub matrix) */
            int ** submtx = (int**) malloc ((tempN-1) * sizeof(int*));    
            for (i = 0; i < (tempN-1); i++)
            {
                submtx[i] = (int *) malloc ((tempN-1) * sizeof(int));
            }
            
            /* Populate the sub matrix */
            submtx_i = 0;
            for (i = 1; i < tempN; i++)
            {
                submtx_y = 0;
                for (j = 0; j < tempN; j++)
                {
                    if (j != m)
                    {
                        submtx[submtx_i][submtx_y] = temp->mtx[i][j];
                        submtx_y++;
                    }
                }
                submtx_i++;
            }
            /* Create new thread to compute sub matrix's determinant*/
            subData[m].sizeN = (tempN-1);
            subData[m].mtx = submtx;
            pthread_create(&subData[m].tid,NULL,(void *)det, &subData[m]);
        }
        
        for (m = 0; m < temp->sizeN; m++)
        {
            /* Wait for subthreads to be done */
            pthread_join(subData[m].tid, NULL);
            
            /* Alternating sign for adding first row */
            detAns += sign * (temp->mtx[0][m] * subData[m].detAns);
            sign = -1 * sign;
			
			/* Release dynamic allocated memories */
			for(i = 0; i < tempN-1; i++)
			{
				free(subData[m].mtx[i]);
			}
			free(subData[m].mtx);

        }
    }
	/* Save the sub matrix's determinant and close the thread */
    temp->detAns = detAns;
    pthread_exit(&ret);
}

int main()
{
    int sizeN = -1;
    int i = -1, j = -1;
    struct detData findDet;
                              
    while (1)
    {
       printf("Input(N):");
       scanf("%d", &sizeN);
       if (sizeN >= 2)
       {
            break;
            
       }
       printf("\nThis program only support when sizeN >= 2\n");
    }
    
    /* Example One */
    /*
    sizeN = 4;
    int test[4][4] = {{0, 4, 0, -3},
                      {1, 1, 5, 2},
                      {1, -2, 0, 6},
                      {3, 0, 0, 1}}; /* det = -250 */
    /* Example 2 */
    /*
    sizeN = 4;
    int test[4][4] = {{1, 0, 2, -1},
                     {3, 0, 0, 5},
                     {2, 1, 4, -3},
                     {1, 0, 5, 0}}; /* det = 30 */

    /* Making rand() matrix NxN */
    srand(time(NULL));
    int ** mtx = (int**) malloc (sizeN * sizeof(int*));    
    for (i = 0; i < sizeN; i ++)
    {
        mtx[i] = (int *) malloc (sizeN * sizeof(int));
    }

    for (i = 0; i < sizeN; i++)
    {
        for (j = 0; j < sizeN; j++)
        {
            mtx[i][j] = rand();
            /*mtx[i][j] = test[i][j];*/
        }
    }
    
    findDet.sizeN = sizeN;
    findDet.mtx = mtx;
    pthread_create(&findDet.tid,NULL,(void *)det, &findDet);
    
    pthread_join(findDet.tid, NULL);

    printf("\nOutput:\n");
    printf("Determinant: %d\n", findDet.detAns);

	/* Release dynamic allocated memories */
    for(i = 0; i < sizeN; i++)
    {
		free(mtx[i]);
    }
    free(mtx);

    return 0;
}
