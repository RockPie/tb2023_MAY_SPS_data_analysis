#include "easylogging++.h"
#include "SJ_datareading.h"
#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"
#include "SJ_plotting.h"
#include "SJ_fitting.h"
#include "SJ_cluster.h"

#include <TH1.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TLine.h>  
#include <TLegend.h>
#include <TLatex.h>
#include <TVector.h>
#include <TMultiGraph.h>
// for multi-threading
#include <TThread.h>
// for TThreadData
#include <TROOT.h>
// for product of two pdfs
#include <RooProdPdf.h>
// for gStyle
#include <TStyle.h>
#include <TF2.h>

// for getopt
#include <unistd.h>
