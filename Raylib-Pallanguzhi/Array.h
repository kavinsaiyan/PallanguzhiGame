#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
    int* arr;
    int len;
} Array;

Array* CreateArray(int len);

void DestoryArray(Array* array);

#endif

