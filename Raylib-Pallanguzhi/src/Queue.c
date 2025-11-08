#include "raylib.h"
#include "Queue.h"
#include <stdlib.h>

const int INITIAL_MAX_QUEUE_SIZE = 50;

Queue* CreateQueue()
{
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if(q == NULL)
    {
        TraceLog(LOG_ERROR, "[Queue.c/CreateQueue]: malloc failed for Queue!");
        return NULL;
    }
    q->count=0;
    q->arr = (int*)malloc(sizeof(int)*INITIAL_MAX_QUEUE_SIZE);
    if(q->arr == NULL)
    {
        TraceLog(LOG_ERROR, "[Queue.c/CreateQueue]: malloc failed for Array!");
        return NULL;
    }
    q->size = INITIAL_MAX_QUEUE_SIZE;
    return q;
}

void Enqueue(Queue* q, int element)
{
    if(q->count < q->size)
    {
        q->arr[q->count] = element;
        q->count++;
    }
    else
        TraceLog(LOG_ERROR, "[Queue.c/Enqueue]:Queue size exceeded need resize");
}

void EnqueueArray(Queue* q, int* arr, int len)
{
    for(int i = 0; i < len; i++)
    {
        Enqueue(q, arr[i]);
    }
}

int Dequeue(Queue* q)
{
    int res = 0;
    if(q->count < q->size && q->count > 0)
    {
        res = q->arr[0];
        for(int i = 1; i < q->count; i++)
            q->arr[i-1] = q->arr[i];
        q->count--;
    }
    else
        TraceLog(LOG_ERROR, "[Queue.c/Dequeue]:Queue size exceeded need resize");
    return res;
}

void DestroyQueue(Queue *q)
{
    if(q == NULL)
    {
        TraceLog(LOG_ERROR, "[Queue.c/Free]: Null is passed in!");
        return;
    }
    free(q->arr);
    free(q);
}
