#include "SJ_includes.h"

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations
    int run_number = 3104;
    int opt;
    while ((opt = getopt(argc, argv, "r:")) != -1){
        switch (opt){
            case 'r':
                run_number = atoi(optarg);
                break;
            default:
                LOG(ERROR) << "Wrong arguments!";
                return 1;
        }
    }

    // * File path
    auto file_CAEN_path             = SJUtil::create_filename_CAEN(
            "../dataFiles",    run_number);
    auto file_root_frames_path      = SJUtil::create_filename_frames(
            "../cachedFiles",  run_number);
    auto file_root_events_path      = SJUtil::create_filename_events(
            "../cachedFiles",  run_number);
    auto file_mapping_path          = "../dataFiles/Mapping_tb2023SPS.csv";
    auto file_root_results_path     = SJUtil::create_filename_results(
            "../cachedFiles",  run_number);
    auto file_pedestal_path         = "../dataFiles/Pedestal_tb_2023SPS.csv";
    auto file_original_fitting_path = SJUtil::create_filename("../cachedFiles",     
        DEFAULT_PREFIX_ROOT, run_number, "_fit_res", DEFAULT_EXTENSION_ROOT);
    auto file_mixed_fitting_path    = SJUtil::create_filename("../cachedFiles", 
        DEFAULT_PREFIX_ROOT, run_number, "_mixed_fit_res", DEFAULT_EXTENSION_ROOT);

    auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
    auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
    auto pedestalInfo   = SJUtil::read_pedestal_csv_file(file_pedestal_path);

    // auto *reader = new CAEN_data_reader(file_CAEN_path);
    // auto *builder = new CAEN_event_builder();
    // reader->extract_frame_info_array(INFINITE_FRAMES, true);
    // reader->write_frame_array2root_file(file_root_frames_path);
    // builder->reconstruct_event(reader->get_frame_info_array_ptr(), INFINITE_FRAMES, false);
    // builder->write_event_array2root_file(file_root_events_path);

    // delete builder;
    // delete reader;

    // * Main program
    auto builder        = new CAEN_event_builder();
    builder->read_root_file2event_array(file_root_events_path);
    auto eventArrayPtr  = builder->get_event_array_ptr();
    auto eventValidPtr  = builder->get_event_valid_array_ptr();

    double default_error = 1/3.0;
    auto first_event_index = 0;
    auto event_number = 20000;


    std::vector<Double_t> initial_values;
    for (auto i = 0; i < 256; i++) initial_values.push_back(0);
    SJUtil::DataErrorSet2D<Double_t> _twoD_hg_sum = SJUtil::map1d_to_2d(initial_values, initial_values, mapping_coords);

    for (auto _event_index = first_event_index; _event_index < event_number; _event_index++){
        if (!eventValidPtr->at(_event_index)) continue;
        auto HG_charges     = eventArrayPtr->at(_event_index).HG_charges;
        auto LG_charges     = eventArrayPtr->at(_event_index).LG_charges;
        auto _currentName   = Form("event_%d", _event_index);
        auto _currentTitle  = Form("Event %d", _event_index);

        std::vector<Double_t> HG_charges_double;
        std::vector<Double_t> HG_charges_errors_double;
        for (auto i = 0; i < HG_charges.size(); i++) {
            HG_charges_double.push_back(Double_t(HG_charges[i]));
            // LOG(INFO) << HG_charges[i];
            HG_charges_errors_double.push_back(default_error);
        }

        auto _twoD_hg_values        = SJUtil::map1d_to_2d(HG_charges_double, HG_charges_errors_double, mapping_coords);
        _twoD_hg_sum = _twoD_hg_sum + _twoD_hg_values;
    }

    auto hist_canvas = new TCanvas("hist_canvas", "hist_canvas", 1000, 1000);
    auto _2d_hist = SJPlot::hist_2d(_twoD_hg_sum, 2096 * event_number);
    _2d_hist->Draw("colz");
    auto file_name = "test" + std::to_string(run_number) + ".png";
    hist_canvas->SaveAs(file_name.c_str());
    hist_canvas->Close();
    _2d_hist->Delete();

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