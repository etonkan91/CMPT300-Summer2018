/*
Dining Philosopher using resource hierarchy solution
Author: Hei Tung Kan
Student ID: 301235768
Course: CMPT300
Date: July 8, 2018
File: dinphil_no_sema.c
Compile Instruction: gcc dinphil.c -o dinphil -lpthread

Assignment 2 Question 1
Example deadlock scenario:
Resource hierarchy solution for dining philosopher will not generate deadlocks because the lower number philosopher must wait for the higher number philosopher to finish eating before locking any resources. Seem the highest philosopher will always be able to eat (it have its own fork and lower number philosopher must wait for higher number philosopher to finish eating). Deadlocks where two philosophers are waiting for each other's resource will not happen. Therefore, there are no specific input and expected output for TA to verify as Deadlock will never occur.
*/

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<signal.h>
 
int state[5] = {}; /* philosophers' state init to Thinking(0) */
pthread_mutex_t mut_state = PTHREAD_MUTEX_INITIALIZER; /* Mutex for philosophers' state */
pthread_mutex_t fork_mut[5] = PTHREAD_MUTEX_INITIALIZER; /* Mutex for Eating */
pthread_mutex_t eat_mut[5] = PTHREAD_MUTEX_INITIALIZER; /* Mutex for Eating */
pthread_mutex_t think_mut[5] = PTHREAD_MUTEX_INITIALIZER; /* Mutex for Thinking */
pthread_cond_t eat_cv[5] = PTHREAD_COND_INITIALIZER; /* Eating */
pthread_cond_t think_cv[5] = PTHREAD_COND_INITIALIZER; /* Thinking */

/* Thinking(0), Eating(1), Hungry(2) */
void* phil(int num)
{
    while(1) /* true */
    {
        /* Thinking */
        pthread_cond_wait(&think_cv[num], &think_mut[num]);
        
        /* Grab fork */
        if (num == 0)
        {
            pthread_mutex_lock(&fork_mut[num]);
            pthread_mutex_lock(&fork_mut[((num + 4) % 5)]); /* Left lock (Higher) */
        }
        else
        {
            pthread_mutex_lock(&fork_mut[((num + 4) % 5)]); /* Left lock (lower) */
            pthread_mutex_lock(&fork_mut[num]);
        }
        
        /* Acquired both forks, start eating */
        pthread_mutex_lock(&mut_state);
        state[num] = 1; /* Change state to eating */
        pthread_mutex_unlock(&mut_state);
        pthread_cond_wait(&eat_cv[num], &eat_mut[num]);
        
        /* Release forks */ 
        pthread_mutex_lock(&mut_state);
        state[num] = 0; /* Change state to thinking */
        pthread_mutex_unlock(&mut_state);
        
        pthread_mutex_unlock(&fork_mut[((num + 4) % 5)]); /* Left lock (lower) */
        pthread_mutex_unlock(&fork_mut[num]);
        
    }
}

int main()
{
    int i, input_num = 0;
    char input_char = ' ';
    pthread_t tid[5];
    
    for(i = 0; i < 5; ++i)
    {
        /* Creating 5 Philosophers represented with 5 threads*/
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
                pthread_mutex_destroy(&fork_mut[i]);
                pthread_mutex_destroy(&eat_mut[i]);
                pthread_mutex_destroy(&think_mut[i]);
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
                printf("%d ", state[i]);
            }
            printf("%d\n", state[i]);
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
