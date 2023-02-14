#ifndef _FUNC_H_
#define _FUNC_H_

#include <math.h>
#include <gsl/gsl_multimin.h>
double f(const gsl_vector *v, void* params);
#endif //_FUNC_H_