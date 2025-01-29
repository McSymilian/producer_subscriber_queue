#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "queue.h"

void *initTest(void *args) {
    TQueue *queue = createQueue(10);

    bool success = queue != NULL
                && queue->maxSize == 10;
    destroyQueue(queue);

    printf("initTest: %s\n", success ? "success" : "failed");

    return NULL;
}

void *threadBusyWait(void *args) {
    while (true) {}
}

void *singleSubscriberMultipleMessagesSequentialReceivedTest(void *args) {
    TQueue *queue = createQueue(10);

    pthread_t* thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, threadBusyWait, NULL);

    subscribe(queue, *thread);

    int *msg = malloc(sizeof(int));
    *msg = 10;
    addMsg(queue, msg);

    const int available = getAvailable(queue, *thread);
    const int* msg2 = getMsg(queue, *thread);
    const int available2 = getAvailable(queue, *thread);

    bool success = available == 1
                && available2 == 0
                && *msg == *msg2;

    if (!success) {
        destroyQueue(queue);
        pthread_cancel(*thread);
        free(thread);
        free(msg);
        goto RESULT;
    }

    int *msg3 = malloc(sizeof(int));
    *msg3 = 20;
    addMsg(queue, msg3);

    success = getAvailable(queue, *thread) == 1
            && *msg3 == *(int*)getMsg(queue, *thread)
            && getAvailable(queue, *thread) == 0;

    destroyQueue(queue);
    free(msg);
    free(msg3);
    pthread_cancel(*thread);
    free(thread);

RESULT:
    printf("singleSubscriberMultipleMessagesSequentialReceivedTest: %s\n", success ? "success" : "failed");
    return NULL;
}

void *singleSubscriberMultipleMessagesSingleReceivedTest(void *args) {
    TQueue *queue = createQueue(10);

    pthread_t* thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, threadBusyWait, NULL);
    subscribe(queue, *thread);

    int *msg = malloc(sizeof(int));
    *msg = 10;
    addMsg(queue, msg);

    const int available = getAvailable(queue,*thread);

    int *msg2 = malloc(sizeof(int));
    *msg2 = 20;
    addMsg(queue, msg2);

    const int available2 = getAvailable(queue, *thread);
    const int* msg3 = getMsg(queue, *thread);
    const int available3 = getAvailable(queue, *thread);
    const int* msg4 = getMsg(queue, *thread);
    const int available4 = getAvailable(queue, *thread);

    bool success = available == 1
                && available2 == 2
                && available3 == 1
                && available4 == 0
                && *msg3 == 10
                && *msg4 == 20;

    destroyQueue(queue);
    free(msg);
    free(msg2);
    pthread_cancel(*thread);
    free(thread);

    printf("singleSubscriberMultipleMessagesSingleReceivedTest: %s\n", success ? "success" : "failed");
    return NULL;
}

void *multipleSubscribersSingleMessageTest(void *args) {
    int size = 10;
    TQueue *queue =createQueue(size);

    pthread_t* thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, threadBusyWait, NULL);
    subscribe(queue, *thread);

    int *msg = malloc(sizeof(int));
    *msg = 10;
    addMsg(queue, msg);

    bool success = getAvailable(queue, *thread) == 1;
    if (!success) {
        destroyQueue(queue);
        free(msg);
        pthread_cancel(*thread);
        free(thread);

       goto RESULT;
    }

    pthread_t* thread2 = malloc(sizeof(pthread_t));
    pthread_create(thread2, NULL, threadBusyWait, NULL);
    subscribe(queue, *thread2);

    success = getAvailable(queue, *thread2) == 0;
    if (!success) {
        destroyQueue(queue);
        free(msg);

        pthread_cancel(*thread2);
        free(thread2);

        pthread_cancel(*thread);
        free(thread);

        goto RESULT;
    }

    getMsg(queue, *thread);

    success = getAvailable(queue, *thread) == 0;
    if (!success) {
        destroyQueue(queue);
        free(msg);
        pthread_cancel(*thread2);
        free(thread2);
        pthread_cancel(*thread);
        free(thread);

        goto RESULT;
    }

    char* msg2 = malloc(sizeof(char) * 10);
    strcpy(msg2, "hello");
    addMsg(queue, msg2);

    success = getAvailable(queue, *thread) == 1
            && getAvailable(queue, *thread2) == 1
            && !strcmp(getMsg(queue, *thread), "hello")
            && !strcmp(getMsg(queue, *thread2), "hello");

    destroyQueue(queue);
    free(msg);
    free(msg2);

    pthread_cancel(*thread2);
    free(thread2);

    pthread_cancel(*thread);
    free(thread);

