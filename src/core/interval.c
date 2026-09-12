#include <diosolver/interval.h>
#include <diosolver/common.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

Interval make_interval(double low, double high, bool left_open, bool right_open) {
    return (Interval) {low, high, left_open, right_open, true};
}

bool equal_interval(Interval i1, Interval i2) {
    return i1.low == i2.low &&
           i1.high == i2.high &&
           i1.left_open == i2.left_open &&
           i1.right_open == i2.right_open &&
           i1.valid == i2.valid;
}

char *interval_to_str(Interval intvl) {
    if (intvl.low == NEG_INF && intvl.high == POS_INF) {
        return fstr("(-inf,inf)");
    }
    if (intvl.low == NEG_INF) {
        return fstr("(-inf,%g%c", intvl.high, intvl.right_open ? ')' : ']');
    }
    if (intvl.high == POS_INF) {
        return fstr("%c%g,inf)", intvl.left_open ? '(' : '[', intvl.low);
    }
    return fstr("%c%g,%g%c", intvl.left_open ? '(' : '[', intvl.low,
                      intvl.high, intvl.right_open ? ')' : ']');
}

bool is_valid_interval(Interval intvl) {
    return intvl.valid &&
           (intvl.low != NEG_INF || intvl.left_open) &&
           (intvl.high != POS_INF || intvl.right_open) &&
           ((!intvl.left_open && !intvl.right_open && intvl.low <= intvl.high) ||
            (intvl.low < intvl.high));
}

bool is_in_interval(double n, Interval intvl) {
    return intvl.valid &&
           (intvl.left_open ? (n > intvl.low) : (n >= intvl.low)) &&
           (intvl.right_open ? (n < intvl.high) : (n <= intvl.high));
}

Interval intersection(Interval i1, Interval i2) {
    if (!is_valid_interval(i1) || !is_valid_interval(i2)) {
        return INVALID_INTVL;
    }

    double low = fmax(i1.low, i2.low);
    double high = fmin(i1.high, i2.high);
    
    bool left_open;
    if (i1.low > i2.low) {
        left_open = i1.left_open;
    } else if (i1.low == i2.low) {
        left_open = i1.left_open || i2.left_open;
    } else {
        left_open = i2.left_open;
    }

    bool right_open;
    if (i1.high < i2.high) {
        right_open = i1.right_open;
    } else if (i1.high == i2.high) {
        right_open = i1.right_open || i2.right_open;
    } else {
        right_open = i2.right_open;
    }

    Interval intvl = make_interval(low, high, left_open, right_open);
    return is_valid_interval(intvl) ? intvl : INVALID_INTVL;
}

Interval int_interval(Interval intvl) {
    if (!is_valid_interval(intvl)) {
        return INVALID_INTVL;
    }

    int low;
    if (intvl.left_open && is_int(intvl.low) && intvl.low != NEG_INF) {
        low = intvl.low + 1;
    } else {
        low = ceil(intvl.low);
    }

    int high;
    if (intvl.right_open && is_int(intvl.high) && intvl.high != POS_INF) {
        high = intvl.high - 1;
    } else {
        high = floor(intvl.high);
    }

    return make_interval(low, high, intvl.low == NEG_INF, intvl.high == POS_INF);
}

int num_int_in(Interval intvl) {
    if (!is_valid_interval(intvl)) {
        return 0;    
    }
    
    Interval int_intvl = int_interval(intvl);
    return int_intvl.high - int_intvl.low + 1;
}

void test_interval_to_str(void) {
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

void test_is_valid_interval(void) {
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

void test_is_in_interval(void) {
    assert(is_in_interval(5, REAL));
    assert(is_in_interval(5, POS));
    assert(is_in_interval(-5.5, NEG));
    assert(!is_in_interval(5.5, NEG));
    assert(!is_in_interval(0, NEG));
    assert(is_in_interval(0, NONNEG));
    assert(is_in_interval(0, NONPOS));
    assert(!is_in_interval(0, INVALID_INTVL));
}

void test_intersection(void) {
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

void test_int_interval(void) {
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

void test_num_int_in(void) {
    assert(num_int_in(REAL) == POS_INF);
    assert(num_int_in(POS) == POS_INF);
    assert(num_int_in(NONPOS) == POS_INF);
    assert(num_int_in(INVALID_INTVL) == 0);
    assert(num_int_in(make_interval(3, 5, true, true)) == 1);
    assert(num_int_in(make_interval(3, 5, false, true)) == 2);
    assert(num_int_in(make_interval(3, 5, false, false)) == 3);
    assert(num_int_in(make_interval(137.0 / 5, 274.0 / 9, true, false)) == 3);
}

void test_interval(void) {
    test_interval_to_str();
    test_is_valid_interval();
    test_is_in_interval();
    test_intersection();
    test_int_interval();
    test_num_int_in();
}
