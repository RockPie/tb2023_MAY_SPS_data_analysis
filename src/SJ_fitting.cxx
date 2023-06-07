#include "SJ_fitting.h"

double SJFunc::gaussian2D (double *x, double *par){
    double xc   = par[0]; // mean x
    double yc   = par[1]; // mean y
    double sigmaX   = par[2]; // standard deviation x
    double sigmaY   = par[3]; // standard deviation y
    return par[4] * exp(-0.5 * ((x[0] - xc) * (x[0] - xc) / (sigmaX * sigmaX) + (x[1] - yc) * (x[1] - yc) / (sigmaY * sigmaY)));
}

double SJFunc::gaussian2D_c (double *x, double *par){
    double xc   = par[0]; // mean x
    double yc   = par[1]; // mean y
    double sigmaX   = par[2]; // standard deviation x
    double sigmaY   = par[3]; // standard deviation y
    double padastal = par[4]; // padastal
    return par[5] * exp(-0.5 * ((x[0] - xc) * (x[0] - xc) / (sigmaX * sigmaX) + (x[1] - yc) * (x[1] - yc) / (sigmaY * sigmaY))) + padastal;
}