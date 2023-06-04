#include "easylogging++.h"
#include "SJ_datareading.h"
#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"

#include <TH1.h>
#include <TCanvas.h>
#include <TGraph2D.h>

// TODO: why there is a (0,0) point in 3-D scatter plot?
// TODO: swap HG and LG charge data in root files

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