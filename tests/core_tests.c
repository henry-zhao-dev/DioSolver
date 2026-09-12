#include <diosolver/common.h>
#include <diosolver/extended_euclidean.h>
#include <diosolver/inequality.h>
#include <diosolver/interval.h>

#include <stdio.h>

int main(void) {
    test_common();
    test_extended_euclidean();
    test_interval();
    test_inequality();

    puts("DioSolver core tests passed.");
    return 0;
}
