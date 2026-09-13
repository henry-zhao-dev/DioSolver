/**
 * @file inequality.h
 * @brief Utilities for solving linear inequalities over intervals.
 *
 * The functions in this header operate on real-valued variables and return
 * their solution sets as `Interval` values.  An invalid input interval or an
 * empty intersection is represented by `INVALID_INTERVAL`.
 */

#ifndef DIOSOLVER_INEQUALITY_H
#define DIOSOLVER_INEQUALITY_H

#include <diosolver/interval.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Operators supported by the inequality solver. */
typedef enum Op {
    GREATER,    /**< Greater than (`>`). */
    GREATER_EQ, /**< Greater than or equal to (`>=`). */
    LESS,       /**< Less than (`<`). */
    LESS_EQ,    /**< Less than or equal to (`<=`). */
} Op;

/**
 * Solves the linear inequality `con + coeff * x op target`.
 *
 * The returned interval contains real values of `x`.  `coeff` must be
 * nonzero.  The endpoint openness of the result follows the selected
 * operator; for example, `GREATER` produces an open boundary while
 * `GREATER_EQ` produces a closed boundary.
 * 
 * @param con Constant term.
 * @param coeff Coefficient of `x`; must not be zero.
 * @param op Inequality operator.
 * @param target Right-hand-side target.
 * @return An `Interval` containing the real solutions to the inequality.
 */
Interval solve_ineq(int con, int coeff, Op op, int target);

/**
 * Finds values of `x` whose affine expression lies within an interval.
 *
 * Specifically, it finds values of `x` for which `con + coeff * x` belongs to
 * `intvl`, respecting the interval's open or closed endpoints.  Thus `intvl`
 * constrains the value of the expression; it is not itself a domain
 * restriction on `x`.  `coeff` must be nonzero and `intvl` must be valid.
 * 
 * @param con Constant term in the affine expression.
 * @param coeff Coefficient of `x`; must not be zero.
 * @param intvl Allowed interval for `con + coeff * x`.
 * @return An `Interval` containing the real values of `x` that satisfy the
 *         constraint, or `INVALID_INTERVAL` when `intvl` is invalid.
 */
Interval solve_ineq_in(int con, int coeff, Interval intvl);

/**
 * Intersects two interval constraints on a shared real parameter.
 *
 * The returned interval contains values of `n` satisfying both
 * `x_con + x_coeff * n` in `xi` and `y_con + y_coeff * n` in `yi`.  This is
 * useful for finding parameter values that keep both coordinates of a
 * parameterized Diophantine solution inside their domains.  Both coefficients
 * must be nonzero and both input intervals must be valid.
 * 
 * @param x_con Constant term for the first affine expression.
 * @param x_coeff Coefficient of the shared parameter in the first expression.
 * @param y_con Constant term for the second affine expression.
 * @param y_coeff Coefficient of the shared parameter in the second expression.
 * @param xi Allowed interval for the first affine expression.
 * @param yi Allowed interval for the second affine expression.
 * @return An `Interval` containing the shared parameter values satisfying both
 *         constraints, or `INVALID_INTERVAL` when the intersection is empty or
 *         either input interval is invalid.
 */
Interval solve_ineq_sys(int x_con, int x_coeff, int y_con, int y_coeff,
                        Interval xi, Interval yi);

#ifdef __cplusplus
}
#endif

#endif
