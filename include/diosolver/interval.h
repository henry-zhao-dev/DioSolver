/**
 * "interval.h" provides functions and macros for working with intervals.
 */

#ifndef DIOSOLVER_INTERVAL_H
#define DIOSOLVER_INTERVAL_H

#include <stdbool.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

// Positive and negative infinity
#define POS_INF INT_MAX
#define NEG_INF -POS_INF

// Common intervals
#define REAL (Interval) {NEG_INF, POS_INF, true, true, true}
#define POS (Interval) {0, POS_INF, true, true, true}
#define NEG (Interval) {NEG_INF, 0, true, true, true}
#define NONPOS (Interval) {NEG_INF, 0, true, false, true}
#define NONNEG (Interval) {0, POS_INF, false, true, true}

// Invalid interval with the field 'valid' set to false
#define INVALID_INTVL (Interval) {0, 0, true, true, false}

/**
 * Represents a mathematical interval.
 */
typedef struct Interval {
    double low;         // Lower bound of the interval
    double high;        // Upper bound of the interval
    bool left_open;     // True if the interval includes the left endpoint
    bool right_open;    // True if the interval includes the right endpoint

    bool valid;         // True if the interval is valid
} Interval;

/**
 * Creates an interval with specified bounds and openness.
 * 
 * @param low Lower bound of the interval.
 * @param high Upper bound of the interval.
 * @param left_open True if the interval is left-open.
 * @param right_open True if the interval is right-open.
 * @return An initialized interval.
 */
Interval make_interval(double low, double high, bool left_open, bool right_open);

/**
 * Checks if two intervals are equal.
 * 
 * @param i1 The first interval.
 * @param i2 The second interval.
 * @return True if i1 and i2 are equal, false otherwise.
 */
bool equal_interval(Interval i1, Interval i2);

/**
 * Converts an interval to string.
 * 
 * @param intvl The interval to convert.
 * @return A dynamically allocated string representing intvl.
 *         Make sure to call free() after usage.
 */
char *interval_to_str(Interval intvl);

/**
 * Checks if an interval is valid:
 *   - low <= high, if left_open and right_open are both true.
 *   - low < high, otherwise.
 * 
 * @param intvl The interval to check.
 * @return True if intvl is valid, false otherwise.
 */
bool is_valid_interval(Interval intvl);

/**
 * Checks if a number is within an interval.
 * 
 * @param n The number to check.
 * @param intvl The interval to check against.
 * @return True if n is within the interval, false otherwise.
 */
bool is_in_interval(double n, Interval intvl);

/**
 * Computes the intersection of two intervals.
 * 
 * @param i1 The first interval.
 * @param i2 The second interval.
 * @return The intersection of i1 and i2.
 */
Interval intersection(Interval i1, Interval i2);

/**
 * Converts an interval to its integer-only subinterval.
 * 
 * @param intvl The interval to convert.
 * @return The integer subinterval of intvl.
 */
Interval int_interval(Interval intvl);

/**
 * Counts the number of integers within an interval.
 * 
 * @param intvl The interval to evaluate.
 * @return The number of integers within intvl.
 */
int num_int_in(Interval intvl);

#ifdef __cplusplus
}
#endif

#endif
