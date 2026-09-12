#include <diosolver/common.h>
#include <diosolver/interval.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void test_interval_to_str(void) {
    char *intvl_str;
    intvl_str = interval_to_str(make_interval(3, 5, true, true));
    assert(equal_str(intvl_str, "(3,5)"));
    free(intvl_str);

    intvl_str = interval_to_str(make_interval(-5, 3, false, true));
    assert(equal_str(intvl_str, "[-5,3)"));
    free(intvl_str);

    intvl_str = interval_to_str(make_interval(-5, -3, true, false));
    assert(equal_str(intvl_str, "(-5,-3]"));
    free(intvl_str);

    intvl_str = interval_to_str(make_interval(0, 0, false, false));
    assert(equal_str(intvl_str, "[0,0]"));
    free(intvl_str);

    intvl_str = interval_to_str(make_interval(0, 5, true, true));
    assert(equal_str(intvl_str, "(0,5)"));
    free(intvl_str);

    intvl_str = interval_to_str(make_interval(-5.43, 0.012, true, false));
    assert(equal_str(intvl_str, "(-5.43,0.012]"));
    free(intvl_str);

    intvl_str = interval_to_str(POS);
    assert(equal_str(intvl_str, "(0,inf)"));
    free(intvl_str);

    intvl_str = interval_to_str(NEG);
    assert(equal_str(intvl_str, "(-inf,0)"));
    free(intvl_str);

    intvl_str = interval_to_str(REAL);
    assert(equal_str(intvl_str, "(-inf,inf)"));
    free(intvl_str);
}

static void test_is_valid_interval(void) {
    // Valid intervals
    assert(is_valid_interval(make_interval(0, 5, true, true)));     // (0,5)
    assert(is_valid_interval(make_interval(0, 5, true, false)));    // (0,5]
    assert(is_valid_interval(make_interval(0, 5, false, true)));    // [0,5)
    assert(is_valid_interval(make_interval(0, 5, false, false)));   // [0,5]

    // Invalid intervals
    assert(!is_valid_interval(make_interval(5, 5, true, true)));    // (5,5)
    assert(!is_valid_interval(make_interval(5, 5, true, false)));   // (5,5]
    assert(!is_valid_interval(make_interval(5, 5, false, true)));   // [5,5)
    assert(is_valid_interval(make_interval(5, 5, false, false)));   // [5,5]

    // Invalid intervals with low > high
    assert(!is_valid_interval(make_interval(10, 5, true, true)));   // (10,5)
    assert(!is_valid_interval(make_interval(10, 5, true, false)));  // (10,5]
    assert(!is_valid_interval(make_interval(10, 5, false, true)));  // [10,5)
    assert(!is_valid_interval(make_interval(10, 5, false, false))); // [10,5]

    // Closed interval around infinity
    assert(!is_valid_interval(make_interval(NEG_INF, 0, false, false))); // [-inf,0]
    assert(!is_valid_interval(make_interval(0, POS_INF, true, false)));  // (0,inf]
}

static void test_is_in_interval(void) {
    assert(is_in_interval(5, REAL));
    assert(is_in_interval(5, POS));
    assert(is_in_interval(-5.5, NEG));
    assert(!is_in_interval(5.5, NEG));
    assert(!is_in_interval(0, NEG));
    assert(is_in_interval(0, NONNEG));
    assert(is_in_interval(0, NONPOS));
    assert(!is_in_interval(0, INVALID_INTVL));
}