RESULT:
    printf("multipleSubscribersSingleMessageTest: %s\n", success ? "success" : "failed");

    return NULL;
}

void *multipleSubscribersMultipleMessagesTest(void *args) {
    TQueue *queue = createQueue(10);

    pthread_t* thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, threadBusyWait, NULL);
    subscribe(queue, *thread);

    int *msg = malloc(sizeof(int));
    *msg = 10;
    addMsg(queue, msg);

    bool success = getAvailable(queue, *thread) == 1;
    if (!success) {
        destroyQueue(queue);
        free(msg);
        pthread_cancel(*thread);
        free(thread);

        goto RESULT;
    }

    pthread_t* thread2 = malloc(sizeof(pthread_t));
    pthread_create(thread2, NULL, threadBusyWait, NULL);
    subscribe(queue, *thread2);

    success = getAvailable(queue, *thread2) == 0
            && getAvailable(queue, *thread) == 1;

    if (!success) {
        destroyQueue(queue);
        free(msg);

        pthread_cancel(*thread2);
        free(thread2);

        pthread_cancel(*thread);
        free(thread);

        goto RESULT;
    }

    char* msg2 = malloc(sizeof(char) * 10);
    strcpy(msg2, "hello");
    addMsg(queue, msg2);

    success = getAvailable(queue, *thread) == 2
            && getAvailable(queue, *thread2) == 1;

    if (!success) {
        destroyQueue(queue);
        free(msg);

        pthread_cancel(*thread2);
        free(thread2);

        pthread_cancel(*thread);
        free(thread);
        free(msg2);

        goto RESULT;
    }

    success = *(int*)getMsg(queue, *thread) == 10
            && !strcmp(getMsg(queue, *thread), "hello")
            && !strcmp(getMsg(queue, *thread2), "hello");

    destroyQueue(queue);
    free(msg);
    free(msg2);

    pthread_cancel(*thread2);
    free(thread2);

    pthread_cancel(*thread);
    free(thread);

 RESULT:
    printf("multipleSubscribersMultipleMessagesTest: %s\n", success ? "success" : "failed");
    return NULL;
}

void *changeSizeTest(void *args) {
    TQueue *queue = createQueue(10);
    pthread_t** threads = malloc(sizeof(pthread_t*) * 10);

    for (int i = 0; i < 10; i++) {
        threads[i] = malloc(sizeof(pthread_t));
        pthread_create(threads[i], NULL, threadBusyWait, NULL);
        subscribe(queue, *threads[i]);

        int *msg = malloc(sizeof(int));
        *msg = i * 10;
        addMsg(queue, msg);
    }

    bool success = true;
    setSize(queue, 5);
    for (int i = 0; i < 10; i++) {
        const int available = getAvailable(queue, *threads[i]);
        success = (i < 5 && available == 5)
                || (i >= 5 && available == 10 - i);
        if (!success) break;
    }

    destroyQueue(queue);
    for (int j = 0; j < 10; j++) {
        pthread_cancel(*threads[j]);
        free(threads[j]);
    }
    free(threads);

    printf("changeSizeTest: %s\n", success ? "success" : "failed");
    return NULL;
}

void *sender(void *args) {
    for (int i = 0; i < 10; i++) {
        usleep(30 * 1000);
        char* msg = malloc(sizeof(char) * 10);
        snprintf(msg, 10, "%d", i);
        addMsg(args, msg);
    }

    return NULL;
}

void *subscriber(void *args) {
    subscribe(args, pthread_self());

    char* result = malloc(sizeof(char) * 11);
    for (int i = 0; i < 10; i++) {
        result[i] = 0;
    }

    for (int i = 0; i < 10; i++) {
        char* msg = getMsg(args, pthread_self());
        strcat(result, msg);
    }
    strcat(result, "\0");

    return result;
}

