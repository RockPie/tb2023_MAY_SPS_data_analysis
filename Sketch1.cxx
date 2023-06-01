#include <iostream>
#include <TFile.h>
#include <TTree.h>

int Sketch1(){
    std::cout << "Hello World!" << std::endl;
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