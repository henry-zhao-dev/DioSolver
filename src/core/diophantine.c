#include <diosolver/diophantine.h>
#include <diosolver/inequality.h>
#include <diosolver/string_utils.h>
#include <stdlib.h>

const Solution NO_SOLUTION = {0, 0, false};

Solution make_solution(const int x, const int y) {
    return (Solution){.x = x, .y = y, .exist = true};
}

LDE make_lde(const int a, const int b, const int c) {
    return (LDE){
        .a = a, .b = b, .c = c, .xi = REAL_INTERVAL, .yi = REAL_INTERVAL};
}

LDE make_lde_in(const int a, const int b, const int c, const Interval xi,
                const Interval yi) {
    return (LDE){.a = a, .b = b, .c = c, .xi = xi, .yi = yi};
}

Solution eea_lde(const LDE lde) {
    return eea_lde_row(lde, eea_2nd_last_row(lde.a, lde.b));
}

Solution eea_lde_table(const LDE lde, const EEA_Table *table) {
    const size_t second_last = calist_size(table) - 2;
    const EEAR row = *(const EEAR *)calist_get(table, second_last);
    return eea_lde_row(lde, row);
}

Solution eea_lde_row(const LDE lde, const EEAR row) {
    const int a = lde.a;
    const int b = lde.b;
    const int c = lde.c;
    const int abs_a = abs(a);
    const int abs_b = abs(b);

    const int gcd_ab = eea_gcd_row(row);
    if (c % gcd_ab != 0) {
        return NO_SOLUTION;
    }

    const int factor = c / gcd_ab;
    const int x = row.x * factor;
    const int y = row.y * factor;

    Solution soln = make_solution(abs_a / a, abs_b / b);
    if (abs_a > abs_b) {
        soln.x *= x;
        soln.y *= y;
    } else {
        soln.x *= y;
        soln.y *= x;
    }
    return soln;
}

static char *lde_to_str(const int a, const int b, const int c) {
    char *a_str = (a == 1) ? fstr("") : (a == -1) ? fstr("-") : fstr("%d", a);
    const char op = (b < 0) ? '-' : '+';
    char *b_str = (abs(b) == 1) ? fstr("") : fstr("%d", abs(b));
    char *lde_str = fstr("\t%sx %c %sy = %d\n", a_str, op, b_str, c);
    free(a_str);
    free(b_str);
    return lde_str;
}

static char *lde_soln_to_str(const int a, const int b, const int c, const int x,
                             const int y) {
    const char op = (b < 0) ? '-' : '+';
    char *a_str = (a == 1) ? fstr("") : (a == -1) ? fstr("-") : fstr("%d", a);
    char *b_str = (abs(b) == 1) ? fstr("") : fstr("%d", abs(b));
    char *lde_str = fstr("%s(%d) %c %s(%d) = %d", a_str, x, op, b_str, y, c);
    free(a_str);
    free(b_str);
    return lde_str;
}

static char *n_eq_to_str(const int a, const int b) {
    if (a == 0) {
        return (b == 1) ? fstr("n") : (b == -1) ? fstr("-n") : fstr("%dn", b);
    }

    const char op = (b < 0) ? '-' : '+';
    char *b_str = (abs(b) == 1) ? fstr("n") : fstr("%dn", abs(b));
    char *eq_str = fstr("%d %c %s", a, op, b_str);
    free(b_str);
    return eq_str;
}

static calist *result;

static void append_result(char *str) {
    calist_append(result, str);
    free(str);
}

static void solve_lde_ab0(const int c, const Interval xi, const Interval yi) {
    if (c != 0) {
        append_result(
            fstr("Since a = 0, b = 0, and c ≠ 0, the LDE has no solution.\n"));
        return;
    }

    char *xi_str = interval_to_str(xi);
    append_result(fstr("x is any integer in the interval %s\n", xi_str));
    free(xi_str);

    char *yi_str = interval_to_str(yi);
    append_result(fstr("y is any integer in the interval %s\n", yi_str));
    free(yi_str);
}

static void solve_lde_a0(const int b, const int c, const Interval xi,
                         const Interval yi) {
    if (c % b != 0) {
        append_result(fstr("Since %d does not divide %d, ", b, c));
        append_result(fstr("the LDE has no integer solution.\n"));
        return;
    }

    char *xi_str = interval_to_str(xi);
    append_result(fstr("x is any integer in the interval %s\n", xi_str));
    free(xi_str);

    const int y = c / b;
    append_result(fstr("y = %d\n", y));

    if (!is_in_interval(y, yi)) {
        char *yi_str = interval_to_str(yi);
        append_result(
            fstr("However, %d is not in the interval %s\n", y, yi_str));
        append_result(fstr("Therefore, the LDE has no solution.\n"));
        free(yi_str);
    }
}

