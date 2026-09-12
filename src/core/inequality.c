#include <diosolver/inequality.h>

#include <assert.h>

Interval solve_ineq(int con, int coeff, Op op, int target) {
    double bound = (double) (target - con) / coeff;
    bool op_greater = (op == GREATER || op == GREATER_EQ);
    bool open = (op == GREATER || op == LESS);

    if (target == POS_INF) {
        return op_greater ? INVALID_INTVL : REAL;
    }
    if (target == NEG_INF) {
        return op_greater ? REAL : INVALID_INTVL;
    }
    if (coeff > 0) {
        return op_greater ? make_interval(bound, POS_INF, open, true)
                          : make_interval(NEG_INF, bound, true, open);
    }
    return op_greater ? make_interval(NEG_INF, bound, true, open)
                      : make_interval(bound, POS_INF, open, true);
}

Interval solve_ineq_in(int con, int coeff, Interval intvl) {
    if (!is_valid_interval(intvl)) {
        return INVALID_INTVL;
    }

    Interval intvl_greater 
        = solve_ineq(con, coeff, intvl.left_open ? GREATER : GREATER_EQ, intvl.low);
    Interval intvl_less 
        = solve_ineq(con, coeff, intvl.right_open ? LESS : LESS_EQ, intvl.high);

    return intersection(intvl_greater, intvl_less);
}

Interval solve_ineq_sys(int x_con, int x_coeff, int y_con, int y_coeff, 
                        Interval xi, Interval yi) {
    return intersection(solve_ineq_in(x_con, x_coeff, xi),
                        solve_ineq_in(y_con, y_coeff, yi));
}

void test_solve_ineq(void) {
    assert(equal_interval(
        solve_ineq(-137, 5, GREATER, 0),
        make_interval(137.0 / 5, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq(274, -9, GREATER, 0),
        make_interval(NEG_INF, 274.0 / 9, true, true)));

    assert(equal_interval(
        solve_ineq(137, 5, GREATER, 0),
        make_interval(-137.0 / 5, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq(274, 9, GREATER, 0),
        make_interval(-274.0 / 9, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq(-137, -5, GREATER, 0),
        make_interval(NEG_INF, -137.0 / 5, true, true)));

    assert(equal_interval(
        solve_ineq(-274, -9, GREATER, 0),
        make_interval(NEG_INF, -274.0 / 9, true, true)));

    assert(equal_interval(
        solve_ineq(137, -5, GREATER, 0),
        make_interval(NEG_INF, 137.0 / 5, true, true)));

    assert(equal_interval(
        solve_ineq(-274, 9, GREATER, 0),
        make_interval(274.0 / 9, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq(50, 4, GREATER_EQ, 0),
        make_interval(-50.0 / 4, POS_INF, false, true)));

    assert(equal_interval(
        solve_ineq(-50, -5, GREATER_EQ, 0),
        make_interval(NEG_INF, -10, true, false)));

    assert(equal_interval(
        solve_ineq(4, 7, LESS, 6),
        make_interval(NEG_INF, 2.0 / 7, true, true)));

    assert(equal_interval(
        solve_ineq(5, -3, LESS, 8),
        make_interval(-1, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq(4, 7, LESS_EQ, 6),
        make_interval(NEG_INF, 2.0 / 7, true, false)));

    assert(equal_interval(
        solve_ineq(5, -3, LESS_EQ, 8),
        make_interval(-1, POS_INF, false, true)));

    assert(equal_interval(
        solve_ineq(0, -1, LESS_EQ, 0),
        NONNEG));

    assert(equal_interval(
        solve_ineq(50, 4, LESS, POS_INF),
        REAL));
}

void test_solve_ineq_in(void) {
    assert(equal_interval(
        solve_ineq_in(50, 4, POS),
        make_interval(-50.0 / 4, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq_in(50, 4, NEG),
        make_interval(NEG_INF, -50.0 / 4, true, true)));

    assert(equal_interval(
        solve_ineq_in(50, -4, POS),
        make_interval(NEG_INF, 50.0 / 4, true, true)));

    assert(equal_interval(
        solve_ineq_in(-50, 4, POS),
        make_interval(50.0 / 4, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq_in(-50, -4, POS),
        make_interval(NEG_INF, -50.0 / 4, true, true)));
}

void test_solve_ineq_sys(void) {
    assert(equal_interval(
        solve_ineq_sys(-137, 5, 274, -9, POS, POS),
        make_interval(137.0 / 5, 274.0 / 9, true, true)));

    assert(equal_interval(
        solve_ineq_sys(137, 5, 274, 9, POS, POS),
        make_interval(-137.0 / 5, POS_INF, true, true)));

    assert(equal_interval(
        solve_ineq_sys(-137, -5, -274, -9, POS, POS),
        make_interval(NEG_INF, -274.0 / 9, true, true)));

    assert(equal_interval(
        solve_ineq_sys(137, -5, -274, 9, POS, POS),
        INVALID_INTVL));

    assert(equal_interval(
        solve_ineq_sys(50, 4, -50, -5, POS, POS),
        make_interval(-50.0 / 4, -10, true, true)));

    assert(equal_interval(
        solve_ineq_sys(-50, 4, -50, 5, POS, POS),
        make_interval(50.0 / 4, POS_INF, true, true)));
}

void test_inequality(void) {
    test_solve_ineq();
    test_solve_ineq_in();
    test_solve_ineq_sys();
}
