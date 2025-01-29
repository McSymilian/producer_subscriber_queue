#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include "queue.h"

typedef struct SubscriberNode {
    pthread_t data;
    struct SubscriberNode *prev;
    struct SubscriberNode *next;
} SubscriberNode;

typedef struct SubscribersList {
    SubscriberNode *head;
    SubscriberNode *tail;
    int size;
} SubscribersList;

SubscriberNode* createSubscriberNode(pthread_t subscriber) {
    SubscriberNode *newNode = malloc(sizeof(SubscriberNode));
    if (!newNode)
        exit(EXIT_FAILURE);

    newNode->data = subscriber;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

SubscribersList* createSubscribersList() {
    SubscribersList *list = malloc(sizeof(SubscribersList));
    if (!list)
        exit(EXIT_FAILURE);

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void appendSubscriber(SubscribersList *list, pthread_t subscriber) {
    SubscriberNode *newNode = createSubscriberNode(subscriber);
    if (list->tail) {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    } else {
        list->head = newNode;
        list->tail = newNode;
    }

    list->size++;
}

SubscriberNode* findSubscriberBidirectional(SubscribersList *list, pthread_t subscriber) {
    if (!list || list->size == 0) return NULL;

    SubscriberNode *forwardNode = list->head;
    SubscriberNode *backwardNode = list->tail;

    while (forwardNode && backwardNode && forwardNode != backwardNode && forwardNode->prev != backwardNode) {
        if (pthread_equal(forwardNode->data, subscriber))
            return forwardNode;

        if (pthread_equal(backwardNode->data, subscriber))
            return backwardNode;

        forwardNode = forwardNode->next;
        backwardNode = backwardNode->prev;
    }

    if (forwardNode && pthread_equal(forwardNode->data, subscriber))
        return forwardNode;

    return NULL;
}

void removeSubscriber(SubscribersList *list, pthread_t subscriber) {
    if (!subscriber) return;
    SubscriberNode *node = findSubscriberBidirectional(list, subscriber);
    if (!node) return;

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    free(node);
    list->size--;
}

void destroySubscribersList(SubscribersList *list) {
    SubscriberNode *current = list->head;
    while (current) {
        SubscriberNode *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void copySubscribersList(SubscribersList *from, SubscribersList *to) {
    if (!from || !to) return;

    SubscriberNode *current = from->head;
    while (current) {
        appendSubscriber(to, current->data);
        current = current->next;
    }
}

bool isSubscriber(SubscribersList *list, pthread_t subscriber) {
    if (!list) return false;

    return findSubscriberBidirectional(list, subscriber) != NULL;
}

typedef struct MessageNode {
    void *message;
    SubscribersList *subscribers;
    struct MessageNode *prev;
    struct MessageNode *next;
} MessageNode;

typedef struct MessagesList {
    MessageNode *head;
    MessageNode *tail;
    int size;
} MessagesList;

MessageNode* createMessageNode(void *message, SubscribersList *subscribers) {
    MessageNode *newNode = malloc(sizeof(MessageNode));
    if (!newNode)
        exit(EXIT_FAILURE);

    newNode->message = message;
    newNode->subscribers = createSubscribersList();
    copySubscribersList(subscribers, newNode->subscribers);
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

MessagesList* createMessagesList() {
    MessagesList *list = malloc(sizeof(MessagesList));
    if (!list) {
        perror("Failed to create list");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void appendMessage(MessagesList *list, void *message, SubscribersList *subscribers) {
    MessageNode *newNode = createMessageNode(message, subscribers);
    if (list->tail) {
        list->tail->next = newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    } else {
        list->head = newNode;
        list->tail = newNode;
    }

    list->size++;
}

void removeMessageNode(MessagesList *list, MessageNode *node) {
    if (!node) return;

    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    free(node->subscribers);
    free(node);

    list->size--;
}

MessageNode* findMessageBidirectional(MessagesList *list, const void *message) {
    if (!list || list->size == 0) return NULL;

    MessageNode *forwardNode = list->head;
    MessageNode *backwardNode = list->tail;

    while (
        forwardNode && backwardNode
        && forwardNode != backwardNode
        && forwardNode->prev != backwardNode
    ) {
        if (forwardNode->message == message) {
            return forwardNode;
        }
        if (backwardNode->message == message) {
            return backwardNode;
        }
        forwardNode = forwardNode->next;
        backwardNode = backwardNode->prev;
    }

    if (forwardNode && forwardNode->message == message)
        return forwardNode;

    return NULL;
}

void removeMessage(MessagesList *list, void *message) {
    if (!message) return;

    MessageNode *node = findMessageBidirectional(list, message);
    if (!node) return;

    removeMessageNode(list, node);
}

void destroyMessagesList(MessagesList *list) {
    MessageNode *current = list->head;
    while (current) {
        MessageNode *next = current->next;
        destroySubscribersList(current->subscribers);
        free(current);
        current = next;
    }

    free(list);
}

extern TQueue* createQueue(int size) {
    TQueue *queue = malloc(sizeof(TQueue));
    queue->maxSize = size;

    queue->allSubscribers = createSubscribersList();
    queue->messages = createMessagesList();

    pthread_mutex_init(&queue->operational_mutex, NULL);
    pthread_cond_init(&queue->msg_posted_cond, NULL);
    pthread_cond_init(&queue->msg_fully_received_cond, NULL);

    return queue;
}

extern void destroyQueue(TQueue *queue) {
    pthread_mutex_lock(&queue->operational_mutex);

    destroySubscribersList(queue->allSubscribers);
    destroyMessagesList(queue->messages);

    pthread_cond_destroy(&queue->msg_posted_cond);
    pthread_cond_destroy(&queue->msg_fully_received_cond);

    pthread_mutex_unlock(&queue->operational_mutex);
    pthread_mutex_destroy(&queue->operational_mutex);
    free(queue);
}

extern void subscribe(TQueue *queue, pthread_t thread) {
    pthread_mutex_lock(&queue->operational_mutex);
    appendSubscriber(queue->allSubscribers, thread);
    pthread_mutex_unlock(&queue->operational_mutex);
}

extern void unsubscribe(TQueue *queue, pthread_t thread) {
    pthread_mutex_lock(&queue->operational_mutex);
    removeSubscriber(queue->allSubscribers, thread);

    MessageNode *msgNode = queue->messages->head;
    while (msgNode) {
        MessageNode *nextNode = msgNode->next;
        if (isSubscriber(msgNode->subscribers, thread)) {
            removeSubscriber(msgNode->subscribers, thread);

            if (msgNode->subscribers->size == 0) {
                removeMessageNode(queue->messages, msgNode);
                pthread_cond_signal(&queue->msg_fully_received_cond);
            }
        }
        msgNode = nextNode;
    }

    pthread_mutex_unlock(&queue->operational_mutex);
}

extern void addMsg(TQueue *queue, void *msg) {
    pthread_mutex_lock(&queue->operational_mutex);
    if (queue->messages->size >= queue->maxSize)
        pthread_cond_wait(&queue->msg_fully_received_cond, &queue->operational_mutex);

    appendMessage(queue->messages, msg, queue->allSubscribers);
    pthread_cond_broadcast(&queue->msg_posted_cond);
    pthread_mutex_unlock(&queue->operational_mutex);
}

extern void *getMsg(TQueue *queue, pthread_t thread) {
    pthread_mutex_lock(&queue->operational_mutex);
    RETRY:
    if (!isSubscriber(queue->allSubscribers, thread) || queue->maxSize <= 0) {
        pthread_mutex_unlock(&queue->operational_mutex);
        return NULL;
    }

    MessageNode *msgNode = queue->messages->head;
    while (msgNode) {
        if (isSubscriber(msgNode->subscribers, thread)) {
            removeSubscriber(msgNode->subscribers, thread);
            void *msg = msgNode->message;
            if (msgNode->subscribers->size == 0) {
                removeMessageNode(queue->messages, msgNode);
                pthread_cond_signal(&queue->msg_fully_received_cond);
            }
            pthread_mutex_unlock(&queue->operational_mutex);
            return msg;
        }
        msgNode = msgNode->next;
    }

    pthread_cond_wait(&queue->msg_posted_cond, &queue->operational_mutex);
    goto RETRY;
}

extern int getAvailable(TQueue *queue, pthread_t thread) {
    pthread_mutex_lock(&queue->operational_mutex);
    int sum = 0;
    MessageNode *msgNode = queue->messages->head;
    while (msgNode) {
        if (isSubscriber(msgNode->subscribers, thread)) {
            sum++;
        }
        msgNode = msgNode->next;
    }

    pthread_mutex_unlock(&queue->operational_mutex);
    return sum;
}

extern void removeMsg(TQueue *queue, void *msg) {
    pthread_mutex_lock(&queue->operational_mutex);
    removeMessage(queue->messages, msg);
    pthread_mutex_unlock(&queue->operational_mutex);
    pthread_cond_broadcast(&queue->msg_fully_received_cond);
}

extern void setSize(TQueue *queue, int size) {
    pthread_mutex_lock(&queue->operational_mutex);

    if (queue->messages->size <= size) {
        queue->maxSize = size;
        pthread_mutex_unlock(&queue->operational_mutex);
        pthread_cond_broadcast(&queue->msg_fully_received_cond);
        return;
    }

    int diff = queue->messages->size - size;
    MessageNode *msgNode = queue->messages->head;
    for (int i = 0; i < diff; i++) {
        MessageNode *nextNode = msgNode->next;
        removeMessageNode(queue->messages, msgNode);
        msgNode = nextNode;
    }
    queue->maxSize = size;

    pthread_mutex_unlock(&queue->operational_mutex);
}
