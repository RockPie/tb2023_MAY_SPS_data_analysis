#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "./include/SJ_datareading.h"

int main(){
    //gSystem->AddIncludePath("./include");
    //gSystem->AddIncludePath("./src");
    CAEN_data_reader *reader = new CAEN_data_reader();
    delete reader;
    TFile *file_ptr = new TFile("Sketch1.root", "RECREATE");
    TTree *tree_ptr = new TTree("tree", "tree");
    Int_t x = 0;
    tree_ptr->Branch("x", &x, "x/I");
    for (Int_t i = 0; i < 10; i++){
        x = i;
        tree_ptr->Fill();
    }
    tree_ptr->Write();
    file_ptr->Close();
    return 0;
}