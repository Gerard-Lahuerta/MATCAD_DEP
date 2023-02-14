#include "checkdf.h"

int main() {

  printf("verifying derivatives...\n");
  // Function and derivative:  
  gsl_multimin_function_fdf F;
  F.n = 8; //dimension (number of variables) 
  F.f = &f; //function to optimize
  F.df = &df; //gradient of function
  F.fdf = &ddf; //call to function and gradient together
  double p = 2;
  F.params = &p; //parameters of the function to optimitze

  // Point to check:
  double x[10]={0, 1, 0, 1, 0, 1, 0, 1, 0, 1}; 
  double y[10];
  
  gsl_vector* v;
  v = gsl_vector_alloc(10);
  for(int i=0; i<10; i++){ 
    gsl_vector_set(v, i, x[i]);  
  }

  gsl_vector* w;
  w = gsl_vector_alloc(10);
  for(int i=0; i<10; i++){ 
    gsl_vector_set(w, i, x[i]);  
  }
  
  gsl_vector* gradient;
  gradient = gsl_vector_alloc(10);
  F.df(v, F.params, gradient);
  printf("Partial derivatives:\n");
  for(int i=0; i<10; i++) {
    printf("[%d] %g\n", i+1, gsl_vector_get(gradient, i));
  }  
     
  double s=0.00001;
  printf("Approximations:\n");  
  for(int i=0; i<10; i++){
    double fx = F.f(v, F.params);
    y[i] = x[i] + s;
    gsl_vector_set(w, i, y[i]);
    double fw = F.f(w, F.params);
    printf("[%d] %g\n", i+1, (fw-fx)/s);
    gsl_vector_set(w, i, x[i]);
    y[i]=x[i];
  } 
  
  return 0;
}
