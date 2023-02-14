#include "func.h"

double f(double x, void* params) {
  (void)(params); /* avoid unused parameter warning */
  return cos(x) - 2*sin(x);
}