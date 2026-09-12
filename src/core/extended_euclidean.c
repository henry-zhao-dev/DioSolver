#include <diosolver/extended_euclidean.h>
#include <diosolver/common.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

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

void test_eea_table(void) {
    EEA_Table table;
    table = eea_table(0, 5);
    assert(equal_eea_table(table,
        make_list((EEAR[]) {
            make_eear(1, 0, 5, 0),
            make_eear(0, 1, 0, 0),
        }, 2)));
    list_free(table);

    table = eea_table(-5, -1);
    assert(equal_eea_table(table,
        make_list((EEAR[]) {
            make_eear(1, 0, 5, 0),
            make_eear(0, 1, 1, 0),
            make_eear(1, -5, 0, 5),
        }, 3)));
    list_free(table);

    table = eea_table(5, 5);
    assert(equal_eea_table(table,
        make_list((EEAR[]) {
            make_eear(1, 0, 5, 0),
            make_eear(0, 1, 5, 0),
            make_eear(1, -1, 0, 1),
        }, 3)));
    list_free(table);

    table = eea_table(5, -7);
    assert(equal_eea_table(table,
        make_list((EEAR[]) {
            make_eear(1, 0, 7, 0),
            make_eear(0, 1, 5, 0),
            make_eear(1, -1, 2, 1),
            make_eear(-2, 3, 1, 2),
            make_eear(5, -7, 0, 2),
        }, 5)));
    list_free(table);

    table = eea_table(1386, 322);
    assert(equal_eea_table(table,
        make_list((EEAR[]) {
            make_eear(1, 0, 1386, 0),
            make_eear(0, 1, 322, 0),
            make_eear(1, -4, 98, 4),
            make_eear(-3, 13, 28, 3),
            make_eear(10, -43, 14, 3),
            make_eear(-23, 99, 0, 2),
        }, 6)));
    list_free(table);

    table = eea_table(-2172, 423);
    assert(equal_eea_table(table,
        make_list((EEAR[]) {
            make_eear(1, 0, 2172, 0),
            make_eear(0, 1, 423, 0),
            make_eear(1, -5, 57, 5),
            make_eear(-7, 36, 24, 7),
            make_eear(15, -77, 9, 2),
            make_eear(-37, 190, 6, 2),
            make_eear(52, -267, 3, 1),
            make_eear(-141, 724, 0, 2),
        }, 8)));
    list_free(table);
}

void test_eea_2nd_last_row(void) {
    assert(equal_eear(
        eea_2nd_last_row(0, 5),
        make_eear(1, 0, 5, 0)));

    assert(equal_eear(
        eea_2nd_last_row(-5, -1),
        make_eear(0, 1, 1, 0)));

    assert(equal_eear(
        eea_2nd_last_row(5, 5),
        make_eear(0, 1, 5, 0)));

    assert(equal_eear(
        eea_2nd_last_row(5, -7),
        make_eear(-2, 3, 1, 2)));

    assert(equal_eear(
        eea_2nd_last_row(1386, 322),
        make_eear(10, -43, 14, 3)));

    assert(equal_eear(
        eea_2nd_last_row(-2172, 423),
        make_eear(52, -267, 3, 1)));
}

void test_eea_gcd(void) {
    assert(eea_gcd(0, 5) == 5);
    assert(eea_gcd(-5, 0) == 5);
    assert(eea_gcd(-5, -1) == 1);
    assert(eea_gcd(5, 5) == 5);
    assert(eea_gcd(5, -7) == 1);
    assert(eea_gcd(1386, 322) == 14);
    assert(eea_gcd(-2172, 423) == 3);

    EEA_Table table;
    table = eea_table(1386, 322);
    assert(eea_gcd_table(table) == 14);
    list_free(table);

    table = eea_table(-2172, 423);
    assert(eea_gcd_table(table) == 3);
    list_free(table);

    assert(eea_gcd_row(make_eear(10, -43, 14, 3)) == 14);
    assert(eea_gcd_row(make_eear(52, 267, 3, 1)) == 3);
}

void test_extended_euclidean(void) {
    test_eea_table();
    test_eea_2nd_last_row();
    test_eea_gcd();
}
