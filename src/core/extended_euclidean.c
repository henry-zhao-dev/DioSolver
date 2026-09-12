#include <diosolver/extended_euclidean.h>
#include <diosolver/common.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

EEAR make_eear(int x, int y, int r, int q) {
    return (EEAR) {x, y, r, q};
}

bool equal_eear(EEAR r1, EEAR r2) {
    return r1.x == r2.x &&
           r1.y == r2.y &&
           r1.r == r2.r &&
           r1.q == r2.q;
}

EEA_Table eea_table(int a, int b) {
    EEAR r1 = make_eear(1, 0, fmax(abs(a), abs(b)), 0);
    EEAR r2 = make_eear(0, 1, fmin(abs(a), abs(b)), 0);

    List table = list_init_size(2, EEAR);
    list_set(table, 0, r1, EEAR);
    list_set(table, 1, r2, EEAR);
    
    while (r2.r != 0) {
        int q = r1.r / r2.r;
        int r = r1.r % r2.r;
        int x = r1.x - r2.x * q;
        int y = r1.y - r2.y * q;

        r1 = r2;
        r2 = make_eear(x, y, r, q);
        list_append(table, r2, EEAR);
    }

    return table;
}

bool equal_eea_table(EEA_Table t1, EEA_Table t2) {
    if (t1.size != t2.size) {
        return false;
    }

    for (int i = 0; i < t1.size; ++i) {
        if (!equal_eear(list_at(t1, i, EEAR), list_at(t2, i, EEAR))) {
            return false;
        }
    }

    return true;
}

EEAR eea_2nd_last_row(int a, int b) {
    EEAR r1 = make_eear(1, 0, fmax(abs(a), abs(b)), 0);
    EEAR r2 = make_eear(0, 1, fmin(abs(a), abs(b)), 0);

    while (r2.r != 0) {
        int q = r1.r / r2.r;
        int r = r1.r % r2.r;
        int x = r1.x - r2.x * q;
        int y = r1.y - r2.y * q;

        r1 = r2;
        r2 = make_eear(x, y, r, q);
    }

    return r1;
}

int eea_gcd(int a, int b) {
    return eea_gcd_row(eea_2nd_last_row(a, b));
}

int eea_gcd_table(EEA_Table table) {
    return eea_gcd_row(list_at(table, table.size - 2, EEAR));
}

int eea_gcd_row(EEAR row) {
    return row.r;
}
