#include "main.h"

int main() {
  int status;
  status = BFGS();
  printf("status: %s\n", gsl_strerror(status));
}
