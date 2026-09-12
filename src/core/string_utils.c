#include <diosolver/string_utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

bool equal_str(const char *s1, const char *s2) {
    return !strcmp(s1, s2);
}

char *fstr(const char *format, ...) {
    va_list args;

    // Determine the length of the formatted string
    va_start(args, format);
    size_t str_len = vsnprintf(NULL, 0, format, args) + 1; // +1 for the null terminator
    va_end(args);

    char *result = malloc(str_len);
    if (!result) {
        return NULL;
    }

    // Format the string into the allocated memory
    va_start(args, format);
    vsnprintf(result, str_len, format, args);
    va_end(args);

    return result;
}
