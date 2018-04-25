//
//  main.c
//  Producer_Consumer
//
//  Created by PEITONG SHI on 05/11/2017.
//  Copyright © 2017 PEITONG SHI. All rights reserved.
//
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <semaphore.h>

#define BUFF_SIZE 16

int buff[BUFF_SIZE]; 
pthread_mutex_t lock;
sem_t full;
sem_t empty;
int counter = 0;
int num_item_per_producer;
int num_item_per_consumer;
int num_items;


struct p_pthread {
    int thread_index;
    int numProduce;
};


void *produce(void *para) {
    struct p_pthread* p = (struct p_pthread*) para;
    int count = 0;
    int item; // to be produed
    
    while (p->numProduce > count) {
        
        item = p->thread_index * p->numProduce + count;
        sem_wait(&empty);//atom, avoid producer and consumer conflicts
        pthread_mutex_lock(&lock);
        //critical session
        
        if (BUFF_SIZE>counter) {
            buff[counter] = item;
            printf("\nProducer %i produce %i at position %i",(p->thread_index +1),item,counter);
            counter++;
        }
        else {
            exit(1);
        }
        //critical end
        pthread_mutex_unlock(&lock);
        sem_post(&full);
        count++;
    }
    pthread_exit(0);
}

void *consume(void *para) {
    int consumed = *(int*) para;
    int item; // to be consumed
    int count = 0;
    
    while (consumed > count)  {
        sem_wait(&full);//atom, avoid producer and consumer conflicts
        pthread_mutex_lock(&lock);
        //critical session

        if (counter > 0) {
            counter--;
            item = buff[counter]; //assign the value to the item from the buff[counter-1]
            printf("\nConsumer consumes %i at position %i", item,count);
            buff[counter] = 0;  //delete position "counter"s value in buff
        }
        else {
            exit(1);
        }
        //critical end
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
        count++;
    }
    pthread_exit(0);
}


int main(int argc, const char * argv[]) {
    int num_of_producer = atoi(argv[1]);
    int num_of_consumer = atoi(argv[2]);
    int total_amount_of_items_tobe_produced = atoi(argv[3]) * num_of_producer;
    num_item_per_consumer = total_amount_of_items_tobe_produced / num_of_consumer;
    num_item_per_producer = total_amount_of_items_tobe_produced / num_of_producer;
    
    printf("Number of producers: %i\nNumber of consumers: %i\nTotal amount of items: %i\n", num_of_producer, num_of_consumer, total_amount_of_items_tobe_produced);
    printf("%i for each producer\n%i for each consumer",num_item_per_producer,num_item_per_consumer);
    
    
    
    pthread_t *producers;
    pthread_t *consumers;
    
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFF_SIZE);
    
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\nMutex lock initialization error\n");
        return 1;
    }
    //memory allocation
    producers = (pthread_t*) malloc(num_of_producer*sizeof(pthread_t));
    consumers = (pthread_t*) malloc(num_of_consumer*sizeof(pthread_t));
    
    struct p_pthread pt[num_of_producer];
    //init each producer thread
    for (int i = 0 ; i < num_of_producer; i++) {
        pt[i].thread_index = i;
        pt[i].numProduce = num_item_per_producer;
        //create thread and make them work
        if (pthread_create(&producers[i], NULL, produce, &pt[i])!=0){
            printf("ERROR at producer creating\n");
            return 1;
        }
    }
    //init consumer and make them work
    for (int i = 0; i < num_of_consumer;i++) {
        if(pthread_create(&consumers[i], NULL, consume, &num_item_per_consumer) != 0){
            printf("ERROR at consumer creating\n");
            return 1;
        }
    }
    //wait threads terminated
    for(int i = 0; i < num_of_producer; i++){
        pthread_join(producers[i], NULL);
    }
    
    for(int i=0; i < num_of_consumer; i++){
        pthread_join(consumers[i], NULL);
    }
    
    
    free(producers);
    free(consumers);
    pthread_mutex_destroy(&lock);
    sem_destroy(&full);
    sem_destroy(&empty);
    //free allocated memory and destory everything used
    return 0;
}

