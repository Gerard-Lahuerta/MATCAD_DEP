#ifndef _FUNC_H_
#define _FUNC_H_

#include <math.h>
#include <gsl/gsl_multimin.h>
double f(const gsl_vector *v, void* params);
void df(const gsl_vector *v, void* params, gsl_vector *df);
void ddf(const gsl_vector* x, void* params, double *F, gsl_vector *DF);
#endif //_FUNC_H_
