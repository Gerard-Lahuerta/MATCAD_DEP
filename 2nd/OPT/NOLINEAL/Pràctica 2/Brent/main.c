#include "main.h"

int main() {
  int status;
  double x;
  printf("Introduce a initial guess: ");
  scanf("%lf",&x);
  status = brent(x);
  printf("status: %s\n", gsl_strerror(status));
}
