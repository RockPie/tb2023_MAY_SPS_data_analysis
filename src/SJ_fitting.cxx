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

double SJFunc::dual_gaussian2D (double *x, double *par){
    double xc   = par[0]; // mean x
    double yc   = par[1]; // mean y
    double sigmaX   = par[2]; // standard deviation x
    double sigmaY   = par[3]; // standard deviation y
    double xc2   = par[4]; // mean x
    double yc2   = par[5]; // mean y
    double sigmaX2   = par[6]; // standard deviation x
    double sigmaY2   = par[7]; // standard deviation y
    return par[8] * exp(-0.5 * ((x[0] - xc) * (x[0] - xc) / (sigmaX * sigmaX) + (x[1] - yc) * (x[1] - yc) / (sigmaY * sigmaY))) + par[9] * exp(-0.5 * ((x[0] - xc2) * (x[0] - xc2) / (sigmaX2 * sigmaX2) + (x[1] - yc2) * (x[1] - yc2) / (sigmaY2 * sigmaY2)));
}

double SJFunc::energy_resolution_func (double *x, double *par){
    double p0 = par[0];
    double p1 = par[1];
    double p2 = par[2];
    return sqrt(pow((p0/sqrt(x[0])),2) + pow(p1/(x[0]),2) + pow(p2,2));
}