#include "Array.h"
#include "raylib.h"
#include <stdlib.h>

Array* CreateArray(int len)
{
    Array* arr = (Array*)malloc(sizeof(Array));
    if(arr == NULL)
    {
        TraceLog(LOG_ERROR, "[Array.c/CreateArray]: malloc failed for Array struct");
        return NULL;
    }
    arr->len = len;
    arr->arr = (int*)malloc(sizeof(int)*len);
    if(arr->arr == NULL)
    {
        TraceLog(LOG_ERROR, "[Array.c/CreateArray]: malloc failed for array inside Array struct");
        return NULL;
    }
    return arr;
}
