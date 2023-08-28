#include "SJ_includes.h"

void set_easylogger();

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    int run_number = 2805;
    int eventNum = 10000;
    const double default_error = 7.37;
    const double size_module = 65; // mm
    const double pixelnum_per_module = 35;
    const double com_offset_x_plus = 10;
    const double com_offset_y_plus = 5;
    const double com_offset_x_minus = 0;
    const double com_offset_y_minus = 5;
    set_easylogger();

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
    SJUtil::create_filename("../cachedFiles", 
        DEFAULT_PREFIX_ROOT, run_number, "_mixed_fit_res", DEFAULT_EXTENSION_ROOT);
    auto file_relative_gain_path    = "../dataFiles/slope_offset.root";

    // * Other info generation
    auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
    auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
    auto pedestalInfo   = SJUtil::read_pedestal_csv_file(file_pedestal_path);

    auto gainInfo       = SJUtil::read_relative_gain_root_file(file_relative_gain_path);
    auto slopeInfo      = gainInfo.at(0);
    auto offsetInfo     = gainInfo.at(1);
    LOG(DEBUG) << "slopeInfo size: " << slopeInfo.size();

    // * Main program
    auto builder        = new CAEN_event_builder();
    builder->read_root_file2event_array(file_root_events_path);
    auto eventArrayPtr  = builder->get_event_array_ptr();
    auto eventValidPtr  = builder->get_event_valid_array_ptr();
    // auto eventNum       = int(eventArrayPtr->size()/100);
    auto eventNum_progress_divider = eventNum / 10;

    auto total_events       = 0;
    auto total_valid_events = 0;
    auto total_fit_success  = 0;
    auto fit_param_num      = 10;
    
    TFile *f = new TFile(file_root_results_path, "RECREATE");

    std::vector<Double_t> fit_integral;
    std::vector<Double_t> fit_amp1;
    std::vector<Double_t> fit_amp2;
    std::vector<Double_t> fit_sigma1;
    std::vector<Double_t> fit_sigma2;
    std::vector<Double_t> max_error;
    std::vector<Double_t> fit_x0_1;
    std::vector<Double_t> fit_x0_2;
    std::vector<Double_t> fit_y0_1;
    std::vector<Double_t> fit_y0_2;
    std::vector<Double_t> fit_sigmax_1;
    std::vector<Double_t> fit_sigmax_2;
    std::vector<Double_t> fit_sigmay_1;
    std::vector<Double_t> fit_sigmay_2;
    std::vector<Double_t> chi2_ndf;
    std::vector<Double_t> max_chn_value;
    std::vector<Double_t> chn_sum;
    std::vector<Double_t> chn_sum_N;

    std::vector<Double_t> CoM_array_x;
    std::vector<Double_t> CoM_array_y;

    for (auto i = 0; i < eventNum; i++){
        total_events++;
        if (!eventValidPtr->at(i)) continue;
        
        auto HG_charges     = eventArrayPtr->at(i).HG_charges;
        auto LG_charges     = eventArrayPtr->at(i).LG_charges;
        auto _currentName   = Form("event_%d", i);
        auto _currentTitle  = Form("Event %d", i);
        auto Canvas_Ptr     = new TCanvas(
            _currentName, _currentTitle, 200, 10, 700, 500);

        std::vector<Double_t> HG_charges_double;
        std::vector<Double_t> HG_charges_errors_double;
        for (auto i = 0; i < HG_charges.size(); i++) {
            HG_charges_double.push_back(Double_t(HG_charges[i]));
            HG_charges_errors_double.push_back(default_error);
        }
        auto LG_charges_Multipled   = SJUtil::gain_multiplication(slopeInfo, offsetInfo, LG_charges);
        auto LG_errors_Multipled    = SJUtil::gain_error_multiplication(slopeInfo, offsetInfo, default_error);
        auto _twoD_hg_values        = SJUtil::map1d_to_2d(HG_charges_double, HG_charges_errors_double, mapping_coords);
        auto _twoD_lg_values        = SJUtil::map1d_to_2d(LG_charges_Multipled, LG_errors_Multipled, mapping_coords);
        auto _twoD_hg_values_N      = SJUtil::noise_subtracted_data(_twoD_hg_values, 0);
        auto _twoD_lg_values_N      = SJUtil::noise_subtracted_data(_twoD_lg_values, 0);
        auto _twoD_hg_values_NC     = SJUtil::geo_cutout_data(_twoD_hg_values_N, 20, 85, 25, 70);
        auto _twoD_lg_values_NC     = SJUtil::geo_cutout_data(_twoD_lg_values_N, 20, 85, 25, 70);
        auto _target_event_N          = SJUtil::substitued_data_error(
            _twoD_hg_values_N, 
            _twoD_lg_values_N,
            Double_t(1500),
            Double_t(1.0)
        );
        auto _CoM_vec = SJUtil::get_CoM(_target_event_N);
        CoM_array_x.push_back(_CoM_vec[0]);
        CoM_array_y.push_back(_CoM_vec[1]);
    }

    // Save CoM array to .csv file
    std::ofstream CoM_file;
    auto file_name = Form("../cachedFiles/CoM/CoM_%d.csv", run_number);
    CoM_file.open(file_name);
    for (auto i = 0; i < CoM_array_x.size(); i++){
        CoM_file << CoM_array_x[i] << "," << CoM_array_y[i] << std::endl;
    }
    CoM_file.close();

    // * Get the most dense area of the CoM
    Double_t _sum_CoM_x = 0;
    Double_t _sum_CoM_y = 0;

    for (auto i = 0; i < CoM_array_x.size(); i++){
        _sum_CoM_x += CoM_array_x[i];
        _sum_CoM_y += CoM_array_y[i];
    }

    auto _mean_CoM_x = _sum_CoM_x / CoM_array_x.size();
    auto _mean_CoM_y = _sum_CoM_y / CoM_array_y.size();

    LOG(INFO) << "Mean CoM x: " << _mean_CoM_x;
    LOG(INFO) << "Mean CoM y: " << _mean_CoM_y;

    auto _offset_x_plus_pixel = com_offset_x_plus / size_module * pixelnum_per_module;
    auto _offset_y_plus_pixel = com_offset_y_plus / size_module * pixelnum_per_module;
    auto _offset_x_minus_pixel = com_offset_x_minus / size_module * pixelnum_per_module;
    auto _offset_y_minus_pixel = com_offset_y_minus / size_module * pixelnum_per_module;

    auto _CoM_cutedge_x_max = _mean_CoM_x + _offset_x_plus_pixel;
    auto _CoM_cutedge_x_min = _mean_CoM_x - _offset_x_minus_pixel;
    auto _CoM_cutedge_y_max = _mean_CoM_y + _offset_y_plus_pixel;
    auto _CoM_cutedge_y_min = _mean_CoM_y - _offset_y_minus_pixel;

    LOG(INFO) << "CoM cut edge x max: " << _CoM_cutedge_x_max;
    LOG(INFO) << "CoM cut edge x min: " << _CoM_cutedge_x_min;
    LOG(INFO) << "CoM cut edge y max: " << _CoM_cutedge_y_max;
    LOG(INFO) << "CoM cut edge y min: " << _CoM_cutedge_y_min;

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