#include <diosolver/inequality.h>

#include <assert.h>
#include <stdio.h>

static void test_solve_ineq(void) {
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

static void test_solve_ineq_in(void) {
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

static void test_solve_ineq_sys(void) {
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

int main(void) {
    test_solve_ineq();
    test_solve_ineq_in();
    test_solve_ineq_sys();
    puts("DioSolver inequality tests passed.");
    return 0;
}
