#include "easylogging++.h"
#include "SJ_datareading.h"
#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"
#include "SJ_plotting.h"
#include "SJ_fitting.h"

#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TLine.h>  
#include <TLegend.h>
#include <TLatex.h>
#include <TVector.h>
#include <TMultiGraph.h>
// for RooFit
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooConstVar.h>
#include <RooMinuit.h>
#include <RooNLLVar.h>
#include <RooMinimizer.h>
#include <RooChi2Var.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooGenericPdf.h>
#include <RooExtendPdf.h>
// for product of two pdfs
#include <RooProdPdf.h>
// for gStyle
#include <TStyle.h>
#include <TF2.h>