void *parallelSenderSubscriberTest(void *args) {

    TQueue *queue = createQueue(10);

    pthread_t* subscriberThread = malloc(sizeof(pthread_t));
    pthread_create(subscriberThread, NULL, subscriber, queue);

    pthread_t* senderThread = malloc(sizeof(pthread_t));
    pthread_create(senderThread, NULL, sender, queue);

    char *result;
    pthread_join(*subscriberThread, (void**)&result);

    bool success = !strcmp(result, "0123456789");

    destroyQueue(queue);
    free(senderThread);
    free(subscriberThread);

    printf("parallelSenderSubscriberTest: %s\n", success ? "success" : "failed");
    return NULL;
}

typedef struct {
    int time;
    TQueue *queue;
} TimeAndQueueArgs;

void *subscriberDelayed(void *args) {
    TimeAndQueueArgs *threadArgs = args;
    subscribe(threadArgs->queue, pthread_self());

    char* result = malloc(sizeof(char) * 11);
    for (int i = 0; i < 10; i++)
        result[i] = 0;


    for (int i = 0; i < 10; i++) {
        usleep(threadArgs->time);
        char* msg = getMsg(threadArgs->queue, pthread_self());
        strcat(result, msg);
    }

    strcat(result, "\0");

    return result;
}

void *parallelSenderSubscribersTest(void *args) {
    TQueue *queue = createQueue(10);

    pthread_t* subscriberThread0 = malloc(sizeof(pthread_t));
    TimeAndQueueArgs *args0 = malloc(sizeof(TimeAndQueueArgs));
    args0->time = 100 * 1000;
    args0->queue = queue;
    pthread_create(subscriberThread0, NULL, subscriberDelayed, args0);

    pthread_t* subscriberThread1 = malloc(sizeof(pthread_t));
    TimeAndQueueArgs *args1 = malloc(sizeof(TimeAndQueueArgs));
    args1->time = 150 * 1000;
    args1->queue = queue;
    pthread_create(subscriberThread1, NULL, subscriberDelayed, args1);

    pthread_t* senderThread = malloc(sizeof(pthread_t));
    pthread_create(senderThread, NULL, sender, queue);

    char *result0;
    pthread_join(*subscriberThread0, (void**)&result0);

    char *result1;
    pthread_join(*subscriberThread1, (void**)&result1);

    bool success = !strcmp(result0, "0123456789")
                && !strcmp(result1, "0123456789");

    destroyQueue(queue);
    free(senderThread);
    free(subscriberThread0);
    free(subscriberThread1);
    free(args0);
    free(args1);

    printf("parallelSenderSubscribersTest: %s\n", success ? "success" : "failed");
    return NULL;
}

typedef struct {
    pthread_mutex_t* mutex;
    TQueue *queue;
} SenderArgs;

const unsigned long int threshold = 0xFFFF;

typedef struct {
    long double arr[1000][1000][100];
} BigMemoryChunk;

void *senderBigMemory(void *args) {
    SenderArgs *senderArgs = args;
    for (int i = 0; i < 5; i++) pthread_mutex_lock(&senderArgs->mutex[i]);

    for (unsigned long int i = 0; i < threshold; i++)
        addMsg(senderArgs->queue, malloc(sizeof(BigMemoryChunk)));

    return NULL;
}

typedef struct {
    pthread_mutex_t* mutex;
    TQueue *queue;
} SubscriberArgs;

void *subscriberBigMemory(void *args) {
    SubscriberArgs *subscriberArgs = args;
    subscribe(subscriberArgs->queue, pthread_self());
    pthread_mutex_unlock(subscriberArgs->mutex);

    for (unsigned long int i = 0; i < threshold; i++)
        getMsg(subscriberArgs->queue, pthread_self());

    unsubscribe(subscriberArgs->queue, pthread_self());

    return NULL;
}

void *garbageCollector(void *args) {
    SubscriberArgs *subscriberArgs = args;
    subscribe(subscriberArgs->queue, pthread_self());
    pthread_mutex_unlock(subscriberArgs->mutex);

    for (unsigned long int i = 0; i < threshold; i++)
        free(getMsg(subscriberArgs->queue, pthread_self()));

    unsubscribe(subscriberArgs->queue, pthread_self());

    return NULL;
}

