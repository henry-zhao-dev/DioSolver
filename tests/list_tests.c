#include <diosolver/list.h>

#include <assert.h>
#include <stdio.h>

static void test_make_list(void) {
    int *values = malloc(2 * sizeof(*values));
    List list = make_list(values, 2);

    assert(list.arr == values);
    assert(list.size == 2);

    list_set(list, 0, 13, int);
    list_set(list, 1, 37, int);
    assert(list_at(list, 0, int) == 13);
    assert(list_at(list, 1, int) == 37);

    list_free(list);
    assert(list.size == 0);
}

static void test_list_append(void) {
    List list = list_init_empty();

    assert(list.size == 0);
    list_append(list, 13, int);
    list_append(list, 37, int);

    assert(list.size == 2);
    assert(list_at(list, 0, int) == 13);
    assert(list_at(list, 1, int) == 37);

    list_free(list);
}

int main(void) {
    test_make_list();
    test_list_append();
    puts("DioSolver list tests passed.");
    return 0;
}
