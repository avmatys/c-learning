#ifndef MY_CYCLIC_BUFFER
#define MY_CYCLIC_BUFFER

typedef struct {
    char** data;
    int head;
    int tail;
    int count;
    int capacity;
} CyclicBuffer;

int init_buffer(CyclicBuffer *cb, int capacity);
void free_buffer(CyclicBuffer *cb);
int push_buffer(CyclicBuffer *cb, const char* str);
int poll_buffer(CyclicBuffer *cb, char** str); 

#endif
