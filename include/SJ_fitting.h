// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 07-06-2023
// ***********************************************************

#pragma once

#define HG_RELATIVE_GAIN    9.0
#define SUBSTITUE_THRESHOLD 4096-100

// * Roofit Gaussian 2D
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooPlot.h>
#include <RooFitResult.h>
#include <RooArgList.h>
#include <RooArgSet.h>
#include <RooAddPdf.h>
#include <RooProdPdf.h>
#include <RooFormulaVar.h>
#include <RooAbsReal.h>

#include "easylogging++.h"

namespace SJFunc{
    double gaussian2D (double *x, double *par);
    double gaussian2D_c (double *x, double *par);
    double dual_gaussian2D (double *x, double *par);
    double triple_gaussian2D (double *x, double *par);
    double energy_resolution_func (double *x, double *par);
}; // namespace SJFunc