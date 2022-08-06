#include "bf.h"

#ifndef BN_NO_STDMATH
#include <math.h>
#else
#ifdef BN_CUSTOM_FMATH
//from https://stackoverflow.com/questions/70382318/ex-without-math-h
double fabs(double x) {
    if(x >= 0){
        return x;
    } else {
        return -x;
    }
}
double exp(double x) {
    double a = 1.0, e = 0;
    int invert = x<0;
    x = fabs(x);
    for (int n = 1; e != e + a ; ++n) {
        e += a;
        a = a * x / n;
    }
    return invert ? 1/e : e;
}
//from https://stackoverflow.com/a/64896598
#define EULER_CONST 2.718281828459045235
#define TAYLOR_ITERATIONS 20
double log(double x) {
    // Trap illegal values
    if (x <= 0) {
        return 0.0/0.0;  // NaN
    }
    
    // Confine x to a sensible range
    int power_adjust = 0;
    while (x > 1.0) {
        x /= EULER_CONST;
        power_adjust++;
    }
    while (x < .25) {
        x *= EULER_CONST;
        power_adjust--;
    }
    
    // Now use the Taylor series to calculate the logarithm
    x -= 1.0;
    double t = 0.0, s = 1.0, z = x;
    for (int k=1; k<=TAYLOR_ITERATIONS; k++) {
        t += z * s / k;
        z *= x;
        s = -s;
    }
    
    // Combine the result with the power_adjust value and return
    return t + power_adjust;
}
//from https://stackoverflow.com/a/14104864
double pow(double x, double y) {
    return exp(log(x)*y);
}
double fmax(double a, double b){
	if(a > b){
		return(a);
	}else{
		return(b);
	}
}
#endif
#endif

#if (!defined(BN_NO_STDMATH)) || defined(BN_CUSTOM_FMATH) || defined(BN_NATIVE_FMATH)
#define BF_EPSILON 10e-12
int bf_closeWithinEpsilon(double d1, double d2){
	return (fabs(d1-d2) < BF_EPSILON * fmax(fabs(d1), fabs(d2))); //epsilon is 0.1 for now
}
#endif

#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bf_.c"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bf_.c"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD
