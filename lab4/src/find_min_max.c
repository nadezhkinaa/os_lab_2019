#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

    int elem;

    if (begin > end) {
     
        elem = begin;
        begin = end;
        end = elem;
    }

    for (int i = begin; i < end; i++) {
        if (array[i] > min_max.max) {
            min_max.max = array[i];
        }
        if (array[i] < min_max.min) {
            min_max.min = array[i];
        }
    }

  // your code here
  return min_max;
}
