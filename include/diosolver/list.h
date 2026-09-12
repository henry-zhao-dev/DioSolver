/**
 * "list.h" provides a generic list implementation.
 */

#ifndef DIOSOLVER_LIST_H
#define DIOSOLVER_LIST_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Initializes an empty list. */
#define list_init_empty() \
    make_list(malloc(0), 0)

/** Initializes a list of a specific size. */
#define list_init_size(size, type) \
    make_list(malloc(size * sizeof(type)), size)

/** Accesses an element in the list at a specific index. */
#define list_at(lst, index, type) \
    ((type*) lst.arr)[index]

/** Sets an element in the list at a specific index. */
#define list_set(lst, index, value, type) \
    ((type*) lst.arr)[index] = value

/** Appends an element to the list. */
#define list_append(lst, x, type) \
    lst.arr = (type*) realloc(lst.arr, (lst.size + 1) * sizeof(type)); \
    ((type*) lst.arr)[lst.size++] = x

/** Free a list from the memory. */
#define list_free(lst) \
    free(lst.arr); \
    lst.size = 0;

/**
 * Represents a generic list.
 */
typedef struct List {
    void *arr;  // Pointer to the array
    int size;   // Number of elements in the list
} List;

/**
 * Creates a list with specified array and size.
 */
List make_list(void *arr, int size);

#ifdef __cplusplus
}
#endif

#endif
