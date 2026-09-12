/**
 * @file string_utils.h
 * @brief Utilities for comparing and formatting null-terminated strings.
 */

#ifndef DIOSOLVER_COMMON_H
#define DIOSOLVER_COMMON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compares two null-terminated strings for exact equality.
 *
 * The comparison is case-sensitive and has the same content semantics as
 * `strcmp(s1, s2) == 0`, but returns the result directly as a boolean.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @return `true` if the strings are equal; otherwise, `false`.
 *
 * @pre s1 and s2 point to valid null-terminated strings.
 */
bool equal_str(const char *s1, const char *s2);

/**
 * Formats a newly allocated string using `printf`-style arguments.
 *
 * `format` and the variadic arguments follow the rules of `printf`; the
 * caller is responsible for supplying an argument of the correct type for
 * every conversion specifier.
 *
 * @param format The `printf`-style format string.
 * @param ... Arguments referenced by `format`.
 * @return A newly allocated, null-terminated string, or `NULL` if allocation
 *         fails.
 * @warning The caller owns the returned memory and must release it with
 *          `free()` when the return value is non-NULL.
 */
char *fstr(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
