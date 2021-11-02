/*
 * producer&consumer.c
 *
 * Created on: Nov 5, 2020
 * Author: Aishwarya
 */



#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXITEMS 10 // Maximum items a producer can produce or a consumer can consume
#define SIZEOFBUFFER 10 // Size of the buffer

//global variables
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[SIZEOFBUFFER];
pthread_mutex_t mutex;

/**
 * producer() will produce items
 * .
 * @param other the other node object to copy.
 */
void *producer(void *pro);
void *consumer(void *cno);


int main()
{

    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,SIZEOFBUFFER);
    sem_init(&full,0,0);

    int a[10] = {1,2,3,4,5,6,7,8,9,10}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 10; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 10; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 10; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 10; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;

}

void *producer(void *pno)
{
    int item;
    for(int i = 0; i < MAXITEMS; i++) {
        item = rand(); // Produce an random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%SIZEOFBUFFER;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *cno)
{
    for(int i = 0; i < MAXITEMS; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%SIZEOFBUFFER;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
