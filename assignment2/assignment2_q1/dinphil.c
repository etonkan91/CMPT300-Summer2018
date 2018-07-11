/*
Dining Philosopher using resource hierarchy solution
Author: Hei Tung Kan
Student ID: 301235768
Course: CMPT300
Date: July 8, 2018
Modified: July 10, 2018
File: dinphil.c
Compile Instruction: gcc dinphil.c -o dinphil -lpthread
*/

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include <unistd.h>

int isEnd[5] = {}; /* Thread should end */
int state[5] = {}; /* Philosophers' state init to Thinking(0) */
pthread_mutex_t mut_state = PTHREAD_MUTEX_INITIALIZER; /* Mutex for philosophers' state array */
pthread_mutex_t fork_mut[5] = PTHREAD_MUTEX_INITIALIZER; /* Mutex for Fork */
pthread_mutex_t think_eat_mut[5] = PTHREAD_MUTEX_INITIALIZER; /* Mutex for Thinking and Eating */
pthread_cond_t eat_cv[5] = PTHREAD_COND_INITIALIZER; /* Eating */
pthread_cond_t think_cv[5] = PTHREAD_COND_INITIALIZER; /* Thinking */

/* Thinking(0), Eating(1), Hungry(2) */
void* phil(void* args)
{
    int num = *((int *) args);
    int ret = 0;
    while(!isEnd[num])
    {
        /* Thinking */
        pthread_cond_wait(&think_cv[num], &think_eat_mut[num]);
        
        if (isEnd[num])
        {
            /* Got end program signal (isEnd) */
            break;
        }
        
        /* Grab fork */
        if (num == 4)
        {
            pthread_mutex_lock(&fork_mut[((num + 1) % 5)]); /* Left lock (Lower, Fork 0)) */
            pthread_mutex_lock(&fork_mut[num]);
        }
        else
        {
            pthread_mutex_lock(&fork_mut[num]);
            pthread_mutex_lock(&fork_mut[((num + 1) % 5)]); /* Left lock (Higher) */
        }
        
        /* Acquired both forks, start eating */
        pthread_mutex_lock(&mut_state);
        state[num] = 1; /* Change state to eating */
        pthread_mutex_unlock(&mut_state);
        pthread_cond_wait(&eat_cv[num], &think_eat_mut[num]);
        
        /* Starting thinking, release forks */ 
        pthread_mutex_lock(&mut_state);
        state[num] = 0; /* Change state to thinking */
        pthread_mutex_unlock(&mut_state);
        
        /* Release forks (Order doesn't matter) */
        if (num == 4)
        {
            pthread_mutex_unlock(&fork_mut[((num + 1) % 5)]); /* Left lock (Lower, Fork 0)) */
            pthread_mutex_unlock(&fork_mut[num]);
        }
        else
        {
            pthread_mutex_unlock(&fork_mut[num]);
            pthread_mutex_unlock(&fork_mut[((num + 1) % 5)]); /* Left lock (Higher) */
        }
    }
    pthread_exit(&ret);
}

int main()
{
    int i = 0, input_num = -1;
    int phil_num[5] = {0,1,2,3,4};
    char input[4] = {' ',' ',' ',' '};
    char output[10] = {' ',' ',' ',' ',' ',' ',' ',' ',' ','\n'};
    pthread_t tid[5];
    
    for(i = 0; i < 5; ++i)
    {
        /* Creating 5 Philosophers represented with 5 threads*/
        pthread_create(&tid[i],NULL,(void *)phil, &phil_num[i]);
         
    }
    
    while (1) /* True */
    {

        /* Read user input */
        read(0, &input, 4);
        
        if(input[0] == '!')
        {
            /* Exit Program */ 
            break;
        }
        else if (input[0] == 'P')
        {
            /* Print states of all philosophers */
            for(i = 0; i < 10; i+=2)
            {
                output[i] = state[i/2] + '0';
            }
            write(1, &output, 10);
        }
        else if (input[0] == 'T')
        {
            input_num = input[2] - '0';
            /* Request Philosopher to Think */
            pthread_cond_signal(&eat_cv[input_num]);
        }
        else if (input[0] == 'E')
        {
            input_num = input[2] - '0';
            /* Request Philosopher to Eat */
            pthread_cond_signal(&think_cv[input_num]);
        }
        else 
        { 
            /* For invalid input */        
            /* Assignment said all input are valid */
        }
    }
    
    /* join threads and destory mutex and cv */
    for (i = 0; i < 5; ++i)
    {
        /* Signaling each thread that it is ending */
        isEnd[i] = 1;
        
        pthread_cond_signal(&eat_cv[i]);
        pthread_cond_signal(&think_cv[i]);
        
        /* Wait for threads to close */
        pthread_join(tid[i],NULL);
    }
    
    for (i = 0; i < 5; ++i)
    {
        /* Destory mutex and cv*/
        pthread_mutex_destroy(&fork_mut[i]);
        pthread_cond_destroy(&eat_cv[i]);
        pthread_cond_destroy(&think_cv[i]);
        pthread_mutex_destroy(&think_eat_mut[i]);
    }
    
    pthread_mutex_destroy(&mut_state);
    return 0;
}
