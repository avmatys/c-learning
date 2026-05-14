#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_CAPACITY 32

#define DECLARE_LIST(T, ArrayList)      \
    typedef struct {                    \
        T* data;                        \
        size_t size;                    \
        size_t capacity;                \
    } ArrayList;                        \
                                        \
    int ArrayList##_init(ArrayList* l); \
    ArrayList* ArrayList##_new();       \
    int ArrayList##_add(ArrayList* l, T val);   \
    void ArrayList##_free(ArrayList* l);        \
    void ArrayList##_clear(ArrayList* l);       

#endif
