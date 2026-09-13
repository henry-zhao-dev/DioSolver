/**
 * @file interval.h
 * @brief Mathematical interval types, common domains, and operations.
 *
 * An `Interval` represents a real interval with independently open or closed
 * endpoints.  `NEG_INF` and `POS_INF` are the library's sentinel bounds for
 * unbounded intervals.  `INVALID_INTERVAL` is the standard sentinel for an
 * explicitly invalid interval or a failed interval intersection.
 */

#ifndef DIOSOLVER_INTERVAL_H
#define DIOSOLVER_INTERVAL_H

#include <limits.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Sentinel value used as the positive-infinity bound. */
#define POS_INF INT_MAX

/** Sentinel value used as the negative-infinity bound. */
#define NEG_INF INT_MIN

/**
 * Represents a real mathematical interval.
 *
 * A bound is open when the corresponding flag is true, so an open bound is
 * excluded from the interval.  An interval is usable only when `valid` is
 * true and its bounds satisfy `is_valid_interval()`.
 */
typedef struct Interval {
    double low;      /**< Lower bound of the interval. */
    double high;     /**< Upper bound of the interval. */
    bool left_open;  /**< True when the lower endpoint is excluded. */
    bool right_open; /**< True when the upper endpoint is excluded. */

    bool valid; /**< True when the interval has been marked valid. */
} Interval;

/** The unbounded interval `(-inf, inf)`. */
extern const Interval REAL_INTERVAL;

/** The positive interval `(0, inf)`. */
extern const Interval POSITIVE_INTERVAL;

/** The negative interval `(-inf, 0)`. */
extern const Interval NEGATIVE_INTERVAL;

/** The non-positive interval `(-inf, 0]`. */
extern const Interval NONPOSITIVE_INTERVAL;

/** The non-negative interval `[0, inf)`. */
extern const Interval NONNEGATIVE_INTERVAL;

/** Invalid interval sentinel with `valid` set to false. */
extern const Interval INVALID_INTERVAL;

/**
 * Creates an interval with specified bounds and endpoint openness.
 *
 * The constructor sets `valid` to true but does not check whether the bounds
 * form a valid interval.  Call `is_valid_interval()` when validation is
 * required.  Use an open endpoint with `NEG_INF` or `POS_INF` to represent an
 * unbounded side.
 * 
 * @param low Lower bound of the interval.
 * @param high Upper bound of the interval.
 * @param left_open True to exclude the lower endpoint; false to include it.
 * @param right_open True to exclude the upper endpoint; false to include it.
 * @return An initialized `Interval` with `valid` set to true.
 */
Interval make_interval(double low, double high, bool left_open,
                       bool right_open);

/**
 * Checks whether two intervals have identical fields.
 *
 * This is an exact field comparison; it does not normalize equivalent
 * interval representations.
 * 
 * @param i1 The first interval.
 * @param i2 The second interval.
 * @return `true` if all fields match; otherwise, `false`.
 */
bool equal_interval(Interval i1, Interval i2);

/**
 * Formats an interval using mathematical interval notation.
 * 
 * @param intvl The interval to convert.
 * @return A dynamically allocated, null-terminated string representing
 *         `intvl`.  The caller must release it with `free()`.
 */
char *interval_to_str(Interval intvl);

/**
 * Checks whether an interval is valid.
 *
 * A valid interval must have `valid` set to true, must not include either
 * infinity sentinel as a closed endpoint, and must satisfy `low < high` unless
 * both endpoints are closed, in which case `low == high` is also allowed.
 * 
 * @param intvl The interval to check.
 * @return `true` if `intvl` is valid; otherwise, `false`.
 */
bool is_valid_interval(Interval intvl);

/**
 * Checks whether a number lies within an interval.
 *
 * Open endpoints are excluded and closed endpoints are included.  An interval
 * whose `valid` flag is false never contains a value.  Call
 * `is_valid_interval()` separately when the bounds themselves must be
 * validated.
 * 
 * @param n The number to check.
 * @param intvl The interval to check against.
 * @return `true` if `n` is inside `intvl`; otherwise, `false`.
 */
bool is_in_interval(double n, Interval intvl);

/**
 * Computes the intersection of two intervals.
 *
 * If either input is invalid or the intervals do not overlap, the result is
 * `INVALID_INTERVAL`.  Endpoint openness is preserved at shared bounds.
 * 
 * @param i1 The first interval.
 * @param i2 The second interval.
 * @return The intersection of `i1` and `i2`, or `INVALID_INTERVAL` when no valid
 *         intersection exists.
 */
Interval intersection(Interval i1, Interval i2);

/**
 * Computes the interval's integer-only subinterval.
 *
 * The result contains the integer values in `intvl`, with finite endpoints
 * rounded inward and open integer endpoints excluded.  If `intvl` is invalid,
 * the function returns `INVALID_INTERVAL`; callers should also validate the
 * result when a valid integer interval is required, since a valid real
 * interval may contain no integers.
 * 
 * @param intvl The interval to convert.
 * @return An interval representing the integer values in `intvl`.
 */
Interval int_interval(Interval intvl);

/**
 * Counts the integer values contained in an interval.
 *
 * Invalid intervals and valid intervals containing no integers have a count
 * of zero.
 * 
 * @param intvl The interval to evaluate.
 * @return The number of integer values in `intvl`.
 */
int num_int_in(Interval intvl);

#ifdef __cplusplus
}
#endif

#endif
