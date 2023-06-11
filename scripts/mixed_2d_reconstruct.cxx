#include "SJ_includes.h"

// TODO: Fix cmakelists.txt with correct project name

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    // * -n 1 - 10: index of this job
    // analyse input arguments
    int opt;
    int job_index   = 0; // -n 1 - 10: index of this job
    int job_num     = 10; // -t 1 - 10: total number of jobs
    int eventNum    = 10; // -e 1 - 1000: number of events to be processed

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
    std::vector<Double_t> fit_chi2;
    std::vector<Double_t> fit_ndf;

    RooRealVar x("x", "x", 0, 105);
    RooRealVar y("y", "y", 0, 105);

    RooRealVar mean1_x("mean_x", "mean_x", 52.5, 0, 105);
    RooRealVar mean1_y("mean_y", "mean_y", 52.5, 0, 105);

    RooRealVar sigma1_x("sigma_x", "sigma_x", 5, 0.1, 52.5);
    RooRealVar sigma1_y("sigma_y", "sigma_y", 5, 0.1, 52.5);

    RooRealVar mean2_x("mean2_x", "mean2_x", 52.5, 0, 105);
    RooRealVar mean2_y("mean2_y", "mean2_y", 52.5, 0, 105);

    RooRealVar sigma2_x("sigma2_x", "sigma2_x", 15, 0.1, 52.5);
    RooRealVar sigma2_y("sigma2_y", "sigma2_y", 15, 0.1, 52.5);

    RooRealVar amplitude2("amplitude2", "amplitude2", 0.7, 0.001, 0.999);

    RooGaussian gauss1_x("gauss_x", "gauss_x", x, mean1_x, sigma1_x);
    RooGaussian gauss1_y("gauss_y", "gauss_y", y, mean1_y, sigma1_y);

    RooGaussian gauss2_x("gauss2_x", "gauss2_x", x, mean2_x, sigma2_x);
    RooGaussian gauss2_y("gauss2_y", "gauss2_y", y, mean2_y, sigma2_y);

    RooProdPdf gauss1_2d("gauss_2d", "gauss_2d", RooArgList(gauss1_x, gauss1_y));
    RooProdPdf gauss2_2d("gauss2_2d", "gauss2_2d", RooArgList(gauss2_x, gauss2_y));

    RooAddPdf double_gauss_2d("double_gauss_2d", "double_gauss_2d", RooArgList(gauss1_2d, gauss2_2d), amplitude2);

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
        //auto Canvas_Ptr     = new TCanvas(
        //    _currentName, _currentTitle, 200, 10, 700, 500);
        
        std::vector<Double_t> HG_charges_double;
        for (auto i = 0; i < HG_charges.size(); i++)
            HG_charges_double.push_back(Double_t(HG_charges[i]));
        auto LG_charges_Multipled   = SJUtil::gain_multiplication(slopeInfo, offsetInfo, LG_charges);
        auto _twoD_hg_values        = SJUtil::map1d_to_2d(HG_charges_double, mapping_coords);
        auto _twoD_lg_values        = SJUtil::map1d_to_2d(LG_charges_Multipled, mapping_coords);
        // auto _twoD_hg_values_N      = SJUtil::noise_subtracted_data(_twoD_hg_values, 10);
        // auto _twoD_lg_values_N      = SJUtil::noise_subtracted_data(_twoD_lg_values, 10);
        // auto _twoD_hg_values_A     = SJUtil::area_normalized_data(_twoD_hg_values);
        // auto _twoD_lg_values_A     = SJUtil::area_normalized_data(_twoD_lg_values);
        auto _target_event       = SJUtil::substitued_data(
            _twoD_hg_values, 
            _twoD_lg_values,
            Double_t(1500),
            Double_t(1)
        );
        // auto _target_event = _twoD_values_NAM;

        if ( _target_event.value_vec.size() <= fit_param_num + 1)  
            continue;
        total_valid_events++;

        LOG(INFO) << "Fitting event " << i;
        auto _hist_2d = SJUtil::get_2d_histogram(_target_event.x_vec, _target_event.y_vec, _target_event.value_vec, _currentName, _currentTitle);
        // TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 700, 500);
        // _hist_2d->Draw("colz");
        // c1->Update();
        // c1->Write();
        // delete c1;
        
        
        RooDataHist data("data", "data", RooArgList(x, y), _hist_2d);

        // set the initial values
        mean1_x.setVal(52.5);
        mean1_y.setVal(52.5);
        sigma1_x.setVal(6);
        sigma1_y.setVal(6);
        mean2_x.setVal(52.5);
        mean2_y.setVal(52.5);
        sigma2_x.setVal(15);
        sigma2_y.setVal(15);
        amplitude2.setVal(0.7);

        // choose the fitting method
        // * Speed up the fitting by using the Hesse matrix
        // * Further speed up the fitting by using the Optimize and NumCPU


        RooFitResult *fit_result = double_gauss_2d.chi2FitTo(data, RooFit::Save(), RooFit::PrintLevel(-1), RooFit::Optimize());
        // use multi-thread fitting for 10 core CPU
        // RooFitResult *fit_result = double_gauss_2d.fitTo(data, RooFit::Save(false), RooFit::PrintLevel(-1), RooFit::SumW2Error(kTRUE), RooFit::NumCPU(10));
        // RooFitResult *fit_result = double_gauss_2d.fitTo(data, RooFit::Save(), RooFit::PrintLevel(-1), SumW2Error(kTRUE));

        // tell if the fit is valid
        if (fit_result->status() == 0) {
            total_fit_success++;
        }

        // //! calculate the integral of the 2D gaussian
        RooArgSet vars(x, y);
        RooAbsReal *integral = double_gauss_2d.createIntegral(vars);
        Double_t integral_value = integral->getVal();
        fit_integral.push_back(integral_value);

        // get chi2
        Double_t chi2 = double_gauss_2d.createChi2(data)->getVal();
        fit_chi2.push_back(chi2);
        //LOG(INFO) << "Chi2: " << chi2;
        // get ndf
        Int_t ndf = data.numEntries() - double_gauss_2d.getParameters(data)->selectByAttrib("Constant", kTRUE)->getSize();
        fit_ndf.push_back(ndf);
        //LOG(INFO) << "NDF: " << ndf;


        // RooPlot *xframe = x.frame(RooFit::Title("2D Gaussian Fit"), RooFit::Bins(105));
        // data.plotOn(xframe);
        // double_gauss_2d.plotOn(xframe);

        // RooPlot *yframe = y.frame(RooFit::Title("2D Gaussian Fit"), RooFit::Bins(105));
        // data.plotOn(yframe);
        // double_gauss_2d.plotOn(yframe);

        // TCanvas *cx = new TCanvas("cx", "cx", 200, 10, 700, 500);
        // xframe->Draw();
        // cx->Write();
        // cx->Close();

        // TCanvas *cy = new TCanvas("cy", "cy", 200, 10, 700, 500);
        // yframe->Draw();
        // cy->Write();
        // cy->Close();

        // delete xframe;
        // delete yframe;

        // delete in-loop variables
        delete integral;
        delete fit_result;
        delete _hist_2d;
    }
    LOG(INFO) << "Total expected events: "  << total_events;
    LOG(INFO) << "Total valid events: "     << total_valid_events;

    f->Close();
    delete builder;

    if (job_index != 0){
        auto _save_file_name = "../cachedFiles/Run_2806_Roofit_res_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name.c_str(), fit_integral);
        auto _save_file_name1 = "../cachedFiles/Run_2806_Roochi2_res_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name1.c_str(), fit_chi2);
        auto _save_file_name2 = "../cachedFiles/Run_2806_Roondf_res_" + std::to_string(job_index) + ".root";
        SJUtil::write_double_array_to_file(_save_file_name2.c_str(), fit_ndf);
    }
    else {
        auto _save_file_name = "../cachedFiles/Run_2806_Roofit_res.root";
        SJUtil::write_double_array_to_file(_save_file_name, fit_integral);
        auto _save_file_name1 = "../cachedFiles/Run_2806_Roochi2_res.root";
        SJUtil::write_double_array_to_file(_save_file_name1, fit_chi2);
        auto _save_file_name2 = "../cachedFiles/Run_2806_Roondf_res.root";
        SJUtil::write_double_array_to_file(_save_file_name2, fit_ndf);
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

