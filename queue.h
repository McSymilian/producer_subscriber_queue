#ifndef LCL_QUEUE_H
#define LCL_QUEUE_H

// ==============================================
//
//  Version 1.1, 2025-01-16
//
// ==============================================

#include <pthread.h>

struct TQueue {
    int maxSize;
    struct SubscribersList *allSubscribers;
    struct MessagesList *messages;

    pthread_mutex_t operational_mutex;
    pthread_cond_t msg_posted_cond;
    pthread_cond_t msg_fully_received_cond;
};
typedef struct TQueue TQueue;

TQueue* createQueue(int size);

void destroyQueue(TQueue *queue);

void subscribe(TQueue *queue, pthread_t thread);

void unsubscribe(TQueue *queue, pthread_t thread);

void addMsg(TQueue *queue, void *msg);

void* getMsg(TQueue *queue, pthread_t thread);

int getAvailable(TQueue *queue, pthread_t thread);

void removeMsg(TQueue *queue, void *msg);

void setSize(TQueue *queue, int size);

#endif //LCL_QUEUE_H
