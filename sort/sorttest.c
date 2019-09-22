#include <sys/time.h>

#define logsize 22
#define SIZE (1<<logsize);
#define SORT_CMP(x, y) (x - y)
#define MAX(x,y) (((x) > (y) ? (x) : (y)))
#define MIN(x,y) (((x) < (y) ? (x) : (y)))
#define SORT_CSWAP(x, y) {SORT_TYPE _sort_swap_temp = MAX((x), (y)); (x) = MIN((x),(y)); (y) = _sort_swap_temp;}
static __inline int simple_cmp(const void *a, const void *b) {
  const int64_t da = *((const int64_t *) a);
  const int64_t db = *((const int64_t *) b);
  return (da < db) ? -1 : (da == db) ? 0 : 1;
}

#include "sort.h"
int main(){
    qsort(dst, SIZE, sizeof(int64_t), simple_cmp);
    heapsort(dst, SIZE, sizeof(int64_t), simple_cmp);
    mergesort(dst, SIZE, sizeof(int64_t), simple_cmp);
    sorter_quick_sort(dst, SIZE);
    sorter_selection_sort(dst, SIZE);
    sorter_bubble_sort(dst, SIZE);
    sorter_bitonic_sort(dst, SIZE);
    sorter_merge_sort(dst, SIZE);
    sorter_binary_insertion_sort(dst, SIZE);
    sorter_heap_sort(dst, SIZE);
    sorter_shell_sort(dst, SIZE);
    sorter_tim_sort(dst, SIZE);
    sorter_merge_sort_in_place(dst, SIZE);
    sorter_grail_sort(dst, SIZE);
    sorter_sqrt_sort(dst, SIZE);
    sorter_rec_stable_sort(dst, SIZE);
    sorter_grail_sort_dyn_buffer(dst, SIZE);
}

