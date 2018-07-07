/*
Dining Philosopher using resource hierarchy solution
Author: Hei Tung Kan
Course: CMPT300
Assignment 2 Question 1
File: dinphil.c
*/

#include<unistd.h>
#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
 
sem_t mphil[5]; /* Total of 5 Philosophers */ 
int state[5] = {};
pthread_cond_t eat_cv[5];
pthread_cond_t think_cv[5];
pthread_mutex_t mut_state;
pthread_mutex_t mp[5];

void test(int num)
{
    if (num == 0)
    {
        if (state[num] == 2 && state[num+1] != 1)
        {
            state[num] = 1;
            sem_post(&mphil[num]);
        }
    }
    else 
    {
        if (state[num] == 2 && state[num-1] != 1)
        {
            state[num] = 1;
            sem_post(&mphil[num]);
        }
    }
}

void phil(int num)
{
    while(true)
    {
        /* Thinking */
        pthread_cond_wait(&think_cv[num], &mp[num]);
        /* Grab fork */
        pthread_mutex_lock(&mut_state);
        state[num] = 2;
        test(num);
        pthread_mutex_unlock(&mut_state);
        sem_wait(&mphil[num]);
        
        /* Eating */
        pthread_cond_wait(&eat_cv[num], &mp[num]);
        /* Return fork */
        pthread_mutex_lock(&mut_state);
        state[num] = 0;
        if (num == 0)
        {
            test(num+1);
        }
        else if (num == 4)
        {
            test(num-1);
        }
        else
        {
            test(num+1);
            test(num-1);
        }
        pthread_mutex_unlock(&mut_state);
    }
}

int main()
{
    int i, input_num = 0;
//    int input_num = 0;
    char input_char = ' ';
    
    pthread_t tid[5];
    pthread_mutex_t(&mut_state, NULL);
    
    for(i=0;i<5;i++){
        sem_init(&mphil[i],0,0);
        pthread_cond_init(&eat_cv[i], NULL);
        pthread_cond_init(&think_cv[i], NULL);
        pthread_mutex_init(&mp[i], NULL);
        pthread_create(&tid[i],NULL,phil, i);
    }
      
    while (true)
    {
        scanf("%c", &input_char);
        
        if(input_char == '!')
        {
            /* Exit Program */ 
            for (i = 0; i < 5; i++)
            {
                /* Close all threads */
                pthread_join(&tid[i], NULL);
                sem_destory(&mphil[i]);
                pthread_mutex_destroy(&mp[i]);
                pthread_cond_destroy(&eat_cv[i]);
                pthread_cond_destroy(&think_cv[i]);
            }
            break;
        }
        else if (input_char == 't' || input_char == 'T')
        {
            /* Request Philosopher to Think */
            scanf("%d", &input_num);
            pthread_cond_signal(&eat_cv[input_num]);
        }
        else if (input_char == 'e' || input_char == 'E')
        {
            /* Request Philosopher to Eat */
            scanf("%d", &input_num);
            pthread_cond_signal(&think_cv[input_num]);
        }
        else if (input_char == 'p' || input_char == 'P')
        {
            /* Print states of philosophers 
               Note: when state is 2, it is in hungry(2) which is same as thinking (0) */
            for(i = 0; i < 4; i++)
            {
                printf("%d ", (state[i] == 2? 0:state[i]));
            }
            printf("%d\n", (state[i] == 2? 0:state[i]));
        }
        else
        {
            printf("Error: unknown input \n");
            printf("Only allow T,E,P, and ! \n");
        }
    }    
    return NULL;
}