static void solve_lde_b0(const int a, const int c, const Interval xi,
                         const Interval yi) {
    if (c % a != 0) {
        append_result(fstr("Since %d does not divide %d, ", a, c));
        append_result(fstr("the LDE has no integer solution.\n"));
        return;
    }

    const int x = c / a;
    append_result(fstr("x = %d\n", x));

    if (!is_in_interval(x, xi)) {
        char *xi_str = interval_to_str(xi);
        append_result(
            fstr("However, %d is not in the interval %s\n", x, xi_str));
        append_result(fstr("Therefore, the LDE has no solution.\n"));
        free(xi_str);
        return;
    }

    char *yi_str = interval_to_str(yi);
    append_result(fstr("y is any integer in the interval %s\n", yi_str));
    free(yi_str);
}

static void solve_lde_in(const int a, const int b, const int c,
                         const Interval xi, const Interval yi) {
    EEA_Table *table = eea_table(a, b);
    const int d = eea_gcd_table(table);

    append_result(fstr("By the Extended Euclidean Algorithm (EEA):\n"));
    append_result(fstr("x\ty\tr\tq\n"));
    for (size_t i = 0; i < calist_size(table); ++i) {
        const EEAR eear = *(const EEAR *)calist_get(table, i);
        append_result(fstr("%d\t%d\t%d\t%d\n", eear.x, eear.y, eear.r, eear.q));
    }

    append_result(fstr("\nGCD(%d, %d) = %d\n\n", a, b, d));
    if (c % d != 0) {
        append_result(fstr("Since %d does not divide %d, ", d, c));
        append_result(fstr("the LDE has no solution.\n"));
        calist_destroy(table);
        return;
    }

    Solution part_soln = eea_lde_table(make_lde(a, b, d), table);
    append_result(fstr("From the EEA Table:\n"));

    char *soln_str = lde_soln_to_str(a, b, d, part_soln.x, part_soln.y);
    append_result(fstr("\t%s\n", soln_str));
    free(soln_str);

    part_soln = eea_lde_table(make_lde(a, b, c), table);
    const int x0 = part_soln.x;
    const int y0 = part_soln.y;
    append_result(fstr("Thus:\n"));
    soln_str = lde_soln_to_str(a, b, c, x0, y0);
    append_result(fstr("\t%s\n\n", soln_str));
    free(soln_str);
    calist_destroy(table);

    append_result(fstr("A particular solution is:\n"));
    append_result(fstr("\tx₀ = %d\n", x0));
    append_result(fstr("\ty₀ = %d\n", y0));

    append_result(fstr("\nThe complete solution is:\n"));
    char *x_eq = n_eq_to_str(x0, b / d);
    char *y_eq = n_eq_to_str(y0, -a / d);
    append_result(fstr("\tx = %s\n", x_eq));
    append_result(fstr("\ty = %s\n", y_eq));
    free(x_eq);
    free(y_eq);

    const Interval n_intvl =
        int_interval(solve_ineq_sys(x0, b / d, y0, -a / d, xi, yi));
    if (is_valid_interval(n_intvl)) {
        char *n_intvl_str = interval_to_str(n_intvl);
        append_result(fstr("Where:\n\tn ∈ %s\n", n_intvl_str));
        free(n_intvl_str);
    } else {
        char *xi_str = interval_to_str(xi);
        char *yi_str = interval_to_str(yi);
        append_result(
            fstr("\nHowever, there does not exist an integer n such that:\n"));
        append_result(fstr("\tx ∈ %s\n", xi_str));
        append_result(fstr("\ty ∈ %s\n", yi_str));
        append_result(fstr("Therefore, the LDE has no solution.\n"));
        free(xi_str);
        free(yi_str);
    }
}

calist *lde_result(const LDE lde) {
    const int a = lde.a;
    const int b = lde.b;
    const int c = lde.c;
    const Interval xi = lde.xi;
    const Interval yi = lde.yi;

    result = calist_create(cvalue_string());
    append_result(fstr("Solving the Linear Diophantine Equation (LDE):\n"));
    append_result(lde_to_str(a, b, c));
    append_result(fstr("Where:\n"));
    char *xi_str = interval_to_str(xi);
    char *yi_str = interval_to_str(yi);
    append_result(fstr("\tx ∈ %s\n", xi_str));
    append_result(fstr("\ty ∈ %s\n\n", yi_str));
    free(xi_str);
    free(yi_str);

    if (a == 0 && b == 0) {
        solve_lde_ab0(c, xi, yi);
    } else if (a == 0) {
        solve_lde_a0(b, c, xi, yi);
    } else if (b == 0) {
        solve_lde_b0(a, c, xi, yi);
    } else {
        solve_lde_in(a, b, c, xi, yi);
    }

    return result;
}
