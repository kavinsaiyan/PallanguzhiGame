#include "raylib.h"
#include "Queue.h"

const int INITIAL_MAX_QUEUE_SIZE = 50;

Queue CreateQueue()
{
    Queue q;
    q.count=0;
    int arr[INITIAL_MAX_QUEUE_SIZE];
    q.arr = arr;
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
        TraceLog(LOG_ERROR, "Queue size exceeded need resize");
}

int Dequeue(Queue* q, int element)
{
    int res;
    if(q->count < q->size && q->count > 0)
    {
        res = q->arr[0];
        for(int i = 1; i < q->count; i++)
            q->arr[i-1] = q->arr[i];
        q->count--;
    }
    else
        TraceLog(LOG_ERROR, "Queue size exceeded need resize");
    return res;
}
