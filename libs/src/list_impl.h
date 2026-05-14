#ifndef LIST_IMPL_H
#define LIST_IMPL_H

#include "../include/list.h"

#define IMPLEMENT_LIST(T, ArrayList)        \
    int ArrayList##_init(ArrayList* l) {    \
        l->size = 0;                        \
        l->capacity = DEFAULT_CAPACITY;     \
        l->data = malloc(l->capacity * sizeof(T));  \
        return l->data ? 0 : -1;            \
    }                                       \
    ArrayList* ArrayList##_new() {          \
        ArrayList *l = malloc(sizeof(ArrayList));   \
        if (!l) return NULL;                \
        if (ArrayList##_init(l) != 0) {     \
            free(l);                        \
            return NULL;                    \
        }                                   \
        return l;                           \
    }                                       \
    int ArrayList##_add(ArrayList* l, T val) {      \
        if (l->size == l->capacity) {       \
            if (l->capacity > SIZE_MAX / 2) \
                return -1;                  \
            size_t new_capacity = l->capacity << 1;             \
            if (new_capacity > SIZE_MAX / sizeof(T))            \
                return -1;                                      \
            T* tmp = realloc(l->data, new_capacity * sizeof(T));\
            if (!tmp)                       \
                return -1;                  \
            l->data = tmp;                  \
            l->capacity = new_capacity;     \
        }                                   \
        l->data[l->size++] = val;           \
        return 0;                           \
    }                                       \
    void ArrayList##_free(ArrayList* l) {   \
        if (l) {                            \
            free(l->data);                  \
            free(l);                        \
        }                                   \
    }                                       \
    void ArrayList##_clear(ArrayList* l) {  \
        if (l && l->data) {                 \
            free(l->data);                  \
            l->data = NULL;                 \
            l->size = l->capacity = 0;      \
        }                                   \
    }

#endif
