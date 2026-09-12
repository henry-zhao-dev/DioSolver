/**
 * @file list.h
 * @brief A small, type-agnostic dynamic-array list.
 *
 * `List` stores a contiguous array through a `void *`. The macros in this
 * header require the caller to provide the element type when reading or
 * writing the array. They do not perform bounds or type checking.
 */

#ifndef DIOSOLVER_LIST_H
#define DIOSOLVER_LIST_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes an empty list.
 *
 * The returned list has size zero and owns an empty dynamically allocated
 * array suitable for `list_append()`.
 */
#define list_init_empty() \
    make_list(malloc(0), 0)

/**
 * Initializes a list with storage for `size` elements of `type`.
 *
 * The elements are uninitialized.  `size` should be non-negative, and the
 * resulting array is owned by the list.
 */
#define list_init_size(size, type) \
    make_list(malloc(size * sizeof(type)), size)

/**
 * Accesses an element at `index` as `type`.
 *
 * No bounds or type checking is performed.  `index` must be in `[0, size)`
 * and `type` must match the stored element type.
 */
#define list_at(lst, index, type) \
    ((type*) lst.arr)[index]

/**
 * Assigns `value` to the element at `index` as `type`.
 *
 * No bounds or type checking is performed.  `index` must be in `[0, size)`
 * and `type` must match the stored element type.
 */
#define list_set(lst, index, value, type) \
    ((type*) lst.arr)[index] = value

/**
 * Appends one element to a list, growing its dynamically allocated array.
 *
 * `type` must match the stored element type.  If reallocation fails, the
 * implementation calls `abort()`; no recoverable error value is provided.
 */
#define list_append(lst, x, type) \
    do { \
        type *new_arr = (type*) realloc( \
            (lst).arr, ((lst).size + 1) * sizeof(type)); \
        if (new_arr == NULL) { \
            abort(); \
        } \
        (lst).arr = new_arr; \
        new_arr[(lst).size++] = (x); \
    } while (0)

/**
 * Releases a list's array and sets its size to zero.
 *
 * The macro does not set `lst.arr` to `NULL`; do not use the list again or
 * call `list_free()` on it a second time unless it has been reinitialized.
 */
#define list_free(lst) \
    free(lst.arr); \
    lst.size = 0;

/**
 * Represents a generic contiguous list.
 *
 * `arr` points to `size` adjacent elements of the caller-selected type.  The
 * storage must remain valid while the list is in use.  Arrays created by the
 * list initializer macros are owned by the list and must be released with
 * `list_free()`; see `make_list()` for externally supplied arrays.
 */
typedef struct List {
    void *arr; /**< Pointer to the contiguous element array. */
    int size;  /**< Number of elements currently stored. */
} List;

/**
 * Wraps an existing array in a `List` without copying it.
 *
 * The function does not copy or allocate `arr`.  For a list that will be
 * released with `list_free()` or grown with `list_append()`, `arr` must be
 * dynamically allocated and compatible with `free()` and `realloc()`; in that
 * case, ownership is transferred to the list.  A read-only list may refer to
 * other storage as long as it remains valid for the list's lifetime.
 *
 * The array must contain at least `size` elements of the type used with the
 * list macros.
 *
 * @param arr Existing element array, or an empty allocation when `size` is 0.
 * @param size Number of elements in `arr`.
 * @return A `List` referring to `arr`.
 */
List make_list(void *arr, int size);

#ifdef __cplusplus
}
#endif

#endif
