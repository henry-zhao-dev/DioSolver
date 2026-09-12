#include <diosolver/list.h>

List make_list(void *arr, int size) {
    return (List) {arr, size};
}
