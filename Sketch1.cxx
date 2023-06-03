#include "easylogging++.h"
#include "SJ_datareading.h"
#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"

#include <TH1.h>
#include <TCanvas.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations

    // * Read mapping csv file
    std::vector<std::vector<Short_t>> mapping = SJUtil::read_mapping_csv_file("../Mapping_tb2023SPS.csv");

    auto Mapping_Board_Num_Array    = mapping[0];
    auto Mapping_Channel_Num_Array  = mapping[1];
    auto Mapping_Module_Num_Array   = mapping[2];
    auto Mapping_Col_Array          = mapping[3];
    auto Mapping_Row_Array          = mapping[4];

    // CAEN_event_builder  *builder = new CAEN_event_builder();
    //builder->read_root_file2event_array("../cachedFiles///Run_2806_events.root");
    //auto adc_sum = builder->get_event_sum_array();
    //TH1D* h1 = new TH1D("hist", "Histogram", 1000, 5000, 20000);
    //for (const auto& i : adc_sum){
    //    h1->Fill(i);
    //    // LOG(INFO) << i;
    //}
//
    //TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    //h1->Draw();
    //c1->SaveAs("hist.png");
    
    //delete builder;
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