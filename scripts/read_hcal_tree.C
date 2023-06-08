#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include "hcaltreehelpers.h"

using namespace std;

void read_hcal_tree(TString inputfile = "../Rootified/Run2797.root")
{

    TFile *intput = new TFile(inputfile.Data(), "READ");
    TTree *tree = (TTree *)intput->Get("hcaldata");

    // ─── SETTING BRANCH ADDRESSES ────────────────────────────────────────────────
    setBranchAddresses(tree); // see hcaltreehelpers.h

    Long64_t nentries = tree->GetEntries();
    cout << "-> nTriggers = " << nentries << endl;
    TFile *output = new TFile("R2797hist.root", "RECREATE");
//
// pedestals for all channels
//

    int Ped[4][64];
    int PedLG[4][64];

    Ped[0][0]=	41;
    Ped[0][1]=	36;
    Ped[0][2]=	37;
    Ped[0][3]=	39;
    Ped[0][4]=	38;
    Ped[0][5]=	37;
    Ped[0][6]=	38;
    Ped[0][7]=	38;
    Ped[0][8]=	38;
    Ped[0][9]=	39;
    Ped[0][10]=	37;
    Ped[0][11]=	39;
    Ped[0][12]=	39;
    Ped[0][13]=	44;
    Ped[0][14]=	45;
    Ped[0][15]=	38;
    Ped[0][16]=	39;
    Ped[0][17]=	42;
    Ped[0][18]=	45;
    Ped[0][19]=	44;
    Ped[0][20]=	37;
    Ped[0][21]=	41;
    Ped[0][22]=	43;
    Ped[0][23]=	46;
    Ped[0][24]=	45;
    Ped[0][25]=	38;
    Ped[0][26]=	38;
    Ped[0][27]=	38;
    Ped[0][28]=	38;
    Ped[0][29]=	43;
    Ped[0][30]=	44;
    Ped[0][31]=	44;
    Ped[0][32]=	43;
    Ped[0][33]=	40;
    Ped[0][34]=	42;
    Ped[0][35]=	45;
    Ped[0][36]=	34;
    Ped[0][37]=	44;
    Ped[0][38]=	41;
    Ped[0][39]=	32;
    Ped[0][40]=	42;
    Ped[0][41]=	38;
    Ped[0][42]=	40;
    Ped[0][43]=	42;
    Ped[0][44]=	43;
    Ped[0][45]=	43;
    Ped[0][46]=	40;
    Ped[0][47]=	40;
    Ped[0][48]=	38;
    Ped[0][49]=	42;
    Ped[0][50]=	43;
    Ped[0][51]=	42;
    Ped[0][52]=	41;
    Ped[0][53]=	40;
    Ped[0][54]=	38;
    Ped[0][55]=	37;
    Ped[0][56]=	42;
    Ped[0][57]=	40;
    Ped[0][58]=	39;
    Ped[0][59]=	38;
    Ped[0][60]=	38;
    Ped[0][61]=	37;
    Ped[0][62]=	37;
    Ped[0][63]=	32;
    Ped[1][0]=	25;
    Ped[1][1]=	25;
    Ped[1][2]=	24;
    Ped[1][3]=	27;
    Ped[1][4]=	23;
    Ped[1][5]=	24;
    Ped[1][6]=	25;
    Ped[1][7]=	26;
    Ped[1][8]=	26;
    Ped[1][9]=	24;
    Ped[1][10]=	25;
    Ped[1][11]=	25;
    Ped[1][12]=	24;
    Ped[1][13]=	26;
    Ped[1][14]=	25;
    Ped[1][15]=	25;
    Ped[1][16]=	26;
    Ped[1][17]=	31;
    Ped[1][18]=	30;
    Ped[1][19]=	26;
    Ped[1][20]=	25;
    Ped[1][21]=	31;
    Ped[1][22]=	30;
    Ped[1][23]=	31;
    Ped[1][24]=	25;
    Ped[1][25]=	26;
    Ped[1][26]=	25;
    Ped[1][27]=	28;
    Ped[1][28]=	27;
    Ped[1][29]=	30;
    Ped[1][30]=	30;
    Ped[1][31]=	29;
    Ped[1][32]=	17;
    Ped[1][33]=	26;
    Ped[1][34]=	25;
    Ped[1][35]=	29;
    Ped[1][36]=	28;
    Ped[1][37]=	28;
    Ped[1][38]=	26;
    Ped[1][39]=	31;
    Ped[1][40]=	28;
    Ped[1][41]=	28;
    Ped[1][42]=	27;
    Ped[1][43]=	31;
    Ped[1][44]=	28;
    Ped[1][45]=	27;
    Ped[1][46]=	30;
    Ped[1][47]=	30;
    Ped[1][48]=	29;
    Ped[1][49]=	30;
    Ped[1][50]=	29;
    Ped[1][51]=	29;
    Ped[1][52]=	26;
    Ped[1][53]=	27;
    Ped[1][54]=	29;
    Ped[1][55]=	29;
    Ped[1][56]=	27;
    Ped[1][57]=	28;
    Ped[1][58]=	27;
    Ped[1][59]=	27;
    Ped[1][60]=	26;
    Ped[1][61]=	25;
    Ped[1][62]=	21;
    Ped[1][63]=	24;
    Ped[2][0]=	25;
    Ped[2][1]=	20;
    Ped[2][2]=	21;
    Ped[2][3]=	21;
    Ped[2][4]=	22;
    Ped[2][5]=	21;
    Ped[2][6]=	21;
    Ped[2][7]=	21;
    Ped[2][8]=	22;
    Ped[2][9]=	20;
    Ped[2][10]=	23;
    Ped[2][11]=	21;
    Ped[2][12]=	21;
    Ped[2][13]=	21;
    Ped[2][14]=	23;
    Ped[2][15]=	21;
    Ped[2][16]=	21;
    Ped[2][17]=	21;
    Ped[2][18]=	25;
    Ped[2][19]=	26;
    Ped[2][20]=	20;
    Ped[2][21]=	21;
    Ped[2][22]=	24;
    Ped[2][23]=	26;
    Ped[2][24]=	26;
    Ped[2][25]=	22;
    Ped[2][26]=	22;
    Ped[2][27]=	21;
    Ped[2][28]=	21;
    Ped[2][29]=	26;
    Ped[2][30]=	27;
    Ped[2][31]=	25;
    Ped[2][32]=	30;
    Ped[2][33]=	20;
    Ped[2][34]=	21;
    Ped[2][35]=	21;
    Ped[2][36]=	24;
    Ped[2][37]=	25;
    Ped[2][38]=	26;
    Ped[2][39]=	24;
    Ped[2][40]=	26;
    Ped[2][41]=	22;
    Ped[2][42]=	23;
    Ped[2][43]=	26;
    Ped[2][44]=	26;
    Ped[2][45]=	27;
    Ped[2][46]=	24;
    Ped[2][47]=	25;
    Ped[2][48]=	24;
    Ped[2][49]=	24;
    Ped[2][50]=	26;
    Ped[2][51]=	25;
    Ped[2][52]=	25;
    Ped[2][53]=	24;
    Ped[2][54]=	24;
    Ped[2][55]=	26;
    Ped[2][56]=	24;
    Ped[2][57]=	26;
    Ped[2][58]=	23;
    Ped[2][59]=	24;
    Ped[2][60]=	21;
    Ped[2][61]=	24;
    Ped[2][62]=	18;
    Ped[2][63]=	19;
    Ped[3][0]=	24;
    Ped[3][1]=	20;
    Ped[3][2]=	21;
    Ped[3][3]=	21;
    Ped[3][4]=	20;
    Ped[3][5]=	21;
    Ped[3][6]=	20;
    Ped[3][7]=	20;
    Ped[3][8]=	20;
    Ped[3][9]=	21;
    Ped[3][10]=	20;
    Ped[3][11]=	20;
    Ped[3][12]=	22;
    Ped[3][13]=	24;
    Ped[3][14]=	27;
    Ped[3][15]=	20;
    Ped[3][16]=	21;
    Ped[3][17]=	23;
    Ped[3][18]=	26;
    Ped[3][19]=	26;
    Ped[3][20]=	20;
    Ped[3][21]=	21;
    Ped[3][22]=	22;
    Ped[3][23]=	28;
    Ped[3][24]=	28;
    Ped[3][25]=	20;
    Ped[3][26]=	19;
    Ped[3][27]=	21;
    Ped[3][28]=	23;
    Ped[3][29]=	26;
    Ped[3][30]=	26;
    Ped[3][31]=	27;
    Ped[3][32]=	26;
    Ped[3][33]=	22;
    Ped[3][34]=	21;
    Ped[3][35]=	25;
    Ped[3][36]=	27;
    Ped[3][37]=	27;
    Ped[3][38]=	24;
    Ped[3][39]=	26;
    Ped[3][40]=	24;
    Ped[3][41]=	25;
    Ped[3][42]=	26;
    Ped[3][43]=	28;
    Ped[3][44]=	29;
    Ped[3][45]=	26;
    Ped[3][46]=	25;
    Ped[3][47]=	27;
    Ped[3][48]=	27;
    Ped[3][49]=	28;
    Ped[3][50]=	26;
    Ped[3][51]=	26;
    Ped[3][52]=	26;
    Ped[3][53]=	24;
    Ped[3][54]=	23;
    Ped[3][55]=	26;
    Ped[3][56]=	22;
    Ped[3][57]=	27;
    Ped[3][58]=	27;
    Ped[3][59]=	25;
    Ped[3][60]=	25;
    Ped[3][61]=	23;
    Ped[3][62]=	24;
    Ped[3][63]=	19;
    
    PedLG[0][0]=	41;
    PedLG[0][1]=	41;
    PedLG[0][2]=	42;
    PedLG[0][3]=	41;
    PedLG[0][4]=	39;
    PedLG[0][5]=	37;
    PedLG[0][6]=	40;
    PedLG[0][7]=	39;
    PedLG[0][8]=	39;
    PedLG[0][9]=	36;
    PedLG[0][10]=	38;
    PedLG[0][11]=	38;
    PedLG[0][12]=	38;
    PedLG[0][13]=	36;
    PedLG[0][14]=	34;
    PedLG[0][15]=	38;
    PedLG[0][16]=	38;
    PedLG[0][17]=	35;
    PedLG[0][18]=	35;
    PedLG[0][19]=	35;
    PedLG[0][20]=	37;
    PedLG[0][21]=	37;
    PedLG[0][22]=	37;
  PedLG[0][23]=	35;
 PedLG[0][24]=	34;
  PedLG[0][25]=	38;
 PedLG[0][26]=	38;
  PedLG[0][27]=	38;
 PedLG[0][28]=	36;
 PedLG[0][29]=	36;
 PedLG[0][30]=	35;
 PedLG[0][31]=	35;
 PedLG[0][32]=	39;
 PedLG[0][33]=	37;
 PedLG[0][34]=	37;
 PedLG[0][35]=	37;
 PedLG[0][36]=	38;
 PedLG[0][37]=	36;
 PedLG[0][38]=	36;
  PedLG[0][39]=	34;
 PedLG[0][40]=	37;
 PedLG[0][41]=	35;
 PedLG[0][42]=	38;
 PedLG[0][43]=	36;
 PedLG[0][44]=	36;
 PedLG[0][45]=	38;
 PedLG[0][46]=	36;
 PedLG[0][47]=	36;
 PedLG[0][48]=	37;
 PedLG[0][49]=	37;
 PedLG[0][50]=	36;
 PedLG[0][51]=	35;
 PedLG[0][52]=	36;
 PedLG[0][53]=	36;
 PedLG[0][54]=	34;
 PedLG[0][55]=	35;
 PedLG[0][56]=	35;
 PedLG[0][57]=	36;
 PedLG[0][58]=	35;
 PedLG[0][59]=	34;
 PedLG[0][60]=	36;
 PedLG[0][61]=	34;
 PedLG[0][62]=	36;
 PedLG[0][63]=	35;
 PedLG[1][0]=	26;
 PedLG[1][1]=	26;
 PedLG[1][2]=	27;
 PedLG[1][3]=	22;
 PedLG[1][4]=	25;
 PedLG[1][5]=	25;
 PedLG[1][6]=	25;
 PedLG[1][7]=	26;
 PedLG[1][8]=	25;
 PedLG[1][9]=	25;
 PedLG[1][10]=	24;
 PedLG[1][11]=	25;
 PedLG[1][12]=	26;
 PedLG[1][13]=	25;
 PedLG[1][14]=	26;
 PedLG[1][15]=	25;
 PedLG[1][16]=	24;
 PedLG[1][17]=	23;
 PedLG[1][18]=	23;
 PedLG[1][19]=	25;
 PedLG[1][20]=	24;
 PedLG[1][21]=	24;
 PedLG[1][22]=	23;
 PedLG[1][23]=	24;
 PedLG[1][24]=	25;
 PedLG[1][25]=	25;
 PedLG[1][26]=	24;
 PedLG[1][27]=	24;
 PedLG[1][28]=	24;
 PedLG[1][29]=	24;
 PedLG[1][30]=	22;
 PedLG[1][31]=	23;
 PedLG[1][32]=	24;
 PedLG[1][33]=	25;
 PedLG[1][34]=	24;
 PedLG[1][35]=	23;
 PedLG[1][36]=	24;
 PedLG[1][37]=	24;
 PedLG[1][38]=	25;
 PedLG[1][39]=	23;
 PedLG[1][40]=	25;
 PedLG[1][41]=	25;
 PedLG[1][42]=	25;
 PedLG[1][43]=	24;
 PedLG[1][44]=	24;
 PedLG[1][45]=	24;
 PedLG[1][46]=	24;
 PedLG[1][47]=	24;
 PedLG[1][48]=	25;
 PedLG[1][49]=	24;
 PedLG[1][50]=	24;
 PedLG[1][51]=	23;
 PedLG[1][52]=	24;
 PedLG[1][53]=	24;
 PedLG[1][54]=	23;
 PedLG[1][55]=	24;
 PedLG[1][56]=	24;
 PedLG[1][57]=	24;
 PedLG[1][58]=	24;
 PedLG[1][59]=	23;
 PedLG[1][60]=	23;
 PedLG[1][61]=	24;
 PedLG[1][62]=	25;
 PedLG[1][63]=	23;
 PedLG[2][0]=	30;
 PedLG[2][1]=	23;
 PedLG[2][2]=	23;
 PedLG[2][3]=	23;
 PedLG[2][4]=	22;
 PedLG[2][5]=	22;
 PedLG[2][6]=	24;
 PedLG[2][7]=	22;
 PedLG[2][8]=	24;
 PedLG[2][9]=	23;
 PedLG[2][10]=	25;
 PedLG[2][11]=	22;
 PedLG[2][12]=	24;
 PedLG[2][13]=	22;
 PedLG[2][14]=	21;
 PedLG[2][15]=	25;
 PedLG[2][16]=	22;
 PedLG[2][17]=	21;
 PedLG[2][18]=	21;
 PedLG[2][19]=	20;
 PedLG[2][20]=	23;
 PedLG[2][21]=	21;
 PedLG[2][22]=	21;
 PedLG[2][23]=	21;
 PedLG[2][24]=	21;
 PedLG[2][25]=	25;
 PedLG[2][26]=	23;
 PedLG[2][27]=	22;
 PedLG[2][28]=	20;
 PedLG[2][29]=	19;
 PedLG[2][30]=	20;
 PedLG[2][31]=	19;
 PedLG[2][32]=	23;
 PedLG[2][33]=	23;
 PedLG[2][34]=	22;
 PedLG[2][35]=	22;
 PedLG[2][36]=	22;
 PedLG[2][37]=	22;
 PedLG[2][38]=	20;
 PedLG[2][39]=	21;
 PedLG[2][40]=	20;
 PedLG[2][41]=	22;
 PedLG[2][42]=	22;
 PedLG[2][43]=	21;
 PedLG[2][44]=	20;
 PedLG[2][45]=	21;
 PedLG[2][46]=	22;
 PedLG[2][47]=	20;
 PedLG[2][48]=	22;
 PedLG[2][49]=	22;
 PedLG[2][50]=	20;
 PedLG[2][51]=	20;
 PedLG[2][52]=	20;
 PedLG[2][53]=	20;
 PedLG[2][54]=	19;
 PedLG[2][55]=	21;
 PedLG[2][56]=	21;
 PedLG[2][57]=	20;
 PedLG[2][58]=	20;
 PedLG[2][59]=	19;
 PedLG[2][60]=	20;
 PedLG[2][61]=	21;
 PedLG[2][62]=	20;
 PedLG[2][63]=	19;
 PedLG[3][0]=	24;
 PedLG[3][1]=	20;
 PedLG[3][2]=	20;
 PedLG[3][3]=	22;
 PedLG[3][4]=	23;
 PedLG[3][5]=	20;
 PedLG[3][6]=	21;
 PedLG[3][7]=	24;
 PedLG[3][8]=	24;
 PedLG[3][9]=	23;
 PedLG[3][10]=	25;
 PedLG[3][11]=	23;
 PedLG[3][12]=	22;
 PedLG[3][13]=	21;
 PedLG[3][14]=	20;
 PedLG[3][15]=	24;
 PedLG[3][16]=	20;
 PedLG[3][17]=	20;
 PedLG[3][18]=	20;
 PedLG[3][19]=	19;
 PedLG[3][20]=	23;
 PedLG[3][21]=	20;
 PedLG[3][22]=	21;
 PedLG[3][23]=	20;
 PedLG[3][24]=	18;
 PedLG[3][25]=	22;
 PedLG[3][26]=	21;
 PedLG[3][27]=	22;
 PedLG[3][28]=	20;
 PedLG[3][29]=	21;
 PedLG[3][30]=	20;
 PedLG[3][31]=	17;
 PedLG[3][32]=	24;
 PedLG[3][33]=	22;
 PedLG[3][34]=	22;
 PedLG[3][35]=	21;
 PedLG[3][36]=	20;
 PedLG[3][37]=	21;
 PedLG[3][38]=	18;
 PedLG[3][39]=	21;
 PedLG[3][40]=	20;
 PedLG[3][41]=	21;
 PedLG[3][42]=	19;
 PedLG[3][43]=	18;
 PedLG[3][44]=	20;
 PedLG[3][45]=	20;
 PedLG[3][46]=	21;
 PedLG[3][47]=	21;
 PedLG[3][48]=	22;
 PedLG[3][49]=	21;
 PedLG[3][50]=	20;
 PedLG[3][51]=	20;
 PedLG[3][52]=	19;
 PedLG[3][53]=	19;
 PedLG[3][54]=	18;
 PedLG[3][55]=	19;
 PedLG[3][56]=	19;
 PedLG[3][57]=	21;
 PedLG[3][58]=	19;
 PedLG[3][59]=	19;
 PedLG[3][60]=	17;
 PedLG[3][61]=	18;
 PedLG[3][62]=	18;
 PedLG[3][63]=	18;
    // ─── DEFINE Histograms for boards ────────────────────────────────────────
  
    TH1F *hLGBoard0 = new TH1F("hLGBoard0", "hLGBoard0", 2000, 0, 100000);
    hLGBoard0->GetXaxis()->SetTitle("LG Board0");
    hLGBoard0->GetYaxis()->SetTitle("Counts");
    TH1F *hHGBoard0 = new TH1F("hHGBoard0", "hHGBoard0", 500, 0, 30000);
    hHGBoard0->GetXaxis()->SetTitle("HG Board0");
    hHGBoard0->GetYaxis()->SetTitle("Counts");

    TH1F *hLGBoard1 = new TH1F("hLGBoard1", "hLGBoard1", 500, 0, 10000);
    hLGBoard1->GetXaxis()->SetTitle("LG Board1");
    hLGBoard1->GetYaxis()->SetTitle("Counts");
    TH1F *hHGBoard1 = new TH1F("hHGBoard1", "hHGBoard1", 500, 0, 30000);
    hHGBoard1->GetXaxis()->SetTitle("HG Board1");
    hHGBoard1->GetYaxis()->SetTitle("Counts");

    TH1F *hLGBoard2 = new TH1F("hLGBoard2", "hLGBoard2", 500, 0, 10000);
    hLGBoard2->GetXaxis()->SetTitle("LG Board2");
    hLGBoard2->GetYaxis()->SetTitle("Counts");
    TH1F *hHGBoard2 = new TH1F("hHGBoard2", "hHGBoard2", 500, 0, 30000);
    hHGBoard2->GetXaxis()->SetTitle("HG Board2");
    hHGBoard2->GetYaxis()->SetTitle("Counts");

    TH1F *hLGBoard3 = new TH1F("hLGBoard3", "hLGBoard3", 500, 0, 10000);
    hLGBoard3->GetXaxis()->SetTitle("LG Board3");
    hLGBoard3->GetYaxis()->SetTitle("Counts");
    TH1F *hHGBoard3 = new TH1F("hHGBoard3", "hHGBoard3", 500, 0, 30000);
    hHGBoard3->GetXaxis()->SetTitle("HG Board3");
    hHGBoard3->GetYaxis()->SetTitle("Counts");

    // ─── DEFINE Histograms for All channels ────────────────────────────────────────

    TH1F *hLGSum = new TH1F("hLGSum", "hLGSum", 2000, 0, 20000);
    hLGSum->GetXaxis()->SetTitle("LG SUM");
    hLGSum->GetYaxis()->SetTitle("Counts");
    TH1F *hHGSum = new TH1F("hHGSum", "hHGSum", 1000, 0, 200000);
    hHGSum->GetXaxis()->SetTitle("HG SUM");
    hHGSum->GetYaxis()->SetTitle("Counts");
    TH1F *hHGSumT = new TH1F("hHGSumT", "hHGSumT", 1000, 0, 200000);
    hHGSumT->GetXaxis()->SetTitle("HG SUM");
    hHGSumT->GetYaxis()->SetTitle("Counts");
    TH1F *hHGSumM = new TH1F("hHGSumM", "hHGSumM", 1000, 0, 200000);
    hHGSumM->GetXaxis()->SetTitle("HG+LG SUM");
    hHGSumM->GetYaxis()->SetTitle("Counts");
    TH1F *hHGSumP = new TH1F("hHGSumP", "hHGSumP", 1000, 0, 200000);
    hHGSumP->GetXaxis()->SetTitle("HG SUM-ped");
    hHGSumP->GetYaxis()->SetTitle("Counts");
    TH1F *hLGSumT = new TH1F("hLGSumT", "hLGSumT", 2000, 0, 20000);
    hLGSumT->GetXaxis()->SetTitle("LG SUM");
    hLGSumT->GetYaxis()->SetTitle("Counts");
    TH1F *hLGSumP = new TH1F("hLGSumP", "hLGSumP", 2000, 0, 20000);
    hLGSumP->GetXaxis()->SetTitle("LG SUM-ped");
    hLGSumP->GetYaxis()->SetTitle("Counts");
    TH1F *hLGSumSat = new TH1F("hLGSumSat", "hLGSumSat", 2000, 0, 200000);
    hLGSumSat->GetXaxis()->SetTitle("LG SUM-Sat");
    hLGSumSat->GetYaxis()->SetTitle("Counts");
    int lgSum = 0;
    int hgSum = 0;

    //-------------------Selected Single Channel ADC spectra histograms:
    //
    // board 0
    //
    TH1F *hb0ch0hg = new TH1F("hb0ch0hg", "hb0ch0hg", 1024, 0, 4093);
    hb0ch0hg->SetTitle("Board 0 Channel 0 HG ADC");
    hb0ch0hg->GetXaxis()->SetTitle("HGADC");
    hb0ch0hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch0lg = new TH1F("hb0ch0lg", "hb0ch0lg", 1024, 0, 4093);
    hb0ch0lg->SetTitle("Board 0 Channel 0 LG ADC");
    hb0ch0lg->GetXaxis()->SetTitle("LGADC");
    hb0ch0lg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch1hg = new TH1F("hb0ch1hg", "hb0ch1hg", 1024, 0, 4093);
    hb0ch1hg->SetTitle("Board 0 Channel 1 HG ADC");
    hb0ch1hg->GetXaxis()->SetTitle("HGADC");
    hb0ch1hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch1lg = new TH1F("hb0ch1lg", "hb0ch1lg", 1024, 0, 4093);
    hb0ch1lg->SetTitle("Board 0 Channel 1 LG ADC");
    hb0ch1lg->GetXaxis()->SetTitle("LGADC");
    hb0ch1lg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch2hg = new TH1F("hb0ch2hg", "hb0ch2hg", 1024, 0, 4093);
    hb0ch2hg->SetTitle("Board 0 Channel 2 HG ADC");
    hb0ch2hg->GetXaxis()->SetTitle("HGADC");
    hb0ch2hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch2lg = new TH1F("hb0ch2lg", "hb0ch2lg", 1024, 0, 4093);
    hb0ch2lg->SetTitle("Board 0 Channel 2 LG ADC");
    hb0ch2lg->GetXaxis()->SetTitle("LGADC");
    hb0ch2lg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch3hg = new TH1F("hb0ch3hg", "hb0ch3hg", 1024, 0, 4093);
    hb0ch3hg->SetTitle("Board 0 Channel 3 HG ADC");
    hb0ch3hg->GetXaxis()->SetTitle("HGADC");
    hb0ch3hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch40hg = new TH1F("hb0ch40hg", "hb0ch40hg", 1024, 0, 4093);
    hb0ch40hg->SetTitle("Board 0 Channel 40 HG ADC");
    hb0ch40hg->GetXaxis()->SetTitle("HGADC");
    hb0ch40hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb0ch40lg = new TH1F("hb0ch40lg", "hb0ch40lg", 1024, 0, 4093);
    hb0ch40lg->SetTitle("Board 0 Channel 40 LG ADC");
    hb0ch40lg->GetXaxis()->SetTitle("LGADC");
    hb0ch40lg->GetYaxis()->SetTitle("Counts");
    //
    // board 1
    //
    TH1F *hb1ch0hg = new TH1F("hb1ch0hg", "hb1ch0hg", 1024, 0, 4093);
    hb1ch0hg->SetTitle("Board 1 Channel 0 HG ADC");
    hb1ch0hg->GetXaxis()->SetTitle("HGADC");
    hb1ch0hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb1ch1hg = new TH1F("hb1ch1hg", "hb1ch1hg", 1024, 0, 4093);
    hb1ch1hg->SetTitle("Board 1 Channel 1HG ADC");
    hb1ch1hg->GetXaxis()->SetTitle("HGADC");
    hb1ch1hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb1ch3hg = new TH1F("hb1ch3hg", "hb1ch3hg", 1024, 0, 4093);
    hb1ch3hg->SetTitle("Board 1 Channel 3 HG ADC");
    hb1ch3hg->GetXaxis()->SetTitle("HGADC");
    hb1ch3hg->GetYaxis()->SetTitle("Counts");
    //
    // board 2
    //
    TH1F *hb2ch0hg = new TH1F("hb2ch0hg", "hb2ch0hg", 1024, 0, 4093);
    hb2ch0hg->SetTitle("Board 2 Channel 0 HG ADC");
    hb2ch0hg->GetXaxis()->SetTitle("HGADC");
    hb2ch0hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb2ch1hg = new TH1F("hb2ch1hg", "hb2ch1hg", 1024, 0, 4093);
    hb2ch1hg->SetTitle("Board 2 Channel 1 HG ADC");
    hb2ch1hg->GetXaxis()->SetTitle("HGADC");
    hb2ch1hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb2ch2hg = new TH1F("hb2ch2hg", "hb2ch2hg", 1024, 0, 4093);
    hb2ch2hg->SetTitle("Board 2 Channel 2 HG ADC");
    hb2ch2hg->GetXaxis()->SetTitle("HGADC");
    hb2ch2hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb2ch3hg = new TH1F("hb2ch3hg", "hb2ch3hg", 1024, 0, 4093);
    hb2ch3hg->SetTitle("Board 2 Channel 3 HG ADC");
    hb2ch3hg->GetXaxis()->SetTitle("HGADC");
    hb2ch3hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb2ch4hg = new TH1F("hb2ch4hg", "hb2ch4hg", 1024, 0, 4093);
    hb2ch4hg->SetTitle("Board 2 Channel 4 HG ADC");
    hb2ch4hg->GetXaxis()->SetTitle("HGADC");
    hb2ch4hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb2ch5hg = new TH1F("hb2ch5hg", "hb2ch5hg", 1024, 0, 4093);
    hb2ch5hg->SetTitle("Board 2 Channel 5 HG ADC");
    hb2ch5hg->GetXaxis()->SetTitle("HGADC");
    hb2ch5hg->GetYaxis()->SetTitle("Counts");
    //
    // board 3
    //
    TH1F *hb3ch0hg = new TH1F("hb3ch0hg", "hb3ch0hg", 1024, 0, 4093);
    hb3ch0hg->SetTitle("Board 3 Channel 0 HG ADC");
    hb3ch0hg->GetXaxis()->SetTitle("HGADC");
    hb3ch0hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb3ch1hg = new TH1F("hb3ch1hg", "hb3ch1hg", 1024, 0, 4093);
    hb3ch1hg->SetTitle("Board 3 Channel 1 HG ADC");
    hb3ch1hg->GetXaxis()->SetTitle("HGADC");
    hb3ch1hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb3ch2hg = new TH1F("hb3ch2hg", "hb3ch2hg", 1024, 0, 4093);
    hb3ch2hg->SetTitle("Board 3 Channel 2 HG ADC");
    hb3ch2hg->GetXaxis()->SetTitle("HGADC");
    hb3ch2hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb3ch3hg = new TH1F("hb3ch3hg", "hb3ch3hg", 1024, 0, 4093);
    hb3ch3hg->SetTitle("Board 3 Channel 3 HG ADC");
    hb3ch3hg->GetXaxis()->SetTitle("HGADC");
    hb3ch3hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb3ch4hg = new TH1F("hb3ch4hg", "hb3ch4hg", 1024, 0, 4093);
    hb3ch4hg->SetTitle("Board 3 Channel 4 HG ADC");
    hb3ch4hg->GetXaxis()->SetTitle("HGADC");
    hb3ch4hg->GetYaxis()->SetTitle("Counts");
    TH1F *hb3ch5hg = new TH1F("hb3ch5hg", "hb3ch5hg", 1024, 0, 4093);
    hb3ch5hg->SetTitle("Board 3 Channel 5 HG ADC");
    hb3ch5hg->GetXaxis()->SetTitle("HGADC");
    hb3ch5hg->GetYaxis()->SetTitle("Counts");
    // Look at high and low gain ADCs for center module:
    TH2F *hM5lowGainADC = new TH2F("hM5lowGainADC", "LowGain ADC module 5", 49, 0, 48, 4096, 0, 4095);
    hM5lowGainADC->GetYaxis()->SetTitle("ADC value");
    hM5lowGainADC->GetXaxis()->SetTitle("SiPM: 0 to 48");
    TH2F *hM5lowGainADCsat = new TH2F("hM5lowGainADCsat", "LowGain ADC module 5 HGsaturated", 49, 0, 48, 4096, 0, 4095);
    hM5lowGainADCsat->GetYaxis()->SetTitle("ADC value");
    hM5lowGainADCsat->GetXaxis()->SetTitle("SiPM: 0 to 48");

    TH2F *hM5highGainADC = new TH2F("hM5highGainADC", "HighGain ADC module 5", 49, 0, 48, 4096, 0, 4095);
    hM5highGainADC->GetYaxis()->SetTitle("ADC value");
    hM5highGainADC->GetXaxis()->SetTitle("SiPM: 0 to 48");
    TH2F *hM5ToT = new TH2F("hM5ToT", "ToT module 5", 49, 0, 48, 4096, 0, 4095);
    hM5ToT->GetYaxis()->SetTitle("ToT value");
    hM5ToT->GetXaxis()->SetTitle("SiPM: 0 to 48");
//
//
//
TH2F* hb0Ch0gcomp = new TH2F("hb0Ch0gcomp", "hb0Ch0gcomp", 1024, 0, 4095,1024,0,4095);
    hb0Ch0gcomp->SetTitle("Board 0 Channel 0 HGvsLG ADC");
    hb0Ch0gcomp->GetXaxis()->SetTitle("HGADC");
    hb0Ch0gcomp->GetYaxis()->SetTitle("LGADC");

TH2F* hb3Ch0gcomp = new TH2F("hb3Ch0gcomp", "hb3Ch0gcomp", 1024, 0, 4095,1024,0,4095);
    hb3Ch0gcomp->SetTitle("Board 3 Channel 0 HGvsLG ADC");
    hb3Ch0gcomp->GetXaxis()->SetTitle("HGADC");
    hb3Ch0gcomp->GetYaxis()->SetTitle("LGADC");

    // ─── BEGINNING OF EVENT LOOP ─────────────────────────────────────────────────
    for (Long64_t i = 0; i < nentries; i++)
    {
        tree->GetEntry(i);
        // cout << "Triggerid = " << trgid << endl;

        // ─── BUILDING EVENT ───────────────────────────────────────────
        // all board you find for given entry will always have same trig id!
        // for convience of user, build board and channel objects
        // but in principle not needed
        Board boards[4];
        buildEvent(boards);

        // finished building event
        int lgsum = 0;
        int hgsum = 0;
        float hglgSum = 0;
        int lgSumP = 0;
        int hgSumP = 0;
        int lgSumSat = 0;

        // ─── EXAMPLE ANALYSIS ─────────────────────────────────────────
        // example loop over all boards of event
        for (int j = 0; j < 4; j++)
        {
            // cout << "Board " << j << " has ts = " << boards[j].ts << endl;
            // example loop over all channels of board

            // one should do a safety check here if board was found
            if (boards[j].ts == -1)
                continue;

            // example of a loop over all channels of a board

            for (int k = 0; k < 64; k++)
            {

                // cout << "Board " << j << " channel " << k << " has lg = " << boards[j].channels[k].lg << endl;
                if (((boards[j].channels[k].hg+Ped[j][k]) < 3000) && (boards[j].channels[k].hg>-1))
                {
                    hgsum += boards[j].channels[k].hg;
                    hglgSum += (boards[j].channels[k].hg-Ped[j][k]);
                    hgSumP += (boards[j].channels[k].hg-Ped[j][k]);
                }
                else if ((boards[j].channels[k].hg>2000)&&(boards[j].channels[k].hg<5000)){
                    hglgSum += (boards[j].channels[k].lg-PedLG[j][k])*9.13;
                    hgsum += (boards[j].channels[k].lg)*9.13;
                    lgSumSat += (boards[j].channels[k].lg)*9.13;
                }   
                else
                {
                    hgsum += 0;
                    hglgSum += 0;
                }
                lgsum += boards[j].channels[k].lg;
                lgSumP += boards[j].channels[k].lg - PedLG[j][k];
            }

            // but you could also do it via functions of the classes themselves. For example:
            //lgSum = boards[j].getLGChannelSum();
            //hgSum = boards[j].getHGChannelSum();
            //hHGSum->Fill(hgsum);
            //hLGSum->Fill(lgsum);
        }
        hHGSum->Fill(hgsum);
        hLGSum->Fill(lgsum);
        hHGSumM->Fill(hglgSum);
        hLGSumP->Fill(lgSumP);
        hHGSumP->Fill(hgSumP);
        hLGSumSat->Fill(lgSumSat);


        // Fill single channels:
        hb0ch0hg->Fill(boards[0].channels[0].hg);
        hb0ch0lg->Fill(boards[0].channels[0].lg);
        hb0ch1hg->Fill(boards[0].channels[1].hg);
        hb0ch1lg->Fill(boards[0].channels[1].lg);
        hb0ch2hg->Fill(boards[0].channels[2].hg);
        hb0ch2lg->Fill(boards[0].channels[2].lg);
        hb0ch3hg->Fill(boards[0].channels[3].hg);
        hb0ch40hg->Fill(boards[0].channels[40].hg);
        hb0ch40lg->Fill(boards[0].channels[40].lg);
        //
        hb1ch0hg->Fill(boards[1].channels[0].hg);
        hb1ch1hg->Fill(boards[1].channels[1].hg);
        hb1ch3hg->Fill(boards[1].channels[3].hg);
        //
        hb2ch0hg->Fill(boards[2].channels[0].hg);
        hb2ch1hg->Fill(boards[2].channels[1].hg);
        hb2ch2hg->Fill(boards[2].channels[2].hg);
        hb2ch3hg->Fill(boards[2].channels[3].hg);
        hb2ch4hg->Fill(boards[2].channels[4].hg);
        hb2ch5hg->Fill(boards[2].channels[5].hg);
        //
        hb3ch0hg->Fill(boards[3].channels[0].hg);
        hb3ch1hg->Fill(boards[3].channels[1].hg);
        hb3ch2hg->Fill(boards[3].channels[2].hg);
        hb3ch3hg->Fill(boards[3].channels[3].hg);
        hb3ch4hg->Fill(boards[3].channels[4].hg);
        hb3ch5hg->Fill(boards[3].channels[5].hg);

        // Fill various sums
        int hgsum0 = boards[0].getHGChannelSum();
        int lgsum1 = boards[1].getLGChannelSum();
        int hgsum1 = boards[1].getHGChannelSum();
        int hgsum2 = boards[2].getHGChannelSum();
        hLGBoard0->Fill(boards[0].getLGChannelSum());
        hHGBoard0->Fill(boards[0].getHGChannelSum());
        hHGBoard1->Fill(boards[1].getHGChannelSum());
        hLGBoard1->Fill(boards[1].getLGChannelSum());
        hLGBoard2->Fill(boards[2].getLGChannelSum());
        hHGBoard2->Fill(boards[2].getHGChannelSum());
        hLGBoard3->Fill(boards[3].getLGChannelSum());
        hHGBoard3->Fill(boards[3].getHGChannelSum());
        int SumHG = boards[0].getHGChannelSum() + boards[1].getHGChannelSum() + boards[2].getHGChannelSum() + boards[3].getHGChannelSum();
        int SumLG = boards[0].getLGChannelSum() + boards[1].getLGChannelSum() + boards[2].getLGChannelSum() + boards[3].getLGChannelSum();

        hHGSumT->Fill(SumHG);
        hLGSumT->Fill(SumLG);
        // Try 2d histo to get all 49 channels of module 1
        // top row of Module 5 (middle module)

        hM5lowGainADC->Fill(0., boards[0].channels[12].lg);
        hM5lowGainADC->Fill(1., boards[0].channels[11].lg);
        hM5lowGainADC->Fill(2., boards[0].channels[10].lg);
        hM5lowGainADC->Fill(3., boards[1].channels[8].lg);
        hM5lowGainADC->Fill(4., boards[1].channels[9].lg);
        hM5lowGainADC->Fill(5., boards[1].channels[10].lg);
        hM5lowGainADC->Fill(6., boards[1].channels[11].lg);
        //
        hM5lowGainADC->Fill(7., boards[0].channels[9].lg);
        hM5lowGainADC->Fill(8., boards[0].channels[8].lg);
        hM5lowGainADC->Fill(9., boards[0].channels[7].lg);
        hM5lowGainADC->Fill(10., boards[0].channels[6].lg);
        hM5lowGainADC->Fill(11., boards[1].channels[5].lg);
        hM5lowGainADC->Fill(12., boards[1].channels[6].lg);
        hM5lowGainADC->Fill(13., boards[1].channels[7].lg);
        //
        hM5lowGainADC->Fill(14., boards[0].channels[5].lg);
        hM5lowGainADC->Fill(15., boards[0].channels[4].lg);
        hM5lowGainADC->Fill(16., boards[0].channels[3].lg);
        hM5lowGainADC->Fill(17., boards[0].channels[2].lg);
        hM5lowGainADC->Fill(18., boards[1].channels[2].lg);
        hM5lowGainADC->Fill(19., boards[1].channels[3].lg);
        hM5lowGainADC->Fill(20., boards[1].channels[4].lg);
        //
        //
        hM5lowGainADC->Fill(21., boards[2].channels[1].lg);
        hM5lowGainADC->Fill(22., boards[2].channels[0].lg);
        hM5lowGainADC->Fill(23., boards[0].channels[1].lg);
        hM5lowGainADC->Fill(24., boards[0].channels[0].lg);
        hM5lowGainADC->Fill(25., boards[3].channels[0].lg);
        hM5lowGainADC->Fill(26., boards[1].channels[0].lg);
        hM5lowGainADC->Fill(27., boards[1].channels[1].lg);
        //
        hM5lowGainADC->Fill(28., boards[2].channels[4].lg);
        hM5lowGainADC->Fill(29., boards[2].channels[3].lg);
        hM5lowGainADC->Fill(30., boards[2].channels[2].lg);
        hM5lowGainADC->Fill(31., boards[3].channels[1].lg);
        hM5lowGainADC->Fill(32., boards[3].channels[2].lg);
        hM5lowGainADC->Fill(33., boards[3].channels[3].lg);
        hM5lowGainADC->Fill(34., boards[3].channels[4].lg);
        //
        hM5lowGainADC->Fill(35., boards[2].channels[7].lg);
        hM5lowGainADC->Fill(36., boards[2].channels[6].lg);
        hM5lowGainADC->Fill(37., boards[2].channels[5].lg);
        hM5lowGainADC->Fill(38., boards[3].channels[5].lg);
        hM5lowGainADC->Fill(39., boards[3].channels[6].lg);
        hM5lowGainADC->Fill(40., boards[3].channels[7].lg);
        hM5lowGainADC->Fill(41., boards[3].channels[8].lg);
        //
        hM5lowGainADC->Fill(42., boards[2].channels[11].lg);
        hM5lowGainADC->Fill(43., boards[2].channels[10].lg);
        hM5lowGainADC->Fill(44., boards[2].channels[9].lg);
        hM5lowGainADC->Fill(45., boards[2].channels[8].lg);
        hM5lowGainADC->Fill(46., boards[3].channels[9].lg);
        hM5lowGainADC->Fill(47., boards[3].channels[10].lg);
        hM5lowGainADC->Fill(48., boards[3].channels[11].lg);
        //
        // HIGH GAIN
        //
        hM5highGainADC->Fill(0., boards[0].channels[12].hg);
        hM5highGainADC->Fill(1., boards[0].channels[11].hg);
        hM5highGainADC->Fill(2., boards[0].channels[10].hg);
        hM5highGainADC->Fill(3., boards[1].channels[8].hg);
        hM5highGainADC->Fill(4., boards[1].channels[9].hg);
        hM5highGainADC->Fill(5., boards[1].channels[10].hg);
        hM5highGainADC->Fill(6., boards[1].channels[11].hg);
        //
        hM5highGainADC->Fill(7., boards[0].channels[9].hg);
        hM5highGainADC->Fill(8., boards[0].channels[8].hg);
        hM5highGainADC->Fill(9., boards[0].channels[7].hg);
        hM5highGainADC->Fill(10., boards[0].channels[6].hg);
        hM5highGainADC->Fill(11., boards[1].channels[5].hg);
        hM5highGainADC->Fill(12., boards[1].channels[6].hg);
        hM5highGainADC->Fill(13., boards[1].channels[7].hg);
        //
        hM5highGainADC->Fill(14., boards[0].channels[5].hg);
        hM5highGainADC->Fill(15., boards[0].channels[4].hg);
        hM5highGainADC->Fill(16., boards[0].channels[3].hg);
        hM5highGainADC->Fill(17., boards[0].channels[2].hg);
        hM5highGainADC->Fill(18., boards[1].channels[2].hg);
        hM5highGainADC->Fill(19., boards[1].channels[3].hg);
        hM5highGainADC->Fill(20., boards[1].channels[4].hg);
        //
        hM5highGainADC->Fill(21., boards[2].channels[1].hg);
        hM5highGainADC->Fill(22., boards[2].channels[0].hg);
        hM5highGainADC->Fill(23., boards[0].channels[1].hg);
        hM5highGainADC->Fill(24., boards[0].channels[0].hg);
        hM5highGainADC->Fill(25., boards[3].channels[0].hg);
        hM5highGainADC->Fill(26., boards[1].channels[0].hg);
        hM5highGainADC->Fill(27., boards[1].channels[1].hg);
        //
        //
        hM5highGainADC->Fill(28., boards[2].channels[4].hg);
        hM5highGainADC->Fill(29., boards[2].channels[3].hg);
        hM5highGainADC->Fill(30., boards[2].channels[2].hg);
        hM5highGainADC->Fill(31., boards[3].channels[1].hg);
        hM5highGainADC->Fill(32., boards[3].channels[2].hg);
        hM5highGainADC->Fill(33., boards[3].channels[3].hg);
        hM5highGainADC->Fill(34., boards[3].channels[4].hg);
        //
        hM5highGainADC->Fill(35., boards[2].channels[7].hg);
        hM5highGainADC->Fill(36., boards[2].channels[6].hg);
        hM5highGainADC->Fill(37., boards[2].channels[5].hg);
        hM5highGainADC->Fill(38., boards[3].channels[5].hg);
        hM5highGainADC->Fill(39., boards[3].channels[6].hg);
        hM5highGainADC->Fill(40., boards[3].channels[7].hg);
        hM5highGainADC->Fill(41., boards[3].channels[8].hg);
        //
        hM5highGainADC->Fill(42., boards[2].channels[11].hg);
        hM5highGainADC->Fill(43., boards[2].channels[10].hg);
        hM5highGainADC->Fill(44., boards[2].channels[9].hg);
        hM5highGainADC->Fill(45., boards[2].channels[8].hg);
        hM5highGainADC->Fill(46., boards[3].channels[9].hg);
        hM5highGainADC->Fill(47., boards[3].channels[10].hg);
        hM5highGainADC->Fill(48., boards[3].channels[11].hg);
        //
        //ToT
        //
        hM5ToT->Fill(0., boards[0].channels[12].tot);
        hM5ToT->Fill(1., boards[0].channels[11].tot);
        hM5ToT->Fill(2., boards[0].channels[10].tot);
        hM5ToT->Fill(3., boards[1].channels[8].tot);
        hM5ToT->Fill(4., boards[1].channels[9].tot);
        hM5ToT->Fill(5., boards[1].channels[10].tot);
        hM5ToT->Fill(6., boards[1].channels[11].tot);
        //
        hM5ToT->Fill(7., boards[0].channels[9].tot);
        hM5ToT->Fill(8., boards[0].channels[8].tot);
        hM5ToT->Fill(9., boards[0].channels[7].tot);
        hM5ToT->Fill(10., boards[0].channels[6].tot);
        hM5ToT->Fill(11., boards[1].channels[5].tot);
        hM5ToT->Fill(12., boards[1].channels[6].tot);
        hM5ToT->Fill(13., boards[1].channels[7].tot);
        //
        hM5ToT->Fill(14., boards[0].channels[5].tot);
        hM5ToT->Fill(15., boards[0].channels[4].tot);
        hM5ToT->Fill(16., boards[0].channels[3].tot);
        hM5ToT->Fill(17., boards[0].channels[2].tot);
        hM5ToT->Fill(18., boards[1].channels[2].tot);
        hM5ToT->Fill(19., boards[1].channels[3].tot);
        hM5ToT->Fill(20., boards[1].channels[4].tot);
        //
        hM5ToT->Fill(21., boards[2].channels[1].tot);
        hM5ToT->Fill(22., boards[2].channels[0].tot);
        hM5ToT->Fill(23., boards[0].channels[1].tot);
        hM5ToT->Fill(24., boards[0].channels[0].tot);
        hM5ToT->Fill(25., boards[3].channels[0].tot);
        hM5ToT->Fill(26., boards[1].channels[0].tot);
        hM5ToT->Fill(27., boards[1].channels[1].tot);
    
        //
        hM5ToT->Fill(28., boards[2].channels[4].tot);
        hM5ToT->Fill(29., boards[2].channels[3].tot);
        hM5ToT->Fill(30., boards[2].channels[2].tot);
        hM5ToT->Fill(31., boards[3].channels[1].tot);
        hM5ToT->Fill(32., boards[3].channels[2].tot);
        hM5ToT->Fill(33., boards[3].channels[3].tot);
        hM5ToT->Fill(34., boards[3].channels[4].tot);
        //
        hM5ToT->Fill(35., boards[2].channels[7].tot);
        hM5ToT->Fill(36., boards[2].channels[6].tot);
        hM5ToT->Fill(37., boards[2].channels[5].tot);
        hM5ToT->Fill(38., boards[3].channels[5].tot);
        hM5ToT->Fill(39., boards[3].channels[6].tot);
        hM5ToT->Fill(40., boards[3].channels[7].tot);
        hM5ToT->Fill(41., boards[3].channels[8].tot);
        //
        hM5ToT->Fill(42., boards[2].channels[11].tot);
        hM5ToT->Fill(43., boards[2].channels[10].tot);
        hM5ToT->Fill(44., boards[2].channels[9].tot);
        hM5ToT->Fill(45., boards[2].channels[8].tot);
        hM5ToT->Fill(46., boards[3].channels[9].tot);
        hM5ToT->Fill(47., boards[3].channels[10].tot);
        hM5ToT->Fill(48., boards[3].channels[11].tot);
        //
        //what if HG is saturated for event?
        //
        int SatLim = 3000;
        
        if (boards[0].channels[12].hg>SatLim){
        hM5lowGainADCsat->Fill(0., boards[0].channels[12].lg);
        }
        if (boards[1].channels[11].hg>SatLim){
        hM5lowGainADCsat->Fill(1., boards[1].channels[11].lg);
        }
        if (boards[0].channels[10].hg>SatLim){
        hM5lowGainADCsat->Fill(2., boards[0].channels[10].lg);
        }
        if (boards[1].channels[8].hg>SatLim){
        hM5lowGainADCsat->Fill(3., boards[1].channels[8].lg);
        }
        if (boards[1].channels[9].hg>SatLim){
        hM5lowGainADCsat->Fill(4., boards[1].channels[9].lg);
        }
        if (boards[1].channels[10].hg>SatLim){
        hM5lowGainADCsat->Fill(5., boards[1].channels[10].lg);
        }
        if (boards[1].channels[11].hg>SatLim){
        hM5lowGainADCsat->Fill(6., boards[1].channels[11].lg);
        }
        if (boards[0].channels[9].hg>SatLim){
        hM5lowGainADCsat->Fill(7., boards[0].channels[9].lg);
        }
        if (boards[0].channels[8].hg>SatLim){
        hM5lowGainADCsat->Fill(8., boards[0].channels[8].lg);
        }
        if (boards[0].channels[7].hg>SatLim){
        hM5lowGainADCsat->Fill(9., boards[0].channels[7].lg);
        }
        if (boards[0].channels[6].hg>SatLim){
        hM5lowGainADCsat->Fill(10., boards[0].channels[6].lg);
        }
        if (boards[1].channels[5].hg>SatLim){
        hM5lowGainADCsat->Fill(11., boards[1].channels[5].lg);
        }
        if (boards[1].channels[6].hg>SatLim){
        hM5lowGainADCsat->Fill(12., boards[1].channels[6].lg);
        }
        if (boards[1].channels[7].hg>SatLim){
        hM5lowGainADCsat->Fill(13., boards[1].channels[7].lg);
        }
        if (boards[0].channels[5].hg>SatLim){
        hM5lowGainADCsat->Fill(14., boards[0].channels[5].lg);
        }
        if (boards[0].channels[4].hg>SatLim){
        hM5lowGainADCsat->Fill(15., boards[0].channels[4].lg);
        }
        if (boards[0].channels[3].hg>SatLim){
        hM5lowGainADCsat->Fill(16., boards[0].channels[3].lg);
        }
        if (boards[0].channels[2].hg>SatLim){
        hM5lowGainADCsat->Fill(17., boards[0].channels[2].lg);
        }
        if (boards[1].channels[2].hg>SatLim){
        hM5lowGainADCsat->Fill(18., boards[1].channels[2].lg);
        }
        if (boards[1].channels[3].hg>SatLim){
        hM5lowGainADCsat->Fill(19., boards[1].channels[3].lg);
        }
        if (boards[1].channels[4].hg>SatLim){
        hM5lowGainADCsat->Fill(20., boards[1].channels[4].lg);
        }

        if (boards[2].channels[1].hg>SatLim){
        hM5lowGainADCsat->Fill(21., boards[2].channels[1].lg);
        }
        if (boards[2].channels[0].hg>SatLim){
        hM5lowGainADCsat->Fill(22., boards[2].channels[0].lg);
        }
        if (boards[0].channels[1].hg>SatLim){
        hM5lowGainADCsat->Fill(23., boards[0].channels[1].lg);
        }
        if (boards[0].channels[0].hg>SatLim){
        hM5lowGainADCsat->Fill(24., boards[0].channels[0].lg);
        }
        if (boards[3].channels[0].hg>SatLim){
        hM5lowGainADCsat->Fill(25., boards[3].channels[0].lg);
        }
        if (boards[1].channels[0].hg>SatLim){
        hM5lowGainADCsat->Fill(26., boards[1].channels[0].lg);
        }
        if (boards[1].channels[1].hg>SatLim){
        hM5lowGainADCsat->Fill(27., boards[1].channels[1].lg);
        }
        if (boards[2].channels[4].hg>SatLim) {
        hM5lowGainADCsat->Fill(28., boards[2].channels[4].lg);
	    }
	    if (boards[2].channels[3].hg>SatLim){
        hM5lowGainADCsat->Fill(29., boards[2].channels[3].lg);
        }
	    if (boards[2].channels[2].hg>SatLim) {
        hM5lowGainADCsat->Fill(30., boards[2].channels[2].lg);
         }
       if (boards[3].channels[1].hg>SatLim){
        hM5lowGainADCsat->Fill(31., boards[3].channels[1].lg);
          }
	    if  (boards[3].channels[2].hg>SatLim) {
         hM5lowGainADCsat->Fill(32., boards[3].channels[2].lg);
         }
         if ( boards[3].channels[3].hg>SatLim)
        hM5lowGainADCsat->Fill(33., boards[3].channels[3].lg);

if (boards[3].channels[4].hg>SatLim){
        hM5lowGainADCsat->Fill(34., boards[3].channels[4].lg);
    }
if (boards[2].channels[7].hg>SatLim){
        hM5lowGainADCsat->Fill(35., boards[2].channels[7].lg);
}
if (boards[2].channels[6].hg>SatLim){
        hM5lowGainADCsat->Fill(36., boards[2].channels[6].lg);
}
if (boards[2].channels[5].hg>SatLim){
        hM5lowGainADCsat->Fill(37., boards[2].channels[5].lg);
}
if (boards[3].channels[5].hg>SatLim){
        hM5lowGainADCsat->Fill(38., boards[3].channels[5].lg);
}
if (boards[3].channels[6].hg>SatLim){
        hM5lowGainADCsat->Fill(39., boards[3].channels[6].lg);
}

if (boards[3].channels[7].hg>SatLim){
        hM5lowGainADCsat->Fill(40., boards[3].channels[7].lg);
}

if (boards[3].channels[8].hg>SatLim){
        hM5lowGainADCsat->Fill(41., boards[3].channels[8].lg);
}

      if (boards[2].channels[11].hg>SatLim){
     hM5lowGainADCsat->Fill(42., boards[2].channels[11].lg);
      }
if (boards[2].channels[10].hg>SatLim){
        hM5lowGainADCsat->Fill(43., boards[2].channels[10].lg);
}
if (boards[2].channels[9].hg>SatLim){
        hM5lowGainADCsat->Fill(44., boards[2].channels[9].lg);
}
if (boards[2].channels[8].hg>SatLim){
        hM5lowGainADCsat->Fill(45., boards[2].channels[8].lg);
}

if (boards[3].channels[9].hg>SatLim){
        hM5lowGainADCsat->Fill(46., boards[3].channels[9].lg);
}

if (boards[3].channels[10].hg>SatLim){
        hM5lowGainADCsat->Fill(47., boards[3].channels[10].lg);
}

if (boards[3].channels[11].hg>SatLim){
        hM5lowGainADCsat->Fill(48., boards[3].channels[11].lg);
}

        //
      
        

        //look at high vs low gain:
        hb0Ch0gcomp->Fill(boards[0].channels[0].hg,boards[0].channels[0].lg);
        hb3Ch0gcomp->Fill(boards[3].channels[0].hg,boards[3].channels[0].lg);

        // if(i>1) return;
    } // end of event loop

    // write histos
    output->cd();

    hLGBoard0->Write();
    hHGBoard0->Write();
    hLGBoard1->Write();
    hHGBoard1->Write();
    hLGBoard2->Write();
    hHGBoard2->Write();
    hLGBoard3->Write();
    hHGBoard3->Write();
    hLGSum->Write();
    hHGSum->Write();
    hHGSumT->Write();
    hHGSumP->Write();
    hLGSumT->Write();
    hHGSumM->Write();
    hLGSumP->Write();
    hLGSumSat->Write();
    //
    // selected single channel ADCs
    //
    hb0ch0hg->Write();
    hb0ch0lg->Write();
    hb0ch1hg->Write();
    hb0ch1lg->Write();
    hb0ch2hg->Write();
    hb0ch3hg->Write();
    hb0ch2lg->Write();
    hb0ch40hg->Write();
    hb0ch40lg->Write();
    //
    hb1ch0hg->Write();
    hb1ch1hg->Write();
    hb1ch3hg->Write();
    //
    hb2ch0hg->Write();
    hb2ch1hg->Write();
    hb2ch2hg->Write();
    hb2ch3hg->Write();
    hb2ch4hg->Write();
    hb2ch5hg->Write();

    //
    hb3ch0hg->Write();
    hb3ch1hg->Write();
    hb3ch2hg->Write();
    hb3ch3hg->Write();
    hb3ch4hg->Write();
    hb3ch5hg->Write();

    hM5lowGainADC->Write();
    hM5highGainADC->Write();
    hM5ToT->Write();
    hM5lowGainADCsat->Write();
    hb0Ch0gcomp->Write();
    hb3Ch0gcomp->Write();
 


    output->Close();

    return;
}
