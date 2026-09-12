#include <diosolver/diophantine.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void test_make_solution(void) {
    const Solution solution = make_solution(13, -37);

    assert(solution.x == 13);
    assert(solution.y == -37);
    assert(solution.exist);
    assert(!NO_SOLN.exist);
}

static void test_make_lde(void) {
    LDE lde = make_lde(9, 5, 137);

    assert(lde.a == 9);
    assert(lde.b == 5);
    assert(lde.c == 137);
    assert(equal_interval(lde.xi, REAL));
    assert(equal_interval(lde.yi, REAL));

    Interval xi = make_interval(0, 10, false, false);
    Interval yi = make_interval(-10, 10, false, false);
    lde = make_lde_in(9, 5, 137, xi, yi);

    assert(equal_interval(lde.xi, xi));
    assert(equal_interval(lde.yi, yi));
}

static void assert_solution_for_lde(LDE lde, Solution solution) {
    assert(solution.exist);
    assert(lde.a * solution.x + lde.b * solution.y == lde.c);
}

static void test_eea_lde(void) {
    LDE lde = make_lde(9, 5, 137);
    assert_solution_for_lde(lde, eea_lde(lde));

    EEA_Table *table = eea_table(lde.a, lde.b);
    assert_solution_for_lde(lde, eea_lde_table(lde, table));
    assert_solution_for_lde(
        lde, eea_lde_row(lde, eea_2nd_last_row(lde.a, lde.b)));
    calist_destroy(table);

    lde = make_lde(6, 4, 5);
    assert(!eea_lde(lde).exist);

    table = eea_table(lde.a, lde.b);
    assert(!eea_lde_table(lde, table).exist);
    calist_destroy(table);
}

static void test_lde_result(void) {
    calist *result = lde_result(make_lde(0, 0, 0));

    assert(calist_size(result) > 0);
    for (size_t i = 0; i < calist_size(result); ++i) {
        assert(calist_get(result, i) != NULL);
    }
    calist_destroy(result);
}

int main(void) {
    test_make_solution();
    test_make_lde();
    test_eea_lde();
    test_lde_result();
    puts("DioSolver diophantine tests passed.");
    return 0;
}
