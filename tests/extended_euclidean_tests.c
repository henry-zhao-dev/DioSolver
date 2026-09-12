#include <assert.h>
#include <diosolver/extended_euclidean.h>
#include <stdio.h>
#include <stdlib.h>

static void assert_eea_table(const EEA_Table *table, const EEAR expected[], size_t expected_size) {
    assert(calist_size(table) == expected_size);
    for (size_t i = 0; i < expected_size; ++i) {
        const EEAR actual = *(const EEAR *)calist_get(table, i);
        assert(equal_eear(actual, expected[i]));
    }
}

static void test_eea_table(void) {
    EEA_Table *table = eea_table(0, 5);
    assert_eea_table(table,
                     (EEAR[]){
                         make_eear(1, 0, 5, 0),
                         make_eear(0, 1, 0, 0),
                     },
                     2);
    EEA_Table *copy = calist_dup(table);
    assert(equal_eea_table(table, copy));
    calist_destroy(copy);
    calist_destroy(table);

    table = eea_table(-5, -1);
    assert_eea_table(table,
                     (EEAR[]){
                         make_eear(1, 0, 5, 0),
                         make_eear(0, 1, 1, 0),
                         make_eear(1, -5, 0, 5),
                     },
                     3);
    calist_destroy(table);

    table = eea_table(5, 5);
    assert_eea_table(table,
                     (EEAR[]){
                         make_eear(1, 0, 5, 0),
                         make_eear(0, 1, 5, 0),
                         make_eear(1, -1, 0, 1),
                     },
                     3);
    calist_destroy(table);

    table = eea_table(5, -7);
    assert_eea_table(table,
                     (EEAR[]){
                         make_eear(1, 0, 7, 0),
                         make_eear(0, 1, 5, 0),
                         make_eear(1, -1, 2, 1),
                         make_eear(-2, 3, 1, 2),
                         make_eear(5, -7, 0, 2),
                     },
                     5);
    calist_destroy(table);

    table = eea_table(1386, 322);
    assert_eea_table(table,
                     (EEAR[]){
                         make_eear(1, 0, 1386, 0),
                         make_eear(0, 1, 322, 0),
                         make_eear(1, -4, 98, 4),
                         make_eear(-3, 13, 28, 3),
                         make_eear(10, -43, 14, 3),
                         make_eear(-23, 99, 0, 2),
                     },
                     6);
    calist_destroy(table);

    table = eea_table(-2172, 423);
    assert_eea_table(table,
                     (EEAR[]){
                         make_eear(1, 0, 2172, 0),
                         make_eear(0, 1, 423, 0),
                         make_eear(1, -5, 57, 5),
                         make_eear(-7, 36, 24, 7),
                         make_eear(15, -77, 9, 2),
                         make_eear(-37, 190, 6, 2),
                         make_eear(52, -267, 3, 1),
                         make_eear(-141, 724, 0, 2),
                     },
                     8);
    calist_destroy(table);
}

static void test_eea_2nd_last_row(void) {
    assert(equal_eear(eea_2nd_last_row(0, 5), make_eear(1, 0, 5, 0)));

    assert(equal_eear(eea_2nd_last_row(-5, -1), make_eear(0, 1, 1, 0)));

    assert(equal_eear(eea_2nd_last_row(5, 5), make_eear(0, 1, 5, 0)));

    assert(equal_eear(eea_2nd_last_row(5, -7), make_eear(-2, 3, 1, 2)));

    assert(equal_eear(eea_2nd_last_row(1386, 322), make_eear(10, -43, 14, 3)));

    assert(equal_eear(eea_2nd_last_row(-2172, 423), make_eear(52, -267, 3, 1)));
}

static void test_eea_gcd(void) {
    assert(eea_gcd(0, 5) == 5);
    assert(eea_gcd(-5, 0) == 5);
    assert(eea_gcd(-5, -1) == 1);
    assert(eea_gcd(5, 5) == 5);
    assert(eea_gcd(5, -7) == 1);
    assert(eea_gcd(1386, 322) == 14);
    assert(eea_gcd(-2172, 423) == 3);

    EEA_Table *table = eea_table(1386, 322);
    assert(eea_gcd_table(table) == 14);
    calist_destroy(table);

    table = eea_table(-2172, 423);
    assert(eea_gcd_table(table) == 3);
    calist_destroy(table);

    assert(eea_gcd_row(make_eear(10, -43, 14, 3)) == 14);
    assert(eea_gcd_row(make_eear(52, 267, 3, 1)) == 3);
}

int main(void) {
    test_eea_table();
    test_eea_2nd_last_row();
    test_eea_gcd();
    puts("DioSolver extended Euclidean tests passed.");
    return 0;
}
