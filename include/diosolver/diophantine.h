/**
 * "diophantine.h" provides functions for solving linear Diophantine equations (LDE).
 */

#ifndef DIOSOLVER_DIOPHANTINE_H
#define DIOSOLVER_DIOPHANTINE_H

#include <diosolver/extended_euclidean.h>
#include <diosolver/interval.h>

#ifdef __cplusplus
extern "C" {
#endif

// Represents no solution for an LDE with the field "exist" set to false
#define NO_SOLN (Solution) {0, 0, false}

// Maximum number of results for LDE solutions
#define MAX_RESULT 10

/**
 * Represents a solution to an LDE.
 */
typedef struct Solution {
    int x;      // x value of the solution
    int y;      // y value of the solution

    bool exist; // True if a solution exists
} Solution;

/**
 * Creates a solution with specified values.
 * 
 * @param x The x value.
 * @param y The y value.
 * @return An initialized solution.
 */
Solution make_solution(int x, int y);

/**
 * Represents a linear Diophantine equation of the form "ax + by = c".
 */
typedef struct LDE {
    int a;          // Coefficient of x
    int b;          // Coefficient of y
    int c;          // Constant term
    Interval xi;    // Interval constraint for x
    Interval yi;    // Interval constraint for y
} LDE;

/**
 * Creates a LDE with x and y in the real interval.
 * 
 * @param a Coefficient of x.
 * @param b Coefficient of y.
 * @param c Constant term.
 * @return An initialized LDE with x and y in the real interval.
 */
LDE make_lde(int a, int b, int c);

/**
 * Creates a LDE with x and y in the specified interval constraints.
 * 
 * @param a Coefficient of x.
 * @param b Coefficient of y.
 * @param c Constant term.
 * @param xi Interval constraint for x.
 * @param yi Interval constraint for y.
 * @return An initialized LDE with x in xi and y in yi.
 */
LDE make_lde_in(int a, int b, int c, Interval xi, Interval yi);

/**
 * Produces a particular solution to the LDE by the EEA.
 * 
 * @param lde The LDE to be solved.
 * @return A particular solution to the LDE.
 */
Solution eea_lde(LDE lde);

/**
 * Produces a particular solution to the LDE using a precomputed EEA table.
 * 
 * @param lde The LDE to be solved.
 * @param table Precomputed EEA table.
 * @return A solution to the LDE.
 */
Solution eea_lde_table(LDE lde, EEA_Table table);

/**
 * Produces a particular solution to the LDE using a single EEA row.
 * 
 * @param lde The LDE to be solved.
 * @param row A row from the EEA table.
 * @return A solution to the LDE.
 */
Solution eea_lde_row(LDE lde, EEAR row);

/**
 * Produces detailed steps to find all solutions to the LDE 
 * within interval constraints.
 * 
 * @param lde The LDE to be solved.
 * @return A list containing each line in the steps.
 */
List lde_result(LDE lde);

#ifdef __cplusplus
}
#endif

#endif
