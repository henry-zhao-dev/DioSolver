#include <diosolver/string_utils.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool equal_str(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

char *fstr(const char *format, ...) {
    va_list args;

    va_start(args, format);
    const int formatted_len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (formatted_len < 0) {
        return NULL;
    }

    const size_t str_len = (size_t)formatted_len + 1;

    char *result = malloc(str_len);
    if (result == NULL) {
        return NULL;
    }

    va_start(args, format);
    const int written_len = vsnprintf(result, str_len, format, args);
    va_end(args);

    if (written_len < 0) {
        free(result);
        return NULL;
    }

    return result;
}
