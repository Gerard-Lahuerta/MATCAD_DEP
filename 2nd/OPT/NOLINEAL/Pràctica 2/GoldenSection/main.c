#include "main.h"

int main() {
  int status;
  double x;
  printf("Introduce a initial guess: ");
  scanf("%lf",&x);
  status = goldenSection(x);
  printf("status: %s\n", gsl_strerror(status));
}
