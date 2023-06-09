#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include "hcaltreehelpers.h"

using namespace std;


std::map<std::string, TH1F*> fHisto1DMap;
std::map<std::string, TProfile*> fProfile1DMap; 

void AddProfile(std::string namep, Int_t something, Int_t xmin, Int_t xmax, Int_t ymin, Int_t ymax){

  TProfile *hcalib = new TProfile (namep.c_str(), namep.c_str(), something, xmin, xmax, ymin, ymax);

  hcalib->GetXaxis()->SetTitle("ADC Counts");
  hcalib->GetYaxis()->SetTitle("ADC Counts");

  fProfile1DMap[namep]=hcalib;

  return;
  
}

TProfile* GetProfile(std::string namep){

  return fProfile1DMap[namep];
}



void AddHisto (std::string name, Int_t nx,Int_t xlow, Int_t xup )  {

  TH1F* chan = new TH1F ( name.c_str(),name.c_str(),  nx, xlow,  xup );

    chan->GetXaxis()->SetTitle("Charge [ADC]");
    chan->GetYaxis()->SetTitle("Counts");

    fHisto1DMap[name]=chan;

    return;

}

TH1F* GetHisto (std::string name ){

    return fHisto1DMap[name]; 
}


void read_hcal_tree(TString inputfile = "testHCal_2775.root"){


  FILE* fp;

  float p0[4][64];  //f(x) = p0 + p1*x
  float p1[4][64];

  float d,m = 0;

  int brp = 0; //counter to match the board ID


    for (int i = 0; i<4; i++) {
      for (int j=0; j<64; j++) {

   p0[i][j] = 0;
   p1[i][j] = 0;
   
    }
    } 
  
   fp = fopen("fit-params.txt", "r");
  
   if (fp == NULL )     
     {
     printf ( "Error: can't open file\n" );
     return 0;
     }
  
  
  for (int i=0; i<256; i++){ 
    fscanf(fp, "%f %f",&d, &m);

    if (i%64 == 0 && i>0) {brp++;}
    
    p0[brp][i%64] = d;
    p1[brp][i%64] = m;
 
  }
  
  
  fclose(fp);

  

  
    TFile* intput = new TFile(inputfile.Data(), "READ");
    TTree* tree = (TTree*) intput->Get("hcaldata");

    
    // ─── PEDESTAL CALIBRATION VALUES ────────────────────────────────────────────────

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


    

    // ─── SETTING BRANCH ADDRESSES ────────────────────────────────────────────────
    setBranchAddresses(tree); // see hcaltreehelpers.h
    
    Long64_t nentries = tree->GetEntries();
    //  cout << "-> nTriggers = " << nentries << endl;

    // ─── DEFINITION OF EXAMPLE HISTOGRAMS ────────────────────────────────────────
    TFile* output = new TFile("Run2775-calib-check-1500-low.root", "RECREATE");
    TH1F* hLGBoard0 = new TH1F("hLGBoard0", "hLGBoard0", 500, 0, 10000);
    
    TH1F* hTotCharge = new TH1F("hTotCharge", "ADC HG Sum", 5000, 0, 200000);
    
    TH1F* hbrhg = new TH1F("hbrhg", "hbrhg", 256, 0, 256);
    TH1F* hbrlg = new TH1F("hbrlg", "hbrlg", 256, 0, 256);

    TH1F* hTotChargeLG = new TH1F("hTotChargeLG", "ADC LG Sum", 5000, 0, 100000);
    
    TH1F* hTotChargeMixed = new TH1F("hTotChargeMixed", "ADC No Saturation Sum", 5000, 0, 200000);

    

    TH1F* hTotChargeB0 = new TH1F("hTotChargeB0", "ADC Sum in Board 0 HG", 5000, 0, 100000);
    TH1F* hTotChargeB1 = new TH1F("hTotChargeB1", "ADC Sum in Board 1 HG", 5000, 0, 100000);
    TH1F* hTotChargeB2 = new TH1F("hTotChargeB2", "ADC Sum in Board 2 HG", 5000, 0, 100000);
    TH1F* hTotChargeB3 = new TH1F("hTotChargeB3", "ADC Sum in Board 3 HG", 5000, 0, 100000);

    TH1F* hTotChargeB0LG = new TH1F("hTotChargeB0LG", "ADC Sum in Board 0 LG", 5000, 0, 100000);
    TH1F* hTotChargeB1LG = new TH1F("hTotChargeB1LG", "ADC Sum in Board 1 LG", 5000, 0, 100000);
    TH1F* hTotChargeB2LG = new TH1F("hTotChargeB2LG", "ADC Sum in Board 2 LG", 5000, 0, 100000);
    TH1F* hTotChargeB3LG = new TH1F("hTotChargeB3LG", "ADC Sum in Board 3 LG", 5000, 0, 100000);

    TH1D *hNumberOfSaturatedChannels = new TH1D("hNumberOfSaturatedChannels","Number of Saturated Channels",249,0,249);
    TH1D *hNumberOfSaturatedChannelslg = new TH1D("hNumberOfSaturatedChannelslg","Number of Saturated Channels",249,0,249);
    




    // ─── DEFINITION OF Channel HISTOGRAMS ────────────────────────────────────────

    int b = 0; // Board ID
    int c = 0; // Channel ID


    
    for (int i =0; i<4; i++){
      for (int j=0; j<64; j++){

	    std::string str = " hChargeB"+ std::to_string(i) + "Ch" +  std::to_string(j) + "HG"; 
	    
	    const char *cstr = str.c_str();
	
	    AddHisto( cstr, 4096, 0, 4096);
	
      }
    }

        for (int i =0; i<4; i++){
      for (int j=0; j<64; j++){

	    std::string str2 = " hChargeB"+ std::to_string(i) + "Ch" +  std::to_string(j) + "LG"; 

	    const char *cstr2 = str2.c_str();
	
	    AddHisto( cstr2, 4096, 0, 4096);
	
      }
    }

	//  TProfile *hCalib_HG_LG[4][64];
    
    for (int i =0; i<4; i++){
      for (int j=0; j<64; j++){
	
	std::string str3 = "hCalib_HG_LG_B"+ std::to_string(i) + "Ch" +  std::to_string(j);   
	const char *cstr3 = str3.c_str();
	//
	AddProfile (cstr3,1000,0,500,0.0,5000);
	//hCalib_HG_LG[i][j] = new TProfile(cstr2,cstr2,1000,0,500,0.0,5000);  
      }
    }	

    
    hLGBoard0->GetXaxis()->SetTitle("LG SUM");
    hLGBoard0->GetYaxis()->SetTitle("Counts");

    hTotCharge->GetXaxis()->SetTitle("Charge [ADC]");
    hTotCharge->GetYaxis()->SetTitle("Counts");

    hTotChargeB0->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB0->GetYaxis()->SetTitle("Counts");


    hTotChargeB1->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB1->GetYaxis()->SetTitle("Counts");


    hTotChargeB2->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB2->GetYaxis()->SetTitle("Counts");


    hTotChargeB3->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB3->GetYaxis()->SetTitle("Counts");


    hTotChargeB0LG->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB0LG->GetYaxis()->SetTitle("Counts");


    hTotChargeB1LG->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB1LG->GetYaxis()->SetTitle("Counts");


    hTotChargeB2LG->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB2LG->GetYaxis()->SetTitle("Counts");


    hTotChargeB3LG->GetXaxis()->SetTitle("Charge [ADC]");
    hTotChargeB3LG->GetYaxis()->SetTitle("Counts");




    
    // ─── BEGINNING OF EVENT LOOP ─────────────────────────────────────────────────
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);
	//   cout << "Triggerid = " << trgid << endl;

	int totsum = 0;
	int totLG = 0;
	int totmix =0;
	int tot0=0; 
	int tot1=0; 
	int tot2=0; 
	int tot3=0;

	int tot0lg=0; 
	int tot1lg=0; 
	int tot2lg=0; 
	int tot3lg=0;

	int chch[4][64];
	int chch2[4][64];

	int nchannels = 0;
	int nchannelslg = 0;
	
	

	
	for (int k=0; k<4; k++){
	  for (int n=0; n<64; n++){
	    chch[k][n]=0;

	  }
	}
	
	for (int k=0; k<4; k++){
	  for (int n=0; n<64; n++){
	    chch2[k][n]=0;

	  }
	}
	
        // ─── BUILDING EVENT ───────────────────────────────────────────
        // all board you find for given entry will always have same trig id!
        // for convience of user, build board and channel objects
        // but in principle not needed
        Board boards[4];
        buildEvent(boards);

        // finished building event
        
        // ─── EXAMPLE ANALYSIS ─────────────────────────────────────────
        // example loop over all boards of event
        for (int j = 0; j < 4; j++) {
            // cout << "Board " << j << " has ts = " << boards[j].ts << endl;
            // example loop over all channels of board

            // one should do a safety check here if board was found
            if(boards[j].ts == -1) continue;

            // example of a loop over all channels of a board
            int lgsum = 0;
            for (int k = 0; k < 64; k++) {
                // cout << "Board " << j << " channel " << k << " has lg = " << boards[j].channels[k].lg << endl;
                lgsum += boards[j].channels[k].lg;
            }
            hLGBoard0->Fill(lgsum);

            // but you could also do it via functions of the classes themselves. For example:
            int lgsumExample = boards[j].getLGChannelSum();
        }


	int brhg =0;
	int brlg=0;
	
	for (int m =0; m<4; m++){

	  if (boards[m].ts ==-1 ) continue;
	  
	  for (int l=0; l<64; l++){

	    if (m==0){   if (boards[m].channels[l].hg > 4095) {boards[m].channels[l].hg = -1; } if (boards[m].channels[l].lg > 4095) {boards[m].channels[l].lg = -1; }
	      tot0 +=boards[m].channels[l].hg - Ped[m][l]; tot0lg +=boards[m].channels[l].lg - Ped[m][l]; }
	    
	    
	    if (m==1){   if (boards[m].channels[l].hg > 4095) {boards[m].channels[l].hg = -1; } if (boards[m].channels[l].lg > 4095) {boards[m].channels[l].lg = -1; }
	      tot1 +=boards[m].channels[l].hg - Ped[m][l]; tot1lg +=boards[m].channels[l].lg - Ped[m][l]; }


	  if (m==2){   if (boards[m].channels[l].hg > 4095) {boards[m].channels[l].hg = -1; }  if (boards[m].channels[l].lg > 4095) {boards[m].channels[l].lg = -1; }
	    tot2 +=boards[m].channels[l].hg - Ped[m][l]; tot2lg +=boards[m].channels[l].lg - Ped[m][l]; }


	  if (m==3){   if (boards[m].channels[l].hg > 4095) {boards[m].channels[l].hg = -1; }  if (boards[m].channels[l].lg > 4095) {boards[m].channels[l].lg = -1; }
	    tot3 +=boards[m].channels[l].hg - Ped[m][l]; tot3lg +=boards[m].channels[l].lg - Ped[m][l]; }

          

	  if(boards[m].channels[l].hg - Ped[m][l]>0){
	  
	  chch[m][l] += boards[m].channels[l].hg - Ped[m][l]; 

	  totsum +=boards[m].channels[l].hg - Ped[m][l];
	  brhg++;


	    std::string str3 = "hCalib_HG_LG_B"+ std::to_string(m) + "Ch" +  std::to_string(l);   
	    const char *cstr3 = str3.c_str();	    

	  

	    GetProfile(cstr3)->Fill(boards[m].channels[l].lg- PedLG[m][l] ,boards[m].channels[l].hg - Ped[m][l]);
	    
	  
	  
	  if(boards[m].channels[l].hg - Ped[m][l] > 3950. ) {nchannels ++;}

	  if(boards[m].channels[l].hg - Ped[m][l] > 1500.) {

	    
	    if(boards[m].channels[l].lg - PedLG[m][l] > 0){
	    
	      // hCalib_HG_LG[m][l] ->Fit("pol1");
	      totmix += (boards[m].channels[l].lg - PedLG[m][l])*p1[m][l] - p0[m][l];
	    }
	    
	    
	    else{  totmix += boards[m].channels[l].hg - Ped[m][l];} }
	  
	  
	  if(boards[m].channels[l].hg - Ped[m][l] <= 4000. ) {
	    totmix += boards[m].channels[l].hg - Ped[m][l];
	  }
	  
	  }

	  if(boards[m].channels[l].lg - PedLG[m][l] > 0){
	     if(boards[m].channels[l].lg - PedLG[m][l] > 3950. ) {nchannelslg ++;}

	  

	  totLG += boards[m].channels[l].lg - PedLG[m][l];
	  chch2[m][l] += boards[m].channels[l].lg - PedLG[m][l];

	  brlg++;

	  
	  }
	}
	}
	
	hTotChargeMixed->Fill(totmix);
	hTotCharge->Fill(totsum);
	hTotChargeLG->Fill(totLG);
	
	hTotChargeB0->Fill(tot0);
	hTotChargeB1->Fill(tot1);
	hTotChargeB2->Fill(tot2);
	hTotChargeB3->Fill(tot3);

	hTotChargeB0LG->Fill(tot0lg);
	hTotChargeB1LG->Fill(tot1lg);
	hTotChargeB2LG->Fill(tot2lg);
	hTotChargeB3LG->Fill(tot3lg);
	hbrhg->Fill(brhg);
	hbrlg->Fill(brlg);
	hNumberOfSaturatedChannels->Fill(nchannels);
	hNumberOfSaturatedChannelslg->Fill(nchannelslg);

	for (int k=0; k<4; k++){
	  for (int n=0; n<64; n++){

	    std::string str1 = " hChargeB"+ std::to_string(k) + "Ch" +  std::to_string(n) + "HG"; 

	    const char *cstr1 = str1.c_str();

	    GetHisto(cstr1)->Fill(chch[k][n]);
	    
	  }
	}

	for (int k=0; k<4; k++){
	  for (int n=0; n<64; n++){

	    std::string str2 = " hChargeB"+ std::to_string(k) + "Ch" +  std::to_string(n) + "LG"; 

	    const char *cstr2 = str2.c_str();

	    GetHisto(cstr2)->Fill(chch2[k][n]);
	    
	  }
	}

        // if(i>1) return;
    } // end of event loop


    //FOR CHARGE IN CHANNEL STUDIES 

    
    /*    int param[4][64];
	for (int k=0; k<4; k++){
	  for (int n=0; n<64; n++){
	    param[k][n]=0;

	  }
	}
    
        fstream newfile;
	    newfile.open("means-overflow.txt", ios::out);
	      {
    for (int k=0; k<4; k++){
	  for (int n=0; n<64; n++){

	    std::string str1 = " hChargeB"+ std::to_string(k) + "Ch" +  std::to_string(n) + "HG"; 

	    const char *cstr1 = str1.c_str();

	     int min = 3950, max = 4075; 

	    
	     // if (k == 0) { min = 0; max = 45;} 

	    
	   
	    TF1* f1 = new TF1("f1", "gaus", min, max) ;
	    
	    GetHisto(cstr1)->Fit(f1,"","", min, max);
	    //GetHisto(cstr1)->Fit(f1);

	    param[k][n] = f1->GetParameter(1); 

	    std::cout << param[k][n] << std::endl;

	
		if (newfile.is_open())
		  {

		    newfile<<param[k][n]<<std::endl;
		  }


	      }	    
	    
	  }
	}


	      
    */

    // FOR PROFILE STUDIES


    
    float param1[4][64];
    for (int k=0; k<4; k++){
      for (int n=0; n<64; n++){
	param1[k][n]=0;
	
      }
    }
    
    float param2[4][64];
    for (int k=0; k<4; k++){
      for (int n=0; n<64; n++){
	param2[k][n]=0;
	
      }
    }
    
    fstream newfile;
    newfile.open("fit-params.txt", ios::out);
    {
      for (int k=0; k<4; k++){
	for (int n=0; n<64; n++){
	  
	  std::string str3 = "hCalib_HG_LG_B"+ std::to_string(k) + "Ch" +  std::to_string(n);   
	  const char *cstr3 = str3.c_str();
	  
	  
	  
	  Int_t min = 20, max = 160; 
	  
	  
	  // if (k == 0) { min = 0; max = 45;} 
	  
	    
	  
	  TF1* f2 = new TF1("f2", "pol1", min, max) ;
	  
	  GetProfile(cstr3)->Fit("f2", "", "", min,max);
	  //GetHisto(cstr1)->Fit(f1);
	  
	  param1[k][n] = f2->GetParameter(0); 
	  param2[k][n] = f2->GetParameter(1); 

	  std::cout << param1[k][n] << " " <<  param2[k][n] <<std::endl;
	  
	  
		if (newfile.is_open())
		  {
		    
		    newfile<<param1[k][n] << " " <<  param2[k][n]<<std::endl;
		  }


	      }	    
	    
	  }
	}

    /*
    for (int i = 0; i<4; i++) {
      for (int j=0; j<64; j++) {

	std::cout<< p0[i][j] << " "  << p1[i][j] << std::endl;  //Check the values of the calibration stored in the file
    }
    } 
    */ 
// write histos
output->cd();
hLGBoard0->Write();

 output->Write();
return;
}

