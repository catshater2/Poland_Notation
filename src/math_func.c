#include "math_func.h"

#include <math.h>
#include <stdio.h>

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) {
    if (b != 0) return a / b;
    printf("n/a\n");
    return 0;
}
double power(double base, double exponent) { return pow(base, exponent); }