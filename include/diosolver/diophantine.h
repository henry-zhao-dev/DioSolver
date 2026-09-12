/**
 * @file diophantine.h
 * @brief Types and functions for solving linear Diophantine equations (LDEs).
 *
 * A linear Diophantine equation has the form `a * x + b * y = c`, where `x`
 * and `y` are required to be integers.  When `a` and `b` are nonzero, an
 * integer solution exists exactly when `gcd(a, b)` divides `c`.  If `(x0, y0)`
 * is one solution and `d = gcd(a, b)`, every integer solution is described by
 *
 *     x = x0 + (b / d) * n
 *     y = y0 - (a / d) * n
 *
 * for an integer parameter `n`.  The `LDE` type additionally stores intervals
 * that restrict the allowed integer values of `x` and `y`.
 */

#ifndef DIOSOLVER_DIOPHANTINE_H
#define DIOSOLVER_DIOPHANTINE_H

#include <diosolver/extended_euclidean.h>
#include <diosolver/interval.h>

#ifdef __cplusplus
extern "C" {
#endif

/** A `Solution` value indicating that no solution was found. */
#define NO_SOLN (Solution) {0, 0, false}

/**
 * Represents one candidate solution to an LDE.
 *
 * The `x` and `y` members are meaningful when `exist` is true.  The
 * `NO_SOLN` macro provides the conventional value for a failed solve.
 */
typedef struct Solution {
    int x;      /**< The integer value assigned to `x`. */
    int y;      /**< The integer value assigned to `y`. */

    bool exist; /**< True when this value represents a solution. */
} Solution;

/**
 * Creates a successful solution value with specified coordinates.
 * 
 * This constructor does not verify the equation; callers are responsible for
 * passing values that satisfy the intended LDE.
 *
 * @param x The integer value assigned to `x`.
 * @param y The integer value assigned to `y`.
 * @return An initialized `Solution` with `exist` set to true.
 */
Solution make_solution(int x, int y);

/**
 * Represents a linear Diophantine equation of the form `a * x + b * y = c`.
 *
 * `xi` and `yi` are the domains for `x` and `y`, respectively.  Because this
 * is a Diophantine equation, only integer points inside those intervals are
 * considered by the solver.  Use `REAL` for the library's unconstrained
 * interval.
 */
typedef struct LDE {
    int a;       /**< Coefficient of `x`. */
    int b;       /**< Coefficient of `y`. */
    int c;       /**< Constant term on the right-hand side. */
    Interval xi; /**< Allowed interval for integer values of `x`. */
    Interval yi; /**< Allowed interval for integer values of `y`. */
} LDE;

/**
 * Creates an LDE whose variables use the unconstrained `REAL` interval.
 * 
 * @param a Coefficient of `x`.
 * @param b Coefficient of `y`.
 * @param c Constant term on the right-hand side.
 * @return An initialized `LDE` with `xi == REAL` and `yi == REAL`.
 */
LDE make_lde(int a, int b, int c);

/**
 * Creates an LDE with explicit interval constraints for its variables.
 * 
 * The intervals are stored as supplied.  Use `is_valid_interval()` to
 * validate them before solving when they come from untrusted input.
 *
 * @param a Coefficient of `x`.
 * @param b Coefficient of `y`.
 * @param c Constant term on the right-hand side.
 * @param xi Allowed interval for integer values of `x`.
 * @param yi Allowed interval for integer values of `y`.
 * @return An initialized `LDE` with `xi` and `yi` copied into the structure.
 */
LDE make_lde_in(int a, int b, int c, Interval xi, Interval yi);

/**
 * Finds one particular integer solution using the Extended Euclidean
 * Algorithm (EEA).
 *
 * This function does not apply `lde.xi` or `lde.yi`, and it does not enumerate
 * the complete solution family.  For a constrained solve, use `lde_result()`.
 * The coefficient members `lde.a` and `lde.b` must both be nonzero.
 * 
 * @param lde The LDE to solve; its interval members are ignored.
 * @return A particular `Solution`, or `NO_SOLN` when `gcd(a, b)` does not
 *         divide `c`.
 */
Solution eea_lde(LDE lde);

/**
 * Finds one particular integer solution using a precomputed EEA table.
 *
 * `table` must be the table returned by `eea_table(lde.a, lde.b)` (or have
 * the same layout).  The function only reads the table; the caller retains
 * ownership and must release it with `list_free()`.
 *
 * This function ignores `lde.xi` and `lde.yi`, and `lde.a` and `lde.b` must
 * both be nonzero.
 * 
 * @param lde The LDE to solve; its interval members are ignored.
 * @param table An EEA table generated for `lde.a` and `lde.b`.
 * @return A particular `Solution`, or `NO_SOLN` when `gcd(a, b)` does not
 *         divide `c`.
 */
Solution eea_lde_table(LDE lde, EEA_Table table);

/**
 * Finds one particular integer solution using one EEA table row.
 *
 * `row` must be the second-to-last row of the EEA table generated for
 * `lde.a` and `lde.b`: its coefficients provide the Bézout combination and
 * its remainder is the greatest common divisor.  This function ignores
 * `lde.xi` and `lde.yi`, and `lde.a` and `lde.b` must both be nonzero.
 * 
 * @param lde The LDE to solve; its interval members are ignored.
 * @param row The second-to-last EEA row for `lde.a` and `lde.b`.
 * @return A particular `Solution`, or `NO_SOLN` when the row's GCD does not
 *         divide `lde.c`.
 */
Solution eea_lde_row(LDE lde, EEAR row);

/**
 * Builds a human-readable, step-by-step solution for an LDE.
 *
 * The returned lines explain the EEA calculation, the particular solution,
 * the parameterized complete solution, and the admissible integer values of
 * the parameter when interval constraints are present.  Degenerate cases
 * where `a` or `b` is zero are handled here as well.  A no-solution result is
 * represented by explanatory text in the returned list; this function does
 * not return a `Solution` status value.
 *
 * Each list element is a dynamically allocated `char *`.  The caller owns
 * both the strings and the list storage: free every string, then call
 * `list_free()` on the returned list.
 * 
 * @param lde The LDE to solve, including the integer-domain constraints in
 *             `xi` and `yi`.
 * @return A `List` of dynamically allocated strings, one for each output
 *         line.
 */
List lde_result(LDE lde);

#ifdef __cplusplus
}
#endif

#endif
