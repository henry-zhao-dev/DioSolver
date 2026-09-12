#include <diosolver/inequality.h>

Interval solve_ineq(const int con, const int coeff, const Op op, const int target) {
    const bool op_greater = (op == GREATER || op == GREATER_EQ);
    const bool open = (op == GREATER || op == LESS);

    if (target == POS_INF) {
        return op_greater ? INVALID_INTVL : REAL;
    }
    if (target == NEG_INF) {
        return op_greater ? REAL : INVALID_INTVL;
    }

    const double bound = (double) (target - con) / coeff;
    if (coeff > 0) {
        return op_greater ? make_interval(bound, POS_INF, open, true)
                          : make_interval(NEG_INF, bound, true, open);
    }
    return op_greater ? make_interval(NEG_INF, bound, true, open)
                      : make_interval(bound, POS_INF, open, true);
}

Interval solve_ineq_in(const int con, const int coeff, const Interval intvl) {
    if (!is_valid_interval(intvl)) {
        return INVALID_INTVL;
    }

    const Interval intvl_greater = solve_ineq(
        con, coeff, intvl.left_open ? GREATER : GREATER_EQ, (int) intvl.low);
    const Interval intvl_less = solve_ineq(
        con, coeff, intvl.right_open ? LESS : LESS_EQ, (int) intvl.high);

    return intersection(intvl_greater, intvl_less);
}

Interval solve_ineq_sys(const int x_con, const int x_coeff,
                        const int y_con, const int y_coeff,
                        const Interval xi, const Interval yi) {
    return intersection(
        solve_ineq_in(x_con, x_coeff, xi),
        solve_ineq_in(y_con, y_coeff, yi));
}
