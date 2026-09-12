#include <assert.h>
#include <diosolver/string_utils.h>
#include <stdio.h>
#include <stdlib.h>

static void test_equal_str(void) {
    assert(equal_str("abc", "abc"));
    assert(!equal_str("abc", "abC"));
    assert(!equal_str("ab c", "abc d"));
}

static void test_fstr(void) {
    char *str = fstr("%d + %d = %d", 1, 2, 3);
    assert(equal_str(str, "1 + 2 = 3"));
    free(str);

    str = fstr("%d%c = %d%c", 2, 'x', 3, 'y');
    assert(equal_str(str, "2x = 3y"));
    free(str);

    str = fstr("My name is %s.", "Henry");
    assert(equal_str(str, "My name is Henry."));
    free(str);
}

int main(void) {
    test_equal_str();
    test_fstr();
    puts("DioSolver string utils tests passed.");
    return 0;
}
