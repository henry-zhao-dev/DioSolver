/**
 * "extended_euclidean.h" provides the implementation of the Extended Euclidean Algorithm (EEA)
 * and its applications, such as finding the GCD of two numbers.
 */

#ifndef DIOSOLVER_EXTENDED_EUCLIDEAN_H
#define DIOSOLVER_EXTENDED_EUCLIDEAN_H

#include <diosolver/list.h>

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents a single row in the EEA table, with each field representing
 * a column header from left to right.
 */
typedef struct EEAR {
    int x;
    int y;
    int r;
    int q;
} EEAR;

/**
 * Creates an EEAR structure with specified values.
 * @return An initialized EEAR structure.
 */
EEAR make_eear(int x, int y, int r, int q);

/**
 * Checks if two EEAR structures are equal.
 * 
 * @param r1 The first EEAR.
 * @param r2 The second EEAR.
 * @return true if r1 and r2 are equal, false otherwise.
 */
bool equal_eear(EEAR r1, EEAR r2);

/**
 * A list of EEAR rows representing the EEA table.
 */
typedef List EEA_Table;

/**
 * Generates the EEA table for two integers.
 * 
 * @param a The first integer.
 * @param b The second integer.
 * @return A list of EEAR rows in the EEA table of a and b.
 */
EEA_Table eea_table(int a, int b);

/**
 * Checks if two EEA_Table structures are equal.
 * 
 * @param t1 The first EEA_Table.
 * @param t2 The second EEA_Table.
 * @return true if t1 and t2 are equal, false otherwise.
 */
bool equal_eea_table(EEA_Table t1, EEA_Table t2);

/**
 * Returns the second last row from the EEA table of two integers.
 * 
 * @param a The first integer.
 * @param b The second integer.
 * @return The second last EEAR row.
 */
EEAR eea_2nd_last_row(int a, int b);

/**
 * Produces the greatest common divisor (GCD) of two integers by the EEA.
 * 
 * @param a The first integer.
 * @param b The second integer.
 * @return The GCD of a and b.
 */
int eea_gcd(int a, int b);

/**
 * Extracts the GCD from a precomputed EEA table.
 * 
 * @param table The EEA_Table containing the computation steps.
 * @return The GCD extracted from the table.
 */
int eea_gcd_table(EEA_Table table);

/**
 * Extracts the GCD from a single EEAR row.
 * 
 * @param row The EEAR row containing the remainder.
 * @return The GCD extracted from the row.
 */
int eea_gcd_row(EEAR row);

/**
 * Runs unit tests for functions in "extended_euclidean.h".
 */
void test_extended_euclidean(void);

#ifdef __cplusplus
}
#endif

#endif
