#include "main.h"

int main() {
  int status;
  status = PR();
  printf("status: %s\n", gsl_strerror(status));
}
