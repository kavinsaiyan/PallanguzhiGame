#ifndef QUEUE_H
#define QUEUE_H

extern const int INITIAL_MAX_QUEUE_SIZE;

typedef struct {
    int size;
    int count;
    int* arr;
} Queue;

Queue* CreateQueue();

void Enqueue(Queue* q, int element);

void EnqueueArray(Queue* q, int* arr, int len);

int Dequeue(Queue* q);

void DestroyQueue(Queue* q);

#endif
