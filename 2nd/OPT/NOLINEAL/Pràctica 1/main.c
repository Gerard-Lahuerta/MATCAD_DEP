#include "main.h"

int main() {
  int status;
  double x0;
  
  printf("Introduce the initial guess: ");
  scanf("%15lf",&x0);
  status = newton(x0);
  printf("status: %d\n", status);
}
