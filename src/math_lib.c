#include "math_lib.h"

int upper_lower_limit(int val, int u_limit, int l_limit) {
    if (val < l_limit) {
        val = l_limit;
    } else if (val > u_limit) {
        val = u_limit;
    }
    return val;
}

int abs(int j) {
  return j < 0 ? -j : j;
}