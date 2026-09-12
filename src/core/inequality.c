#include <diosolver/inequality.h>

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
