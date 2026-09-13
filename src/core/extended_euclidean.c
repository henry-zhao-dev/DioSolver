#include <diosolver/extended_euclidean.h>
#include <stdio.h>
#include <stdlib.h>

static void *dup_eear(const void *item) {
    EEAR *copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        *copy = *(const EEAR *)item;
    }
    return copy;
}

static void destroy_eear(void *item) {
    free(item);
}

static void print_eear(const void *item) {
    const EEAR *row = item;
    printf("(%d, %d, %d, %d)", row->x, row->y, row->r, row->q);
}

static int compare_eear(const void *item1, const void *item2) {
    const EEAR *row1 = item1;
    const EEAR *row2 = item2;

    if (row1->x != row2->x) {
        return (row1->x > row2->x) - (row1->x < row2->x);
    }
    if (row1->y != row2->y) {
        return (row1->y > row2->y) - (row1->y < row2->y);
    }
    if (row1->r != row2->r) {
        return (row1->r > row2->r) - (row1->r < row2->r);
    }
    return (row1->q > row2->q) - (row1->q < row2->q);
}

static cvalue *eear_cvalue;

static void destroy_eear_ctype(void) {
    cvalue_destroy(eear_cvalue);
    eear_cvalue = NULL;
}

static const cvalue *eear_ctype(void) {
    if (eear_cvalue == NULL) {
        eear_cvalue = cvalue_create(sizeof(EEAR), dup_eear, destroy_eear,
                                    print_eear, compare_eear);
        atexit(destroy_eear_ctype);
    }
    return eear_cvalue;
}

static int max_int(const int a, const int b) {
    return a > b ? a : b;
}

static int min_int(const int a, const int b) {
    return a < b ? a : b;
}

static EEAR next_eear(const EEAR previous, const EEAR current) {
    const int quotient = previous.r / current.r;

    return make_eear(previous.x - current.x * quotient,
                     previous.y - current.y * quotient, previous.r % current.r,
                     quotient);
}

EEAR make_eear(const int x, const int y, const int r, const int q) {
    return (EEAR){.x = x, .y = y, .r = r, .q = q};
}

bool equal_eear(const EEAR r1, const EEAR r2) {
    return r1.x == r2.x && r1.y == r2.y && r1.r == r2.r && r1.q == r2.q;
}

EEA_Table *eea_table(const int a, const int b) {
    const int abs_a = abs(a);
    const int abs_b = abs(b);
    EEAR r1 = make_eear(1, 0, max_int(abs_a, abs_b), 0);
    EEAR r2 = make_eear(0, 1, min_int(abs_a, abs_b), 0);

    EEA_Table *table = calist_create_size(eear_ctype(), 2);
    calist_append(table, &r1);
    calist_append(table, &r2);

    while (r2.r != 0) {
        const EEAR next = next_eear(r1, r2);
        r1 = r2;
        r2 = next;
        calist_append(table, &r2);
    }

    return table;
}

bool equal_eea_table(const EEA_Table *t1, const EEA_Table *t2) {
    return calist_equals(t1, t2);
}

EEAR eea_2nd_last_row(const int a, const int b) {
    const int abs_a = abs(a);
    const int abs_b = abs(b);
    EEAR r1 = make_eear(1, 0, max_int(abs_a, abs_b), 0);
    EEAR r2 = make_eear(0, 1, min_int(abs_a, abs_b), 0);

    while (r2.r != 0) {
        const EEAR next = next_eear(r1, r2);
        r1 = r2;
        r2 = next;
    }

    return r1;
}

int eea_gcd(const int a, const int b) {
    return eea_gcd_row(eea_2nd_last_row(a, b));
}

int eea_gcd_table(const EEA_Table *table) {
    const size_t second_last = calist_size(table) - 2;
    return eea_gcd_row(*(const EEAR *)calist_get(table, second_last));
}

int eea_gcd_row(const EEAR row) {
    return row.r;
}
