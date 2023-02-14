#include "main.h"

int main() {
  int status;
  status = SteepestDescend();
  printf("status: %s\n", gsl_strerror(status));
}
