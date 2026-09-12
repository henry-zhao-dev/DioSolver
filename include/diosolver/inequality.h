/**
 * "inequality.h" provides functions for solving inequalities involving intervals.
 */

#ifndef DIOSOLVER_INEQUALITY_H
#define DIOSOLVER_INEQUALITY_H

#include <diosolver/interval.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Operators for inequalities.
 */
typedef enum Op {
    GREATER,    // >
    GREATER_EQ, // >=
    LESS,       // <
    LESS_EQ,    // <=
} Op;

/**
 * Solves a linear inequality of the form "[con] + [coeff]*x [op] [target]".
 * 
 * @param con Constant term.
 * @param coeff Coefficient of the variable.
 * @param op Inequality operator.
 * @param target Target value.
 * @return An interval containing the solutions to the inequality.
 */
Interval solve_ineq(int con, int coeff, Op op, int target);

/**
 * Solves a linear inequality within a specified interval.
 * 
 * @param con Constant term.
 * @param coeff Coefficient of the variable.
 * @param intvl Interval constraint for the variable.
 * @return An interval containing the solutions to the inequality.
 */
Interval solve_ineq_in(int con, int coeff, Interval intvl);

/**
 * Solves a system of two linear inequalities with interval constraints.
 * 
 * @param x_con Constant for x inequality.
 * @param x_coeff Coefficient for x.
 * @param y_con Constant for y inequality.
 * @param y_coeff Coefficient for y.
 * @param xi Interval constraint for x.
 * @param yi Interval constraint for y.
 * @return An interval containing the solutions to the system.
 */
Interval solve_ineq_sys(int x_con, int x_coeff, int y_con, int y_coeff,
                        Interval xi, Interval yi);

#ifdef __cplusplus
}
#endif

#endif