void *memoryLeakTest(void *args) {
    pthread_mutex_t mutex[5];
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&mutex[i], NULL);
        pthread_mutex_lock(&mutex[i]);
    }

    TQueue *queue = createQueue(100000);

    SubscriberArgs *subscriberArgs = malloc(sizeof(SubscriberArgs));
    subscriberArgs->queue = queue;
    subscriberArgs->mutex = &mutex[0];

    pthread_t* subscriberThread = malloc(sizeof(pthread_t));
    bool success = !pthread_create(subscriberThread, NULL, subscriberBigMemory, subscriberArgs);
    if (!success) {
        pthread_cancel(*subscriberThread);

        free(subscriberThread);

        destroyQueue(queue);

        goto RESULT;
    }

    SubscriberArgs *subscriber0Args = malloc(sizeof(SubscriberArgs));
    subscriber0Args->queue = queue;
    subscriber0Args->mutex = &mutex[1];

    pthread_t* subscriberThread0 = malloc(sizeof(pthread_t));
    success = !pthread_create(subscriberThread0, NULL, subscriberBigMemory, subscriber0Args);
    if (!success) {
        pthread_cancel(*subscriberThread);

        free(subscriberThread);
        free(subscriberThread0);

        destroyQueue(queue);

        goto RESULT;
    }

    SubscriberArgs *subscriber1Args = malloc(sizeof(SubscriberArgs));
    subscriber1Args->queue = queue;
    subscriber1Args->mutex = &mutex[2];

    pthread_t* subscriberThread1 = malloc(sizeof(pthread_t));
    success = !pthread_create(subscriberThread1, NULL, subscriberBigMemory, subscriber1Args);
    if (!success) {
        pthread_cancel(*subscriberThread);
        pthread_cancel(*subscriberThread0);

        free(subscriberThread);
        free(subscriberThread0);
        free(subscriberThread1);

        destroyQueue(queue);

        goto RESULT;
    }

    SubscriberArgs *subscriber2Args = malloc(sizeof(SubscriberArgs));
    subscriber2Args->queue = queue;
    subscriber2Args->mutex = &mutex[3];

    pthread_t* subscriberThread2 = malloc(sizeof(pthread_t));
    success = !pthread_create(subscriberThread2, NULL, subscriberBigMemory, subscriber2Args);
    if (!success) {
        pthread_cancel(*subscriberThread);
        pthread_cancel(*subscriberThread0);
        pthread_cancel(*subscriberThread1);

        free(subscriberThread);
        free(subscriberThread0);
        free(subscriberThread1);
        free(subscriberThread2);

        destroyQueue(queue);

        goto RESULT;
    }

    SubscriberArgs *subscriber3Args = malloc(sizeof(SubscriberArgs));
    subscriber3Args->queue = queue;
    subscriber3Args->mutex = &mutex[4];

    pthread_t* garbageCollectorThread = malloc(sizeof(pthread_t));
    success = !pthread_create(garbageCollectorThread, NULL, garbageCollector, subscriber3Args);
    if (!success) {
        pthread_cancel(*subscriberThread);
        pthread_cancel(*subscriberThread0);
        pthread_cancel(*subscriberThread1);
        pthread_cancel(*subscriberThread2);

        free(subscriberThread);
        free(subscriberThread0);
        free(subscriberThread1);
        free(subscriberThread2);
        free(garbageCollectorThread);

        destroyQueue(queue);

        goto RESULT;
    }

    SenderArgs *senderArgs = malloc(sizeof(SenderArgs));
    senderArgs->queue = queue;
    senderArgs->mutex = mutex;
    pthread_t* senderThread = malloc(sizeof(pthread_t));

    success = !pthread_create(senderThread, NULL, senderBigMemory, senderArgs);
    if (!success) {
        pthread_cancel(*subscriberThread);
        pthread_cancel(*subscriberThread0);
        pthread_cancel(*subscriberThread1);
        pthread_cancel(*subscriberThread2);
        pthread_cancel(*garbageCollectorThread);

        free(senderThread);
        free(subscriberThread);
        free(subscriberThread0);
        free(subscriberThread1);
        free(subscriberThread2);
        free(garbageCollectorThread);

        destroyQueue(queue);

        goto RESULT;
    }

    pthread_join(*senderThread, NULL);

    pthread_join(*subscriberThread, NULL);

    pthread_join(*subscriberThread0, NULL);

    pthread_join(*subscriberThread1, NULL);

    pthread_join(*subscriberThread2, NULL);

    pthread_join(*garbageCollectorThread, NULL);

    destroyQueue(queue);

    for (int i = 0; i < 5; i++)
        pthread_mutex_destroy(&mutex[i]);

    free(senderArgs);
    free(subscriberArgs);
    free(subscriber0Args);
    free(subscriber1Args);
    free(subscriber2Args);
    free(subscriber3Args);
    
    free(senderThread);
    free(subscriberThread);
    free(subscriberThread0);
    free(subscriberThread1);
    free(subscriberThread2);
    free(garbageCollectorThread);

