#include "func.h"



//double f(double x, void* params) {
//  (void)(params); /* avoid unused parameter warning */
//  return cos(x) + sin(x);
//}

//double df(double x, void* params) {
//  (void)(params); /* avoid unused parameter warning */
//  return -sin(x) + cos(x);
//}

//double ddf(double x, void* params) {
//  (void)(params); /* avoid unused parameter warning */
//  return -cos(x) - sin(x);
//}



// (exercise) Change function to f(x) = 4*cos(x) + 2*sin(5*x+1)

double f(double x, void* params) {
  (void)(params); /* avoid unused parameter warning */
  return 4*cos(x) + 2*sin(5*x+1);
}

double df(double x, void* params) {
  (void)(params); /* avoid unused parameter warning */
  return -4*sin(x) + 10*cos(5*x+1);
}

double ddf(double x, void* params) {
  (void)(params); /* avoid unused parameter warning */
  return -4*cos(x) - 50*sin(5*x+1);
}
