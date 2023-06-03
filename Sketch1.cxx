#include "easylogging++.h"
#include "SJ_datareading.h"
#include "SJ_eventbuilder.h"

#include <TH1.h>
#include <TCanvas.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    // --------------------- Initialize easylogging++ -----------------------
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();
    // ---------------------------------------------------------------------
    // CAEN_data_reader *reader = new CAEN_data_reader("../dataFiles/Run2819_list.txt");
    // reader->print_caen_file_header();
    // reader->extract_frame_info_array(INFINITE_FRAMES, true);
    // std::vector<CAEN_data_reader::FrameInfo> * array_ptr = reader->get_frame_info_array_ptr();
    // LOG(INFO) << "Finished";
    // reader->write_frame_array2root_file();
    // delete reader;

    CAEN_data_reader    *reader  = new CAEN_data_reader();
    CAEN_event_builder  *builder = new CAEN_event_builder();
    reader->read_root_file2frame_array("../cachedFiles/Run_2806.root");
    builder->reconstruct_event(
        reader->get_frame_info_array_ptr(), 
        INFINITE_FRAMES
    );
    auto adc_sum = builder->get_event_sum_array();
    TH1D* h1 = new TH1D("hist", "Histogram", 1000, 5000, 20000);
    for (const auto& i : adc_sum){
        h1->Fill(i);
        // LOG(INFO) << i;
    }

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    h1->Draw();
    c1->SaveAs("hist.png");

    builder->write_event_array2root_file("../cachedFiles/Run_2806_events.root");

    LOG(INFO) << "Finished";
    delete builder;
    delete reader;
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