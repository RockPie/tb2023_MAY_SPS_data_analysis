#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "easylogging++.h"
#include "SJ_datareading.h"

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%H:%m:%s}[%level](%fbase) %msg");
    // set the time format

    el::Loggers::reconfigureLogger("default", defaultConf);


    CAEN_data_reader *reader = new CAEN_data_reader("../dataFiles/Run2806_list.txt");
    reader->print_caen_file_header();
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