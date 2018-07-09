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
#include<semaphore.h>
#include<pthread.h>

struct detData 
{
    int n;
    int** mtx;
};

void* det(void *args)
{
    int i,j,m,nrow, ncol, ans = 0;
    struct detData *temp = (struct detData*) args;
    pthread_t subtid[temp->n];
    int subDet[temp->n];

    if (n == 2)
    {
        ans = (temp->mtx[0][0] * temp->mtx[1][1])-(temp->mtx[0][1] * temp->mtx[1][0]);
    }
    else
    {
        struct detData[temp->n];
        for (m = 0; m < n; m++)
        {
            /* Make temp matrix (sub matrix)*/
            int ** submtx = (int**) malloc (temp->n-- * sizeof(int*));    
            for (i = 0; i < temp->n--; i ++)
            {
                submtx[i] = (int *) malloc (temp->n-- * sizeof(int));
            }
            
            /* Populate the sub matrix */
            nrow = 0;
            ncol = 0;
            for (i = 0; i < temp->n--; i++)
            {
                if ( i == m)
                {
                    continue;
                }
                for (j = 0; j < temp->n--; j++)
                {
                    if ( j == m)
                    {
                        continue;
                    }
                    submtx[nrow][ncol] = temp->mtx[i][j];
                    ncol++;
                }
                nrow++;
            }   
            
            /* Create new thread to compute sub matrix's determinant*/
            detData[i].n = temp->n--;
            detData[i].mtx = submtx;
            pthread_create(&subtid[i],NULL,(void *)det, &detData[i]);
        }
        
        for (m = 0; m < n; m++)
        {
            pthread_join(subtid[m], &subDet[m]);
            if (m % 2 == 0)
            {
                ans += (temp->mtx[0][m] * subDet[m]);
            }
            else
            {
                ans -= (temp->mtx[0][m] * subDet[m]);
            }
        }
    }

    pthread_exit(ans);
}

int main()
{
    int n, i, j, ans = -1;
    struct detData findDet;
    pthread_t tid;
    
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
            mtx[i][j] = rand();
        }
    }   
    findDet.n = n;
    findDet.mtx = mtx;
    
    pthread_create(&tid,NULL,(void *)det, &findDet);
    
    pthread_join(tid, &ans);
    printf("\n Determinant is %d \n", ans);
    return 0;
}
