#include "SteepestDescend.h"

int SteepestDescend() {
  // Function and derivative:  
  gsl_multimin_function_fdf F;
  F.n = 10; //dimension (number of variables) 
  F.f = &f; //function to optimize
  F.df = &df;
  F.fdf = &ddf;
  double p = 1.1;
  F.params = &p; //parameters of the function to optimitze
  unsigned n = 10;

  // Set starting point:
  double u[10] = {2.94637096, 3.01527542, 3.05485232, 3.11592101, 3.14543397, 3.42973416, 3.52414502, 3.63947264, 3.79563826, 5.21931803};
  gsl_vector* x;
  x = gsl_vector_alloc(n);
  gsl_vector_set_all(x,1.0);
  for(unsigned i =0; i<10; i++) gsl_vector_set(x, i, u[i]); // x[i] --> i-th component

  // Set initial step sizes (only Nelder-Mead; construction of initial simplex):
  //gsl_vector *ss;
  //ss = gsl_vector_alloc(n);  // ss = stepSizes
  //gsl_vector_set_all(ss, 1.0); // x[i]+ss[i] (i-th point)
  
  //initialize minimizer:
  const gsl_multimin_fdfminimizer_type* T;
  T = gsl_multimin_fdfminimizer_steepest_descent;
  gsl_multimin_fdfminimizer* s = NULL;
  printf("holis\n");
  s = gsl_multimin_fdfminimizer_alloc(T, F.n);
  printf("holis\n");
  gsl_multimin_fdfminimizer_set(s, &F, x, 0.01, 1e-4);
  
  // Iterations control:
  int status;
  int iter = 0;
  int max_iter = 1000;
  double epsabs = 1e-6, f_ant, tol = 1e-3; 
  
  printf("using %s method\n", gsl_multimin_fdfminimizer_name(s));
  printf("%10s","iter");
  for(unsigned i = 1; i< n+1; i++) printf("       x[%d]", i);
  printf(" %10s\n","f_min");
  printf("%10d",iter);
  for(unsigned i = 0; i < n; i++) printf(" %.8f", gsl_vector_get(x, i));
  printf("\n");
  
  do {
  
    iter++;
    status = gsl_multimin_fdfminimizer_iterate(s);

    if(status)
      break;

    //size = gsl_multimin_fminimizer_size(s);
    status = gsl_multimin_test_gradient(s->gradient, epsabs);  //("tol" is the radius of simplex from centroid)
    if(status == GSL_SUCCESS || fabs(f_ant - s->f) < tol) {   // =0 (the simplex size is smaller than the tolerance)
      printf("\n\nConverged at iteration: %d.\n\n", iter);
      status = GSL_SUCCESS;
    }
    
    printf("%10s","iter");
    for(unsigned i = 1; i< n+1; i++) printf("       x[%d]", i);
    printf(" %10s\n","f_min");
    printf("%10d",iter);
    for(unsigned i = 0; i < n; i++) printf(" %.8f", gsl_vector_get(s->x, i));
    printf("  %.10f\n", s->f); 
    f_ant = s->f;
    
    
  } while(status == GSL_CONTINUE && iter < max_iter);  
  
  if(status == GSL_CONTINUE)  // =-2 (the simplex size is still bigger than the tolerance)
    printf("Maximum iterations reached.\n");

  gsl_vector_free(x);
  //gsl_vector_free(ss);
  gsl_multimin_fdfminimizer_free(s);
  return status;
}

/*
### Definition of the types "gsl_multimin_function" and "gsl_multimin_function_fdf":
See manual GSL, chapter "Multidimensional Minimization".
The first for algorithms without derivatives.
The second for algorithms with derivatives, where we have to provide pointers
to both the function and the gradient.
*/
