#include "SJ_includes.h"

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    // * -n 1 - 10: index of this job
    // analyse input arguments
    int opt;
    int job_index   = 0; // -n 1 - 10: index of this job
    int job_num     = 8; // -t 1 - 10: total number of jobs
    int eventNum    = 100; // -e 1 - 1000: number of events to be processed

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

    set_easylogger();   // * set easylogging++ configurations
    int run_number = 2806;

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
    std::vector<Double_t> chi2_ndf;

    //* multi CPU fitting

    auto first_event = 0;

    if (job_index != 0){
        first_event = int(eventNum / job_num * job_index);
        eventNum = int(eventNum / job_num * (job_index + 1));
    }

    for (auto i = first_event; i < eventNum; i++){
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

        std::vector<Double_t> HG_charges_double;
        for (auto i = 0; i < HG_charges.size(); i++)
            HG_charges_double.push_back(Double_t(HG_charges[i]));
        auto LG_charges_Multipled   = SJUtil::gain_multiplication(slopeInfo, offsetInfo, LG_charges);
        auto _twoD_hg_values        = SJUtil::map1d_to_2d(HG_charges_double, mapping_coords);
        auto _twoD_lg_values        = SJUtil::map1d_to_2d(LG_charges_Multipled, mapping_coords);
        auto _twoD_hg_values_N      = SJUtil::noise_subtracted_data(_twoD_hg_values, 1);
        auto _twoD_lg_values_N      = SJUtil::noise_subtracted_data(_twoD_lg_values, 1);
        auto _twoD_hg_values_NA     = SJUtil::area_normalized_data(_twoD_hg_values_N);
        auto _twoD_lg_values_NA     = SJUtil::area_normalized_data(_twoD_lg_values_N);
        auto _target_event          = SJUtil::substitued_data(
            _twoD_hg_values_NA, 
            _twoD_lg_values_NA,
            Double_t(1500),
            Double_t(1)
        );
        // auto _target_event = _twoD_hg_values_NA;
        if ( _target_event.value_vec.size() <= fit_param_num + 1)  
            continue;
        total_valid_events++;

        auto Graph_Ptr      = SJPlot::scatter_3d_raw( 
            _target_event, _currentName, _currentTitle);
        Graph_Ptr->SetMarkerColor(kBlue);
        auto Graph_Sub_Ptr      = SJPlot::scatter_3d_raw( 
            _twoD_lg_values_NA, _currentName, _currentTitle);
        Graph_Sub_Ptr->SetMarkerColor(kRed);
        Graph_Sub_Ptr->SetMarkerStyle(21);

        // LOG(INFO) << "Fitting event " << i;
        
        TF2 *gaussianFunc = new TF2("gaussianFunc", SJFunc::dual_gaussian2D, 0, 105, 0, 105, fit_param_num);
        gaussianFunc->SetParameters(
            52.5, 52.5, 5, 5,
            52.5, 52.5, 15, 15,
            4096, 512
        );

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
        double chi2 = 0;
        double ndf = 0;
        if (fit_res == 0) {
            total_fit_success++;
            fit_res_integral = gaussianFunc->Integral(0, 105, 0, 105);
            fit_res_amp1 = gaussianFunc->GetParameter(8);
            fit_res_amp2 = gaussianFunc->GetParameter(9);
            fit_integral.push_back(fit_res_integral);
            fit_amp1.push_back(fit_res_amp1);
            fit_amp2.push_back(fit_res_amp2);
            chi2 = gaussianFunc->GetChisquare();
            ndf = gaussianFunc->GetNDF();
            chi2_ndf.push_back(chi2 / ndf);
            if (true){
                LOG(INFO) << "Event " << i << " has chi2 / ndf = " << chi2 / ndf;
                Canvas_Ptr->cd();
                Graph_Sub_Ptr->Draw("p");
                Graph_Ptr->Draw("p same");
                gaussianFunc->Draw("surf3 same");
                Canvas_Ptr->Update();
                Canvas_Ptr->WaitPrimitive();
                Canvas_Ptr->Write();
            }
        }
        
        
        // * Set the z axis range
        
        delete Graph_Ptr;
        delete Canvas_Ptr;

    }
    LOG(INFO) << "Total expected events: "  << total_events;
    LOG(INFO) << "Total valid events: "     << total_valid_events;
    LOG(INFO) << "Total fit success: "      << total_fit_success;

    f->Close();
    delete builder;

    if (job_index != 0){
        auto _save_file_name = "../cachedFiles/Run_2806_fit_result_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name.c_str(), fit_integral);
        auto _save_file_name1 = "../cachedFiles/Run_2806_amp1_result_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name1.c_str(), fit_amp1);
        auto _save_file_name2 = "../cachedFiles/Run_2806_amp2_result_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name2.c_str(), fit_amp2);
        auto _save_file_name3 = "../cachedFiles/Run_2806_chi2_ndf_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name3.c_str(), chi2_ndf);
    }
    else {
        auto _save_file_name = "../cachedFiles/Run_2806_fit_result.root";
        SJUtil::write_double_array_to_file(_save_file_name, fit_integral);
        auto _save_file_name1 = "../cachedFiles/Run_2806_amp1_result.root";
        SJUtil::write_double_array_to_file(_save_file_name1, fit_amp1);
        auto _save_file_name2 = "../cachedFiles/Run_2806_amp2_result.root";
        SJUtil::write_double_array_to_file(_save_file_name2, fit_amp2);
        auto _save_file_name3 = "../cachedFiles/Run_2806_chi2_ndf.root";
        SJUtil::write_double_array_to_file(_save_file_name3, chi2_ndf);
    }
    

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

