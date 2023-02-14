#include "newton.h"
#include "math.h"

int newton(double pre_x) {

  struct Function {
    double (*f)(double x, void* params);
    double (*df)(double x, void* params);
    double (*ddf)(double x, void* params);
    void* params;
  };
  struct Function F;

  F.f = &f;
  F.df = &df;
  F.ddf = &ddf;
  F.params = NULL;

  // double x_lower = 0.0, x_upper = 6.0; /* initial interval, unused */
  // (exercise) make initial guess an input of the user, collected in
  // function main() and passed as argument to function newton()
  double x_min = pre_x; /* initial guess */

  int iter = 0;
  int max_iter = 10;
  const double tol = 10e-12;
  double f_min = F.f(x_min, F.params), pre_f = 0, ddf = F.ddf(x_min, F.params);;
  // double epsabs = 0.001;  /* unsued */
  // double epsrel = 0; /* unsued */

  printf("using Newton method\n");
  printf("%5s %15s %15s\n", "iter", "min", "f(x)");
  printf("%5d %15f %.15f \n", iter, x_min, f_min);

  do {
    iter++;
   
    x_min = pre_x - F.df(x_min, F.params) / ddf;  // could be extracted to a function Newton_iteration();


    // (exercise) compare consecutives values of x_min and raise a flag if difference is < 10^(-12)
    // and in that case print "Converged" and finish the iterations
    if( fabs(pre_x-x_min) < tol){
    	printf("Converged, x_min --> %.15f\n",x_min);
    	return 0;
    }
    pre_x=x_min;

    // (exercise) compare consecutive function values and raise a flag if difference is < 10^(-12),
    // and in that case also print "Converged" and finish the iterations
    f_min = F.f(x_min, F.params);
    if( fabs(pre_f-f_min) < tol){
    	printf("Converged, x_min --> %.15f\n",x_min);
    	return 0;
    }
    pre_f = f_min;
    
    // (exercise) If the second derivative is negative, stop the iterations,
    // print an informative message and change status to 1
    ddf = F.ddf(x_min, F.params);
    if(ddf<0){
    	printf("Error: second derivative negative\n");
    	return 1;
    }

    // (exercise) add the value of the function at every iteration to this printf
    printf("%5d %.15f %.15f\n", iter, x_min, f_min);
    
  } while(iter < max_iter);
  
  // (exercise) if maximum iterations are reached, print an informative message
  // and change status to 2
  printf("Error: maximim iterations are reached\n");
  return 2;
}