static void test_intersection(void) {
    assert(equal_interval(
        intersection(
            make_interval(137.0 / 5, POS_INF, true, true),
            make_interval(NEG_INF, 274.0 / 9, true, true)),
        make_interval(137.0 / 5, 274.0 / 9, true, true)));

    assert(equal_interval(
        intersection(
            make_interval(-137.0 / 5, POS_INF, true, true),
            make_interval(-274.0 / 9, POS_INF, true, true)),
        make_interval(-137.0 / 5, POS_INF, true, true)));

    assert(equal_interval(
        intersection(
            make_interval(NEG_INF, -137.0 / 5, true, true),
            make_interval(NEG_INF, -274.0 / 9, true, true)),
        make_interval(NEG_INF, -274.0 / 9, true, true)));

    assert(equal_interval(
        intersection(
            make_interval(NEG_INF, 137.0 / 5, true, true),
            make_interval(274.0 / 9, POS_INF, true, true)),
        INVALID_INTVL));

    assert(equal_interval(
        intersection(
            make_interval(3, 4, true, true),
            make_interval(3, 5, false, false)),
        make_interval(3, 4, true, true)));

    assert(equal_interval(
        intersection(
            make_interval(3, 4, false, false),
            make_interval(3, 5, true, false)),
        make_interval(3, 4, true, false)));

    assert(equal_interval(
        intersection(
            make_interval(3, 4, false, true),
            make_interval(3, 5, false, false)),
        make_interval(3, 4, false, true)));

    assert(equal_interval(
        intersection(
            make_interval(5, 7, false, true),
            make_interval(6, 7, true, true)),
        make_interval(6, 7, true, true)));

    assert(equal_interval(
        intersection(
            make_interval(5, 7, true, false),
            make_interval(6, 7, false, false)),
        make_interval(6, 7, false, false)));

    assert(equal_interval(
        intersection(
            make_interval(5, 7, true, true),
            make_interval(6, 7, false, true)),
        make_interval(6, 7, false, true)));

    assert(equal_interval(
        intersection(
            make_interval(5, 5, false, false),
            make_interval(5, 5, false, false)),
        make_interval(5, 5, false, false)));

    assert(equal_interval(
        intersection(
            make_interval(7, 5, true, true),
            make_interval(6, 6, false, false)),
        INVALID_INTVL));

    assert(equal_interval(
        intersection(
            make_interval(5, 5, true, false),
            make_interval(5, 5, false, true)),
        INVALID_INTVL));

    assert(equal_interval(
        intersection(
            make_interval(5, 5, false, false),
            make_interval(5, 5, true, true)),
        INVALID_INTVL));

    assert(equal_interval(
        intersection(
            INVALID_INTVL,
            make_interval(8, 11, true, false)),
        INVALID_INTVL));

    assert(equal_interval(
        intersection(
            make_interval(1, 2, true, true),
            INVALID_INTVL),
        INVALID_INTVL));
}

static void test_int_interval(void) {
    assert(equal_interval(
        int_interval(make_interval(137.0 / 5, 274.0 / 9, true, true)),
        make_interval(28, 30, false, false)));

    assert(equal_interval(
        int_interval(make_interval(137.0 / 5, -274.0 / 9, true, true)),
        INVALID_INTVL));

    assert(equal_interval(
        int_interval(make_interval(-137.0 / 5, 274.0 / 9, true, true)),
        make_interval(-27, 30, false, false)));

    assert(equal_interval(
        int_interval(make_interval(-274.0 / 9, -137.0 / 5, true, true)),
        make_interval(-30, -28, false, false)));

    assert(equal_interval(
        int_interval(make_interval(27, 31, true, true)),
        make_interval(28, 30, false, false)));

    assert(equal_interval(
        int_interval(make_interval(27, 31, true, false)),
        make_interval(28, 31, false, false)));

    assert(equal_interval(
        int_interval(make_interval(27, 31, false, true)),
        make_interval(27, 30, false, false)));

    assert(equal_interval(
        int_interval(make_interval(27, 31, false, false)),
        make_interval(27, 31, false, false)));

    assert(equal_interval(
        int_interval(make_interval(5, 5, true, true)),
        INVALID_INTVL));

    assert(equal_interval(
        int_interval(make_interval(5, 5, false, false)),
        make_interval(5, 5, false, false)));

    assert(equal_interval(
        int_interval(INVALID_INTVL),
        INVALID_INTVL));
}

static void test_num_int_in(void) {
    assert(num_int_in(REAL) == POS_INF);
    assert(num_int_in(POS) == POS_INF);
    assert(num_int_in(NONPOS) == POS_INF);
    assert(num_int_in(INVALID_INTVL) == 0);
    assert(num_int_in(make_interval(3, 5, true, true)) == 1);
    assert(num_int_in(make_interval(3, 5, false, true)) == 2);
    assert(num_int_in(make_interval(3, 5, false, false)) == 3);
    assert(num_int_in(make_interval(137.0 / 5, 274.0 / 9, true, false)) == 3);
}

int main(void) {
    test_interval_to_str();
    test_is_valid_interval();
    test_is_in_interval();
    test_intersection();
    test_int_interval();
    test_num_int_in();
    puts("DioSolver interval tests passed.");
    return 0;
}
