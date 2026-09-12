#include <diosolver/common.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <assert.h>

bool is_int(long double num) {
    return roundf(num) == (float) num;
}

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

void test_common(void) {
    assert(is_int(0));
    assert(is_int(123456789));
    assert(is_int(-123456789.0000));
    assert(!is_int(-0.000010));
    assert(!is_int(123.456));
    assert(!is_int(123.000045678900000));

    assert(equal_str("abc", "abc"));
    assert(!equal_str("abc", "abC"));
    assert(!equal_str("ab c", "abc d"));

    char *str;
    str = fstr("%d + %d = %d", 1, 2, 3);
    assert(equal_str(str, "1 + 2 = 3"));
    free(str);

    str = fstr("%d%c = %d%c", 2, 'x', 3, 'y');
    assert(equal_str(str, "2x = 3y"));
    free(str);

    str = fstr("My name is %s.", "Henry");
    assert(equal_str(str, "My name is Henry."));
    free(str);
}
