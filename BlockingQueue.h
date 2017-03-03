//
//  BlockingQueue.h
//  595_project1
//
//  Created by David Milewicz on 2/28/17.
//  Copyright © 2017 David Milewicz. All rights reserved.
//

#ifndef BlockingQueue_h
#define BlockingQueue_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/types.h>

#endif /* BlockingQueue_h */




typedef struct BlockingQueue {
    size_t queue_size, cur_size;
    size_t head, tail;
    sem_t insert;
    pthread_mutex_t lock;
    pthread_t** entries;
} blocking_queue;




int blocking_queue_create(size_t q_size) {
    
    // create key
    key_t key = 5678;
    
    // create shared memory
    int mem_id = shmget(key, sizeof(blocking_queue) + (sizeof(pthread_t*) * q_size), IPC_CREAT);
    
    if (mem_id == -1) return -1;
    
    blocking_queue* Q = shmat(mem_id, NULL, 0);
    
    Q->queue_size = q_size;
    Q->cur_size = Q->head = Q->tail = 0;
    

//    pthread_mutex_init(, );
    
    
    
    
    
    
    
//    blocking_queue* q = calloc(1, sizeof(blocking_queue));
//    q->entries = malloc(sizeof(blocking_queue*) * QUEUE_SIZE);
    return 0;
}


