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
    int ans = 0;
    struct detData *temp = (struct detData*) args;
    pthread_t tid[temp->n];
    int subDet[temp->n];

    if (n == 2)
    {
        ans = (temp->mtx[0][0] * temp->mtx[1][1])-(temp->mtx[0][1] * temp->mtx[1][0]);
    }
    else
    {
        
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
