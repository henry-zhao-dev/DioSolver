/**
 * "common.h" provides utilities for integer and string operations in C.
 */

#ifndef DIOSOLVER_COMMON_H
#define DIOSOLVER_COMMON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Checks if a number is an integer.
 * 
 * @param num The number to check.
 * @return true if num is an integer, false otherwise.
 */
bool is_int(long double num);

/**
 * Checks if two strings are equal.
 * 
 * @param s1 The first string.
 * @param s2 The second string.
 * @return true if s1 and s2 are equal, false otherwise.
 */
bool equal_str(const char *s1, const char *s2);

/**
 * Formats a string.
 * 
 * @param format The format string (printf-style).
 * @param ... Additional arguments for formatting.
 * @return A dynamically allocated formatted string. 
 *         Make sure to call free() after usage.
 */
char *fstr(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