RESULT:
    printf("memoryLeakTest: %s\n", success ? "success" : "failed");
    return NULL;
}

void *unsubscribeTest(void *args) {
    pthread_t* thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, threadBusyWait, NULL);

    TQueue* queue = createQueue(10);
    subscribe(queue, *thread);

    int *msg = malloc(sizeof(int));
    *msg = 10;
    addMsg(queue, msg);

    bool success = getAvailable(queue, *thread) == 1;
    if (!success) {
        free(msg);
        destroyQueue(queue);
        pthread_cancel(*thread);
        free(thread);

        goto RESULT;
    }

    int *msg2 = malloc(sizeof(int));
    *msg2 = 20;
    addMsg(queue, msg2);

    unsubscribe(queue, *thread);
    success = getAvailable(queue, *thread) == 0;

    destroyQueue(queue);

    pthread_cancel(*thread);
    free(thread);
    free(msg);
    free(msg2);

RESULT:
    printf("unsubscribeTest: %s\n", success ? "success" : "failed");
    return NULL;
}

void *destroyFullQueueTest(void *args) {
    TQueue *queue = createQueue(10);
    pthread_t* threads = malloc(sizeof(pthread_t)*20);
    for (int i = 0; i < 20; ++i) {
        pthread_create(&threads[i], NULL, threadBusyWait, NULL);
        subscribe(queue, threads[i]);
    }

    int *msg = malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i) {
        msg[i] = i;
        addMsg(queue, &msg[i]);
    }
    destroyQueue(queue);
    free(msg);
    for (int i = 0; i < 20; ++i)
        pthread_cancel(threads[i]);

    free(threads);

    printf("destroyFullQueueTest: success\n");

    return NULL;
}

int main() {
    pthread_t t0;
    pthread_create(&t0, NULL, initTest, NULL);

    pthread_t t1;
    pthread_create(&t1, NULL, singleSubscriberMultipleMessagesSequentialReceivedTest, NULL);

    pthread_t t2;
    pthread_create(&t2, NULL, singleSubscriberMultipleMessagesSingleReceivedTest, NULL);

    pthread_t t3;
    pthread_create(&t3, NULL, multipleSubscribersSingleMessageTest, NULL);

    pthread_t t4;
    pthread_create(&t4, NULL, multipleSubscribersMultipleMessagesTest, NULL);

    pthread_t t5;
    pthread_create(&t5, NULL, changeSizeTest, NULL);

    pthread_t t6;
    pthread_create(&t6, NULL, parallelSenderSubscriberTest, NULL);

    pthread_t t7;
    pthread_create(&t7, NULL, parallelSenderSubscribersTest, NULL);

    pthread_t t8;
    pthread_create(&t8, NULL, memoryLeakTest, NULL);

    pthread_t t9;
    pthread_create(&t9, NULL, unsubscribeTest, NULL);

    pthread_t t10;
    pthread_create(&t10, NULL, destroyFullQueueTest, NULL);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);
    pthread_join(t7, NULL);
    pthread_join(t8, NULL);
    pthread_join(t9, NULL);
    pthread_join(t10, NULL);
    return 0;
}
