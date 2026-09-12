#include <diosolver/interval.h>
#include <diosolver/common.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
