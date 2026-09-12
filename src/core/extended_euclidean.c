#include <diosolver/extended_euclidean.h>

#include <stdio.h>
#include <stdlib.h>

static void *dup_eear(const void *item) {
    EEAR *copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        *copy = *(const EEAR *) item;
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

static ctype *eear_ctype_instance;

static void destroy_eear_ctype(void) {
    ctype_destroy(eear_ctype_instance);
    eear_ctype_instance = NULL;
}

static const ctype *eear_ctype(void) {
    if (eear_ctype_instance == NULL) {
        eear_ctype_instance = ctype_create(
            sizeof(EEAR), dup_eear, destroy_eear, print_eear, compare_eear);
        atexit(destroy_eear_ctype);
    }
    return eear_ctype_instance;
}

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static int min_int(int a, int b) {
    return a < b ? a : b;
}

EEAR make_eear(const int x, const int y, const int r, const int q) {
    return (EEAR) {.x = x, .y = y, .r = r, .q = q};
}

bool equal_eear(const EEAR r1, const EEAR r2) {
    return r1.x == r2.x &&
           r1.y == r2.y &&
           r1.r == r2.r &&
           r1.q == r2.q;
}

EEA_Table *eea_table(const int a, const int b) {
    EEAR r1 = make_eear(1, 0, max_int(abs(a), abs(b)), 0);
    EEAR r2 = make_eear(0, 1, min_int(abs(a), abs(b)), 0);

    EEA_Table *table = calist_create_size(eear_ctype(), 2);
    calist_append(table, &r1);
    calist_append(table, &r2);
    
    while (r2.r != 0) {
        const int q = r1.r / r2.r;
        const int r = r1.r % r2.r;
        const int x = r1.x - r2.x * q;
        const int y = r1.y - r2.y * q;

        r1 = r2;
        r2 = make_eear(x, y, r, q);
        calist_append(table, &r2);
    }

    return table;
}

bool equal_eea_table(const EEA_Table *t1, const EEA_Table *t2) {
    return calist_equals(t1, t2);
}

EEAR eea_2nd_last_row(int a, int b) {
    EEAR r1 = make_eear(1, 0, max_int(abs(a), abs(b)), 0);
    EEAR r2 = make_eear(0, 1, min_int(abs(a), abs(b)), 0);

    while (r2.r != 0) {
        const int q = r1.r / r2.r;
        const int r = r1.r % r2.r;
        const int x = r1.x - r2.x * q;
        const int y = r1.y - r2.y * q;

        r1 = r2;
        r2 = make_eear(x, y, r, q);
    }

    return r1;
}

int eea_gcd(const int a, const int b) {
    return eea_gcd_row(eea_2nd_last_row(a, b));
}

int eea_gcd_table(const EEA_Table *table) {
    const size_t second_last = calist_size(table) - 2;
    return eea_gcd_row(*(const EEAR *) calist_get(table, second_last));
}

int eea_gcd_row(const EEAR row) {
    return row.r;
}
