#include "SJ_includes.h"

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations

    int run_number = 2806;
    // * File path
    auto file_CAEN_path          = SJUtil::create_filename_CAEN(
            "../dataFiles",    run_number);
    auto file_root_frames_path   = SJUtil::create_filename_frames(
            "../cachedFiles",  run_number);
    auto file_root_events_path   = SJUtil::create_filename_events(
            "../cachedFiles",  run_number);
    auto file_mapping_path       = "../Mapping_tb2023SPS.csv";
    auto file_root_results_path  = SJUtil::create_filename_results(
            "../cachedFiles",  run_number);

    // * Main program
    auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
    auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
    auto builder = new CAEN_event_builder();
    builder->read_root_file2event_array(file_root_events_path);

    auto eventArrayPtr  = builder->get_event_array_ptr();
    auto eventValidPtr  = builder->get_event_valid_array_ptr();
    auto eventNum       = int(eventArrayPtr->size()/100);
    TFile *f = new TFile(file_root_results_path, "RECREATE");

    for (auto i = 0; i < eventNum; i++){
        if (!eventValidPtr->at(i)) continue;
        auto HG_charges     = eventArrayPtr->at(i).HG_charges;
        auto LG_charges     = eventArrayPtr->at(i).LG_charges;
        auto _currentName   = Form("event_%d", i);
        auto _currentTitle  = Form("Event %d", i);
        auto Canvas_Ptr     = new TCanvas(_currentName, _currentTitle, 200,10,  700, 500);
        auto Graph_Ptr      = SJPlot::scatter_3d(HG_charges, mapping_coords,    _currentName, _currentTitle);

        Canvas_Ptr->Update();
        Canvas_Ptr->WaitPrimitive();
        Canvas_Ptr->Write();
        delete Graph_Ptr;
        delete Canvas_Ptr;
    }
    f->Close();
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