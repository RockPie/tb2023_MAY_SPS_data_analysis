#include "easylogging++.h"
#include "SJ_datareading.h"
#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"

#include <TH1.h>
#include <TCanvas.h>
#include <TGraph2D.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations

    // * Read mapping csv file
    auto mapping = SJUtil::read_mapping_csv_file("../Mapping_tb2023SPS.csv");
    auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);

    auto builder = new CAEN_event_builder();
    builder->read_root_file2event_array("../cachedFiles/Run_2806_events.root");

    auto eventArrayPtr = builder->get_event_array_ptr();
    auto eventValidPtr = builder->get_event_valid_array_ptr();

    // auto eventNum = eventValidPtr->size();
    auto eventNum = 1;

    for (auto i = 0; i < eventNum; i++){
        if (!eventValidPtr->at(i)) continue;
        auto HG_charges = eventArrayPtr->at(i).HG_charges;
        auto twoD_values = SJUtil::map1d_to_2d(HG_charges, mapping_coords);
        TCanvas *c1 = new TCanvas("c1", "3D scatter", 2500, 2000);
        TGraph2D *gr = new TGraph2D();
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(5);
        gr->SetMarkerColor(kRed);

        for (auto j = 0; j < twoD_values[0].size(); j++){
            if (twoD_values[2][j] == INVALID_2D_VALUE) continue;
            gr->SetPoint(j, twoD_values[0][j], twoD_values[1][j], twoD_values[2][j]);
            LOG(DEBUG) << "x: " << twoD_values[0][j] << ", y: " << twoD_values[1][j] << ", z: " << twoD_values[2][j];
        }

        gr->GetXaxis()->SetRangeUser(0, 105);
        gr->GetYaxis()->SetRangeUser(0, 105);
        gr->GetZaxis()->SetRangeUser(0, 1000);

        gr->Draw("pcol");
        c1->Update();
        c1->WaitPrimitive();
        c1->SaveAs("test.png");
        delete c1;
        delete gr;
    }
    
    delete builder;
    LOG(INFO) << "Finished";
    return 0;
}

void set_easylogger(){
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%H:%m:%s}[%levshort] (%fbase) %msg");
    defaultConf.set(el::Level::Info,    el::ConfigurationType::Format, 
        "%datetime{%H:%m:%s}[\033[1;34m%levshort\033[0m] (%fbase) %msg");
    defaultConf.set(el::Level::Warning, el::ConfigurationType::Format, 
        "%datetime{%H:%m:%s}[\033[1;33m%levshort\033[0m] (%fbase) %msg");
    defaultConf.set(el::Level::Error,   el::ConfigurationType::Format, 
        "%datetime{%H:%m:%s}[\033[1;31m%levshort\033[0m] (%fbase) %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
}