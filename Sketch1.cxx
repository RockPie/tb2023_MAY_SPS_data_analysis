#include "SJ_includes.h"

// TODO: why there is a (0,0) point in 3-D scatter plot?

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
    auto eventNum       = int(eventValidPtr->size() / 100);

    TFile *f = new TFile(file_root_results_path, "RECREATE");

    // * Get HGain data
    std::vector<std::vector<Short_t>> HGain_data;
    for (int i = 0; i < eventNum; i++)
        if (eventValidPtr->at(i))
            HGain_data.push_back(eventArrayPtr->at(i).HG_charges);

    // * Generate 2-D distribution
    auto Canvas_Ptr = new TCanvas("Canvas", "Canvas", 800, 600);
    auto hist2D = SJPlot::distribution_2d(HGain_data, "hgain", "HGain_2D");
    hist2D->Draw("colz");
    gStyle->SetPalette(103);

    std::vector<TLine*> LinePtrArray;

    LinePtrArray.push_back(
        SJPlot::add_horizontal_line(35, 0, 105, kWhite, 2, 1));
    LinePtrArray.push_back(
        SJPlot::add_horizontal_line(70, 0, 105, kWhite, 2, 1));
    LinePtrArray.push_back(
        SJPlot::add_vertical_line(35, 0, 105, kWhite, 2, 1));
    LinePtrArray.push_back(
        SJPlot::add_vertical_line(70, 0, 105, kWhite, 2, 1));

    Canvas_Ptr->Write();

    f->Close();

    for (auto i=0; i<4; i++)
        delete LinePtrArray.at(i);

    delete Canvas_Ptr;
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