#include "SJ_includes.h"

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    // * -n 1 - 10: index of this job
    int run_number = 2805;
    // analyse input arguments
    int opt;
    int job_index   = 0; // -n 1 - 10: index of this job
    int job_num     = 8; // -t 1 - 10: total number of jobs
    const int orginal_eventNum = 100;
    const double default_error = 7.37;
    int eventNum    = orginal_eventNum; // -e 1 - 1000: number of events to be processed

    while ((opt = getopt(argc, argv, "n:t:e:")) != -1){
        switch (opt){
            case 'n':
                job_index = atoi(optarg);
                break;
            case 't':
                job_num = atoi(optarg);
                break;
            case 'e':
                eventNum = atoi(optarg);
                break;
            default:
                LOG(ERROR) << "Wrong arguments!";
                return 1;
        }
    }

    bool is_in_parallel = eventNum != orginal_eventNum;

    set_easylogger();   // * set easylogging++ configurations

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

    //* multi CPU fitting

    auto first_event = 0;

    if (job_index != 0){
        first_event = int(eventNum / job_num * (job_index-1));
        eventNum = int(eventNum / job_num * job_index);
    }

    eventNum_progress_divider = eventNum / 10;

    for (auto i = first_event; i < eventNum; i++){
        total_events++;
        if (!eventValidPtr->at(i)) continue;
        if (i % eventNum_progress_divider == 0 && (job_index == 1 || !is_in_parallel))
            LOG(INFO) << "Processing event " << i << " / " << eventNum;
        
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
        // auto _twoD_hg_values_NC     = SJUtil::geo_cutout_data(_twoD_hg_values_N, 33, 73, 27, 67);
        // auto _twoD_lg_values_NC     = SJUtil::geo_cutout_data(_twoD_lg_values_N, 33, 73, 27, 67);
        auto _target_event_N          = SJUtil::substitued_data_error(
            _twoD_hg_values_N, 
            _twoD_lg_values_N,
            Double_t(1500),
            Double_t(1.0)
        );
        // auto _target_event = _target_event_N;
        auto _target_event = SJUtil::area_normalized_data(_target_event_N);
        // if (SJUtil::geo_event_is_cut(_target_event, 53.22, 58.60, 43.96, 49.34))
        //     continue;
        // auto _twoD_hg_values_NA     = SJUtil::area_normalized_data(_twoD_hg_values_N);
        // auto _twoD_lg_values_NA     = SJUtil::area_normalized_data(_twoD_lg_values_N);

        // auto _target_event = _twoD_hg_values_NA;
        if ( _target_event.value_vec.size() <= fit_param_num + 1)  
            continue;
        total_valid_events++;

        auto value_vec = _target_event.value_vec;
        double sum = 0;
        double sumN = 0;

        for (auto i = 0; i < _target_event.value_vec.size(); i++)
            sum += double(_target_event.value_vec[i]);

        for (auto i = 0; i < _target_event_N.value_vec.size(); i++)
            sumN += double(_target_event_N.value_vec[i]);

        auto Graph_Ptr      = SJPlot::scatter_3d_raw_errors( 
            _target_event, _currentName, _currentTitle);
        Graph_Ptr->SetMarkerColor(kBlue);
        Graph_Ptr->SetLineWidth(2);
        Graph_Ptr->SetLineColor(kBlue);
        auto Graph_Sub_Ptr      = SJPlot::scatter_3d_raw_errors( 
            _twoD_lg_values_N, _currentName, _currentTitle);
        Graph_Sub_Ptr->SetMarkerColor(kRed);
        Graph_Sub_Ptr->SetMarkerStyle(21);

        // * Getting initial parameters
        auto initial_params = SJUtil::map_max_point_index(_target_event);
        auto main_gaussian_initial_amp = initial_params[2] * 0.85;
        auto sub_gaussian_initial_amp  = initial_params[2] * 0.15;
        auto main_gaussian_initial_x0  = initial_params[0];
        auto main_gaussian_initial_y0  = initial_params[1];
        auto sub_gaussian_initial_x0   = initial_params[0];
        auto sub_gaussian_initial_y0   = initial_params[1];

        TF2 *gaussianFunc = new TF2("gaussianFunc", SJFunc::dual_gaussian2D, 0, 105, 0, 105, fit_param_num);
        gaussianFunc->SetParameters(
            main_gaussian_initial_x0,   main_gaussian_initial_y0,   3, 3,
            sub_gaussian_initial_x0,    sub_gaussian_initial_y0,    8, 8,
            main_gaussian_initial_amp,  sub_gaussian_initial_amp
        );

        gaussianFunc->SetParLimits(0, 0, 105);
        gaussianFunc->SetParLimits(1, 0, 105);
        gaussianFunc->SetParLimits(2, 0, 50);
        gaussianFunc->SetParLimits(3, 0, 50);
        gaussianFunc->SetParLimits(4, 0, 105);
        gaussianFunc->SetParLimits(5, 0, 105);
        gaussianFunc->SetParLimits(6, 0, 50);
        gaussianFunc->SetParLimits(7, 0, 50);
        gaussianFunc->SetParLimits(8, 0, 1e6);
        gaussianFunc->SetParLimits(9, 0, 1e6);

        gaussianFunc->SetParNames(
            "x0_1", "y0_1", "sigma_x_1", "sigma_y_1",
            "x0_2", "y0_2", "sigma_x_2", "sigma_y_2",
            "amplitude1", "amplitude2"
        );

        gaussianFunc->SetLineColor(kRed);
        gaussianFunc->SetLineWidth(2);
        gaussianFunc->SetFillColorAlpha(kBlue, 0.3);

        auto fit_res = Graph_Ptr->Fit(gaussianFunc, "RQ");
        double fit_res_integral = 0; 
        double fit_res_amp1 = 0;
        double fit_res_amp2 = 0;
        double fit_res_sigma_x_1 = 0;
        double fit_res_sigma_y_1 = 0;
        double fit_res_sigma_x_2 = 0;
        double fit_res_sigma_y_2 = 0;
        double fit_res_x0_1 = 0;
        double fit_res_y0_1 = 0;
        double fit_res_x0_2 = 0;
        double fit_res_y0_2 = 0;
        double max_error_val = 0;
        double chi2 = 0;
        double ndf = 0;
        if (fit_res == 0) {
            total_fit_success++;
            fit_res_integral = gaussianFunc->Integral(0, 105, 0, 105);
            fit_res_amp1 = gaussianFunc->GetParameter(8);
            fit_res_amp2 = gaussianFunc->GetParameter(9);
            fit_res_sigma_x_1 = gaussianFunc->GetParameter(2);
            fit_res_sigma_y_1 = gaussianFunc->GetParameter(3);
            fit_res_sigma_x_2 = gaussianFunc->GetParameter(6);
            fit_res_sigma_y_2 = gaussianFunc->GetParameter(7);
            fit_res_x0_1 = gaussianFunc->GetParameter(0);
            fit_res_y0_1 = gaussianFunc->GetParameter(1);
            fit_res_x0_2 = gaussianFunc->GetParameter(4);
            fit_res_y0_2 = gaussianFunc->GetParameter(5);

            auto _fit_sigma_1 = std::sqrt(fit_res_sigma_x_1 * fit_res_sigma_x_1 + fit_res_sigma_y_1 * fit_res_sigma_y_1);
            auto _fit_sigma_2 = std::sqrt(fit_res_sigma_x_2 * fit_res_sigma_x_2 + fit_res_sigma_y_2 * fit_res_sigma_y_2);

            fit_integral.push_back(fit_res_integral);
            fit_amp1.push_back(fit_res_amp1);
            fit_amp2.push_back(fit_res_amp2);
            fit_sigma1.push_back(_fit_sigma_1);
            fit_sigma2.push_back(_fit_sigma_2);
            fit_x0_1.push_back(fit_res_x0_1);
            fit_y0_1.push_back(fit_res_y0_1);
            fit_x0_2.push_back(fit_res_x0_2);
            fit_y0_2.push_back(fit_res_y0_2);
            fit_sigmax_1.push_back(fit_res_sigma_x_1);
            fit_sigmay_1.push_back(fit_res_sigma_y_1);
            fit_sigmax_2.push_back(fit_res_sigma_x_2);
            fit_sigmay_2.push_back(fit_res_sigma_y_2);

            chi2 = gaussianFunc->GetChisquare();
            ndf = gaussianFunc->GetNDF();
            
            for (int i = 0; i < _target_event.value_vec.size(); i++) {
                auto _x = _target_event.x_vec[i];
                auto _y = _target_event.y_vec[i];
                auto _z = _target_event.value_vec[i];
                auto _fit_z = gaussianFunc->Eval(_x, _y);
                auto _error = std::abs(_z - _fit_z);
                if (_error > max_error_val) {
                    max_error_val = _error;
                }
            }

            max_error.push_back(max_error_val);
            chi2_ndf.push_back(chi2 / ndf);
            max_chn_value.push_back(initial_params[2]);
            chn_sum.push_back(sum);
            chn_sum_N.push_back(sumN);
        }
        else {
            
        }
        if (true && !is_in_parallel){
                // LOG(INFO) << "Event " << i << " has chi2 / ndf = " << chi2 / ndf;
                Canvas_Ptr->cd();
                // Graph_Sub_Ptr->Draw("p");
                Graph_Ptr->Draw("err p0");
                gaussianFunc->Draw("surf3 same");
                Canvas_Ptr->Update();
                Canvas_Ptr->WaitPrimitive();
                Canvas_Ptr->Write();
                LOG(DEBUG) << "Event " << i << " has chi2 / ndf = " << chi2 / ndf;
        }
        
        delete Graph_Ptr;
        delete Canvas_Ptr;
    }
    LOG(INFO) << "Total expected events: "  << total_events;
    LOG(INFO) << "Total valid events: "     << total_valid_events;
    LOG(INFO) << "Total fit success: "      << total_fit_success;

    f->Close();
    delete builder;
    TFile *_save_file;
    if (job_index != 0){    
        auto run_info = "Run_" + std::to_string(run_number) + "_ho_fit_result_" + std::to_string(job_index);    
        auto _save_file_name = "../cachedFiles/" + run_info +  ".root";
        _save_file = new TFile(_save_file_name.c_str(), "RECREATE");
    }
    else {
        auto run_info = "Run_" + std::to_string(run_number) + "_fit_result";
        auto _save_file_name = "../cachedFiles/" + run_info +  ".root";
        _save_file = new TFile(_save_file_name.c_str(), "RECREATE");
    }
    
    // * Write all vector<double>
    TTree *tree = new TTree("fittree", "Fit Tree");
    tree->Branch("fit_integral", &fit_integral);
    tree->Branch("fit_amp1", &fit_amp1);
    tree->Branch("fit_amp2", &fit_amp2);
    tree->Branch("fit_sigma1", &fit_sigma1);
    tree->Branch("fit_sigma2", &fit_sigma2);
    tree->Branch("fit_x0_1", &fit_x0_1);
    tree->Branch("fit_y0_1", &fit_y0_1);
    tree->Branch("fit_x0_2", &fit_x0_2);
    tree->Branch("fit_y0_2", &fit_y0_2);
    tree->Branch("fit_sigmax_1", &fit_sigmax_1);
    tree->Branch("fit_sigmay_1", &fit_sigmay_1);
    tree->Branch("fit_sigmax_2", &fit_sigmax_2);
    tree->Branch("fit_sigmay_2", &fit_sigmay_2);
    tree->Branch("chi2_ndf", &chi2_ndf);
    tree->Branch("max_error", &max_error);
    tree->Branch("max_chn_value", &max_chn_value);
    tree->Branch("chn_sum", &chn_sum);
    tree->Branch("chn_sum_N", &chn_sum_N);
    tree->Fill();

    tree->Write();
    _save_file->Close();
    // print save file path
    LOG(INFO) << "Saved to " << _save_file->GetName();

    LOG(INFO) << "Finished fitting and plotting";
    auto success_rate = Double_t(total_fit_success) / Double_t(total_valid_events);
    LOG(INFO) << "Fit success rate: " << success_rate*100 << "%";
    // SJUtil::write_fitted_data_file(file_mixed_fitting_path, _fit_result);

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
