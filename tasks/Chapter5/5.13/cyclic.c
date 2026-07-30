#include "cyclic.h"
#include <string.h>
#include <stdlib.h>

int init_buffer(CyclicBuffer *cb, int capacity)
{
    if (!cb || capacity <= 0) return -1;
    cb->data = malloc(capacity * sizeof(char*));
    if (!cb->data) return -1;
    cb->capacity = capacity;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    return 0;
}

void free_buffer(CyclicBuffer *cb)
{
    if (!cb) return;
    if (cb->data) {
        char *str;
        while(poll_buffer(cb, &str) == 0) {
            free(str);
        }
        free(cb->data);
        cb->data = NULL;
    }
}

int push_buffer(CyclicBuffer *cb, const char* str)
{
   if (!cb) return -1;
   char *copy = strdup(str);
   if (!copy) return -1;
   if (cb->count == cb->capacity) {
       free(cb->data[cb->head]);
       cb->head = (cb->head + 1) % cb->capacity;
   }
   else {
       cb->count++;
   }
   cb->data[cb->tail] = copy;
   cb->tail = (cb->tail + 1) % cb->capacity;
   return 0;
}

int poll_buffer(CyclicBuffer *cb, char** str)
{
    if (!cb || cb->count == 0) return -1;
    *str = cb->data[cb->head];
    cb->head = (cb->head + 1) % cb->capacity;
    cb->count--;
    return 0;
}
