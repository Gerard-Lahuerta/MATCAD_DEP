#include "main.h"

int main() {
  int status;
  status = NelderMead();
  printf("status: %s\n", gsl_strerror(status));
}
