#include <diosolver/common.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void test_common(void) {
    assert(is_int(0));
    assert(is_int(123456789));
    assert(is_int(-123456789.0000));
    assert(!is_int(-0.000010));
    assert(!is_int(123.456));
    assert(!is_int(123.000045678900000));

    assert(equal_str("abc", "abc"));
    assert(!equal_str("abc", "abC"));
    assert(!equal_str("ab c", "abc d"));

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
    test_common();
    puts("DioSolver common tests passed.");
    return 0;
}
