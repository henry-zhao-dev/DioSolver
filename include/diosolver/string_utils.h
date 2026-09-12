/**
 * Provides utilities for comparing and formatting null-terminated strings.
 */

#ifndef DIOSOLVER_COMMON_H
#define DIOSOLVER_COMMON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Determines whether two null-terminated strings are equal.
 *
 * Equivalent to checking whether strcmp(s1, s2) == 0, but returns the result
 * directly as a boolean.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @return true if the strings are equal; false otherwise.
 *
 * @pre s1 and s2 point to valid null-terminated strings.
 */
bool equal_str(const char *s1, const char *s2);

/**
 * Formats a string using printf-style arguments.
 *
 * @param format The printf-style format string.
 * @param ... Arguments referenced by format.
 * @return A newly allocated, null-terminated string, or NULL if allocation
 *         fails.
 * @warning The caller owns the returned memory and must free it when non-NULL.
 */
char *fstr(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
