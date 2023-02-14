#include "func.h"



double f(const gsl_vector* v, void* params) { 
  double x[10];
  unsigned i;
  for(i = 0; i < 10; i++) x[i] = gsl_vector_get(v, i);
  //x[1] = gsl_vector_get(v, 1);
  double p = *(double*)params; 
  
  double func = 0, aux;
  for(i = 1; i < 10; i++){
    aux = x[i-1]-x[i];
    func += (1+p)*(x[i-1]-3)*(x[i-1]-3) + aux*aux + exp(20*aux);
  }
  return func;
}

void df(const gsl_vector* v, void* params, gsl_vector *df) { 
  double x[10], dx[10];
  unsigned i;
  for(i = 0; i < 10; i++) x[i] = gsl_vector_get(v, i);
  //x[1] = gsl_vector_get(v, 1);
  double p = *(double*)params; 

  dx[0] = 0;
  
  double aux;
  for(i = 1; i < 10; i++){
    aux = x[i-1]-x[i];
    dx[i-1] += 2*(1+p)*(x[i-1]-3) + 2*aux +20*exp(20*aux);
    dx[i] = -2*(aux) - 20*exp(20*(aux));
  }
  
  for(i = 0; i < 10; i++) gsl_vector_set(df, i, dx[i]);
}

void ddf(const gsl_vector* x, void* params, double *F, gsl_vector *DF) { 
  *F = f(x, params);
  df(x, params, DF);
}


