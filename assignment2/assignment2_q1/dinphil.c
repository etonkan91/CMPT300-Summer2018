/*
Dining Philosopher using resource hierarchy solution
Author: Hei Tung Kan
Student ID: 301235768
Course: CMPT300
Date: July 8, 2018
File: dinphil.c
Compile Instruction: gcc dinphil.c -o dinphil -lpthread

Assignment 2 Question 1
Example deadlock scenario:
Resource hierarchy solution for dining philosopher will not generate deadlocks because the lower number philosopher must wait for the higher number philosopher to finish eating before locking any resources. Seem the highest philosopher will always be able to eat (it have its own fork and lower number philosopher must wait for higher number philosopher to finish eating). Deadlocks where two philosophers are waiting for each other's resource will not happen. Therefore, there are no specific input and expected output for TA to verify as Deadlock will never occur.
*/

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<signal.h>
 
sem_t sem_fork[5]; /* Waiting for Fork Semaphore */
int state[5] = {}; /* philosophers' state init to Thinking(0) */
pthread_mutex_t mut_state = PTHREAD_MUTEX_INITIALIZER; /* Mutex for philosophers' state */
pthread_mutex_t mut_phil[5] = PTHREAD_MUTEX_INITIALIZER; /* Mutex for Thinking and Eating */
pthread_cond_t eat_cv[5] = PTHREAD_COND_INITIALIZER; /* Eating */
pthread_cond_t think_cv[5] = PTHREAD_COND_INITIALIZER; /* Thinking */

/* Thinking(0), Eating(1), Hungry(2) */
void eatTest(int num)
{
    if (num == 0)
    {
        if (state[num] == 2 && state[num+1] != 1 && state[num+2] != 1 )
        {
            state[num] = 1;
            sem_post(&sem_fork[num]);
        }
    }
    else if (num == 2)
    {
        if (state[num] == 2 && state[num-1] != 1 && state[num-2] != 1 && state[num+1] != 1)
        {
            state[num] = 1;
            sem_post(&sem_fork[num]);
        }
    }
    else if (num == 4)
    {
        if (state[num] == 2 && state[num-1] != 1)
        {
            state[num] = 1;
            sem_post(&sem_fork[num]);
        }
    }
    else
    {
        if (state[num] == 2 && state[num-1] != 1 && state[num+1] != 1)
        {
            state[num] = 1;
            sem_post(&sem_fork[num]);
        }
    }
}

void* phil(int num)
{
    while(1) /* true */
    {
        /* Thinking */
        pthread_cond_wait(&think_cv[num], &mut_phil[num]);
        /* Grab sem_fork */
        pthread_mutex_lock(&mut_state);
        state[num] = 2; /* Change state to hungry */
        eatTest(num);
        pthread_mutex_unlock(&mut_state);
        /* Waiting for Fork */
        sem_wait(&sem_fork[num]); 
        
        /* Eating */
        pthread_cond_wait(&eat_cv[num], &mut_phil[num]);
        /* Return sem_fork */
        pthread_mutex_lock(&mut_state);
        state[num] = 0; /* Change state to thinking */
        if (num == 0)
        {
            eatTest(num+2);
            eatTest(num+1);
        }
        else if (num == 2)
        {
            eatTest(num+1);
            eatTest(num-1);
            eatTest(num-2);
        }
        else if (num == 4)
        {
            eatTest(num-1);
        }
        else
        {
            eatTest(num+1);
            eatTest(num-1);
        }
        pthread_mutex_unlock(&mut_state);
    }
}

int main()
{
    int i, input_num = 0;
    char input_char = ' ';
    pthread_t tid[5];
    
    for(i=0;i<5;++i)
    {
        /* Creating 5 Philosophers represented with 5 threads*/
        sem_init(&sem_fork[i],0,0);
        pthread_create(&tid[i],NULL,(void *)phil, (void *) i);
    }
    
    while (1) /* True */
    {
        scanf(" %c", &input_char);
        
        if(input_char == '!')
        {
            /* Exit Program */ 
            for (i = 0; i < 5; ++i)
            {            
		        /* Close all threads, semaphore, mutex, and CV*/
                pthread_kill(tid[i],0);
                sem_close(&sem_fork[i]);
                pthread_mutex_destroy(&mut_phil[i]);
                pthread_cond_destroy(&eat_cv[i]);
                pthread_cond_destroy(&think_cv[i]);
            }
            break;
        }
        else if (input_char == 'P')
        {
            /* Print states of all philosophers 
               Note: when state is 2, it is in hungry(2) which is same as thinking (0) */
            for(i = 0; i < 4; ++i)
            {
                printf("%d ", (state[i] == 2? 0:state[i]));
            }
            printf("%d\n", (state[i] == 2? 0:state[i]));
        }
        else if (input_char == 'T')
        {
            /* Request Philosopher to Think */
            scanf("%d", &input_num);
            pthread_cond_signal(&eat_cv[input_num]);
        }
        else if (input_char == 'E')
        {
            /* Request Philosopher to Eat */
            scanf("%d", &input_num);
            pthread_cond_signal(&think_cv[input_num]);
        }
        else 
        { 
            /* For invalid input */        
            /* Assignment said all input are valid */
        }
    }    
    return 0;
}
