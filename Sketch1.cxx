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
    auto file_pedestal_path      = "../Pedestal_tb_2023SPS.csv";

    // * Other info generation
    auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
    auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
    auto pedestalInfo   = SJUtil::read_pedestal_csv_file(file_pedestal_path);

    // * Main program
    auto builder        = new CAEN_event_builder();
    builder->read_root_file2event_array(file_root_events_path);
    auto eventArrayPtr  = builder->get_event_array_ptr();
    auto eventValidPtr  = builder->get_event_valid_array_ptr();
    /// auto eventNum       = int(eventArrayPtr->size()-1);
    auto eventNum       = int(eventArrayPtr->size()/10);
    auto eventNum_progress_divider = eventNum / 10;

    auto total_events       = 0;
    auto total_valid_events = 0;
    auto total_fit_success  = 0;

    auto NDF                = 5;
    
    TFile *f = new TFile(file_root_results_path, "RECREATE");

    std::vector<double*> _fit_result;
    for (auto i = 0; i < eventNum; i++){
        total_events++;
        if (!eventValidPtr->at(i)) continue;
        if (i % eventNum_progress_divider == 0)
            LOG(INFO) << "Processing event " << i << " / " << eventNum;
        auto HG_charges     = eventArrayPtr->at(i).HG_charges;
        auto LG_charges     = eventArrayPtr->at(i).LG_charges;
        auto _currentName   = Form("event_%d", i);
        auto _currentTitle  = Form("Event %d", i);
        auto Canvas_Ptr     = new TCanvas(
            _currentName, _currentTitle, 200, 10, 700, 500);
        
        auto _twoD_hg_values   = SJUtil::map1d_to_2d(HG_charges, mapping_coords);
        auto _twoD_lg_values   = SJUtil::map1d_to_2d(LG_charges, mapping_coords);
        auto _twoD_hg_values_NS= SJUtil::noise_subtracted_data(_twoD_hg_values, 10);
        auto _twoD_lg_values_NS= SJUtil::noise_subtracted_data(_twoD_lg_values, 10);
        // auto _twoD_hg_values_NSS = SJUtil::substitued_data(
        //     _twoD_hg_values_NS, 
        //     _twoD_lg_values_NS, 
        //     Short_t(SUBSTITUE_THRESHOLD),
        //     Short_t(HG_RELATIVE_GAIN)
        // );
        auto _twoD_hg_values_NSS = _twoD_hg_values_NS;

        if (_twoD_hg_values_NSS.value_vec.size() <= NDF + 1) continue;
        total_valid_events++;
        auto Graph_Ptr      = SJPlot::scatter_3d_raw(_twoD_hg_values_NSS, _currentName, _currentTitle);

        // * Fit 
        auto _initial_values = SJUtil::map_max_point_index(_twoD_hg_values_NSS);
        if (_initial_values == nullptr){
            int _initial_values[3] = {50, 50, 4000};
        }
        TF2 *gaussianFunc = new TF2("gaussianFunc", SJFunc::gaussian2D, 0, 105, 0, 105, NDF);
        gaussianFunc->SetParameters(
            _initial_values[0], 
            _initial_values[1], 
            5, 5, 
            _initial_values[2]
        );

        gaussianFunc->SetParNames("x_{mean}", "y_{mean}", "#sigma_{x}", "#sigma_    {y}", "N");
        gaussianFunc->SetLineColor(kRed);
        gaussianFunc->SetLineWidth(2);
        gaussianFunc->SetFillColorAlpha(kBlue, 0.3);
        auto fit_res = Graph_Ptr->Fit(gaussianFunc, "RQ");
        if (fit_res != 0) 
            LOG(WARNING) << "Fitting failed for event "<< i;
        else {
            total_fit_success++;
            double* _temp_parameters = new double[NDF + 3];
            for (auto i = 0; i < NDF + 1; i++){
                _temp_parameters[i] = gaussianFunc->GetParameter(i);
            }
            _temp_parameters[NDF] = gaussianFunc->GetChisquare();
            _temp_parameters[NDF + 1] = gaussianFunc->GetNDF();
            _temp_parameters[NDF + 2] = double(i);
            _fit_result.push_back(_temp_parameters);
        }
        Canvas_Ptr->cd();
        Graph_Ptr->Draw("pcolz");
        gaussianFunc->Draw("surf3 same");

        Canvas_Ptr->Update();
        Canvas_Ptr->WaitPrimitive();
        Canvas_Ptr->Write();
        delete Graph_Ptr;
        delete Canvas_Ptr;
    }
    LOG(INFO) << "Total expected events: "  << total_events;
    LOG(INFO) << "Total valid events: "     << total_valid_events;
    LOG(INFO) << "Total fit success: "      << total_fit_success;
    double _success_rate = double(total_fit_success) / double(total_valid_events);
    LOG(INFO) << "Success rate: "           << _success_rate;
    f->Close();
    delete builder;
    LOG(INFO) << "Finished fitting and plotting";
    SJUtil::write_fitted_data_file("../cachedFiles/Run_2806_fit_res.root", _fit_result);
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