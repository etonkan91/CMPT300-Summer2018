/*
Find determinant of given NxN matrix
Author: Hei Tung Kan
Student ID: 301235768
Course: CMPT300
Date: July 8, 2018
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
    int n;
    int** mtx;
    int ans;
    pthread_t tid;
};

void* det(void *args)
{
    printf("New Thread \n");
    int i,j,m,nrow, ncol, ans, sign, ret = 0;
    struct detData *temp = (struct detData*) args;
//    pthread_t subtid[temp->n];
    int tempN = temp->n;
    int subDet;
    printf("tempN = %d \n", tempN);
    if (tempN == 2)
    {
        printf("temp->n == 2 \n");
        ans = (temp->mtx[0][0] * temp->mtx[1][1])-(temp->mtx[0][1] * temp->mtx[1][0]);
    }
    else
    {
        printf("else \n");
        struct detData subData[tempN];
        for (m = 0; m < tempN; m++)
        {
            /* Make temp matrix (sub matrix)*/
            int ** submtx = (int**) malloc ((tempN-1) * sizeof(int*));    
            for (i = 0; i < (tempN-1); i++)
            {
                submtx[i] = (int *) malloc ((tempN-1) * sizeof(int));
            }
            printf("Create new array \n");
            /* Populate the sub matrix */
            nrow = 0;
            for (i = 1; i < tempN; i++)
            {
                ncol = 0;
                for (j = 0; j < tempN; j++)
                {
                    if (j != m)
                    {
                        submtx[nrow][ncol] = temp->mtx[i][j];
                        printf("nrow= %d, ncol= %d, i= %d,j= %d \n", nrow,ncol,i,j);
                        ncol++;
                    }
                    
                }
                nrow++;
            }   
            printf("Create new thread \n");
            /* Create new thread to compute sub matrix's determinant*/
            subData[i].n = (tempN-1);
            subData[i].mtx = submtx;
            ret = pthread_create(&subData[m].tid,NULL,(void *)det, &subData[m]);
            printf("ret = %d \n", ret);
        }
        
        for (m = 0; m < temp->n; m++)
        {
            pthread_join(subData[m].tid, NULL);
            ans += sign * (temp->mtx[0][m] * subData[m].ans);
            sign = -1 * sign;
        }
    }

    temp->ans = ans;
}

int main()
{
    int n, i, j, ans = -1;
    struct detData findDet;
//    pthread_t tid;
    
    n = 3;
    int test[3][3] = {{0, 2, -1},
                     {0, 0, 5},
                     {1, 4, -3}};
    /*
    int test[4][4] = {{1, 0, 2, -1},
                     {3, 0, 0, 5},
                     {2, 1, 4, -3},
                     {1, 0, 5, 0}}; /* det = 30 */
    /*
    while (1)
    {
       printf("Input(N): ");
       scanf("%d", &n);
       if (n >= 2)
       {
            break;
            
       }
       printf("\n This program only support when N >= 2 \n");
    }
    */
    /* Making rand() matrix NxN */
    
    srand(time(NULL));
    int ** mtx = (int**) malloc (n * sizeof(int*));    
    for (i = 0; i < n; i ++)
    {
        mtx[i] = (int *) malloc (n * sizeof(int));
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            //mtx[i][j] = rand() % 10;
            mtx[i][j] = test[i][j];
        }
    }
      
    findDet.n = n;
    findDet.mtx = mtx;
    
    pthread_create(&findDet.tid,NULL,(void *)det, &findDet);
    
    pthread_join(findDet.tid, NULL);
    printf("\nDeterminant is %d \n", findDet.ans);
    return 0;
}
