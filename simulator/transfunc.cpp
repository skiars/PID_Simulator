#include "transfunc.h"
#include <math.h>

// transfer function init
TransFunc::TransFunc()
{
    z[1] = z[0] = 0;
}

// generate input value
double TransFunc::drivingFunction(double t)
{
    if (t < 0.3) {
        return 30;
    }
    if (t < 0.6) {
        return t * 100;
    }
    return sin(t * 10 * M_PI) * 20 + 70;
}

//*********************************************************
// IIR Filter:
//        b0 + b1 z^-1 + b2 z^-2
// -------------------------------------
//         1 - a1 z^-1 - a2 z^-2
//*********************************************************

// The IIR Filter cut-off frequency of these parameters
// is 40Hz, and the sampling rate is 1000Hz.
#define a1     -1.64746
#define a2      0.70090
#define b0      0.01336
#define b1      0.02672
#define b2      0.01336

// transfer function
double TransFunc::iteration(double x)
{
    double y;

    // IIR Filter
    x =      x - a1 * z[0] - a2 * z[1];
    y = b0 * x + b1 * z[0] + b2 * z[1];
    z[1] = z[0];
    z[0] = x;
    return y;
}

#undef a1
#undef a2
#undef b0
#undef b1
#undef b2
