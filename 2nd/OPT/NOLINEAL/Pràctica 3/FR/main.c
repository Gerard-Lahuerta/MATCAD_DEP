#include "main.h"

int main() {
  int status;
  status = FR();
  printf("status: %s\n", gsl_strerror(status));
}
