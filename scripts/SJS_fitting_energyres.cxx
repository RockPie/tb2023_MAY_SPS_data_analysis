#include "SJ_includes.h"
#include <TPavesText.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations
    int run_number = 2804;
    int n_dots = 15000;

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

    int n_parallel = 8;
    std::vector<std::string> file_unbinned_file_name_array;
    for (int i = 0; i < n_parallel; i++){
        auto file_unbinned_file_name = "../cachedFiles/Run_" + std::to_string(run_number) + "_fit_result_" + std::to_string(i+1) + ".root";
        file_unbinned_file_name_array.push_back(file_unbinned_file_name);
    }

    std::vector<TFile*> file_unbinned_file_array;
    for (int i = 0; i < n_parallel; i++){
        auto file_unbinned_file = new TFile(file_unbinned_file_name_array[i].c_str(), "READ");
        LOG(DEBUG) << "file_unbinned_file_name_array[i] = " << file_unbinned_file_name_array[i].c_str();
        file_unbinned_file_array.push_back(file_unbinned_file);
    }

    std::vector<TTree*> file_unbinned_tree_array;
    for (int i = 0; i < n_parallel; i++){
        auto file_unbinned_tree = (TTree*)file_unbinned_file_array[i]->Get("fittree");
        file_unbinned_tree_array.push_back(file_unbinned_tree);
    }

    std::vector<std::vector<double>*> fit_integral_parallel;
    std::vector<std::vector<double>*> fit_amp1_parallel;
    std::vector<std::vector<double>*> fit_amp2_parallel;
    std::vector<std::vector<double>*> chi2_ndf_parallel;
    std::vector<std::vector<double>*> max_chn_value_parallel;
    std::vector<std::vector<double>*> chn_sum_parallel;

    for (int i = 0; i < n_parallel; i++){
        std::vector<double> *fit_integral = nullptr;
        std::vector<double> *fit_amp1 = nullptr;
        std::vector<double> *fit_amp2 = nullptr;
        std::vector<double> *chi2_ndf = nullptr;
        std::vector<double> *max_chn_value = nullptr;
        std::vector<double> *chn_sum = nullptr;

        file_unbinned_tree_array[i]->SetBranchAddress("fit_integral", &fit_integral);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_amp1", &fit_amp1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_amp2", &fit_amp2);
        file_unbinned_tree_array[i]->SetBranchAddress("chi2_ndf", &chi2_ndf);
        file_unbinned_tree_array[i]->SetBranchAddress("max_chn_value", &max_chn_value);
        file_unbinned_tree_array[i]->SetBranchAddress("chn_sum", &chn_sum);

        int nentries = file_unbinned_tree_array[i]->GetEntry(0);

        fit_integral_parallel.push_back(fit_integral);
        fit_amp1_parallel.push_back(fit_amp1);
        fit_amp2_parallel.push_back(fit_amp2);
        chi2_ndf_parallel.push_back(chi2_ndf);
        max_chn_value_parallel.push_back(max_chn_value);
        chn_sum_parallel.push_back(chn_sum);
    }

    std::vector<double> fit_integral;
    std::vector<double> fit_amp1;
    std::vector<double> fit_amp2;
    std::vector<double> chi2_ndf;
    std::vector<double> max_chn_value;
    std::vector<double> chn_sum;

    auto _total_event_cnt = 0;
    auto _event_pass_chi2_cnt = 0;

    for (int i = 0; i < n_parallel; i++){
        for (int j = 0; j < fit_integral_parallel[i]->size(); j++){
            _total_event_cnt++;
            if (chi2_ndf_parallel[i]->at(j) > 2500) {
                continue;
            }
            _event_pass_chi2_cnt++;
            fit_integral.push_back(fit_integral_parallel[i]->at(j));
            fit_amp1.push_back(fit_amp1_parallel[i]->at(j));
            fit_amp2.push_back(fit_amp2_parallel[i]->at(j));
            chi2_ndf.push_back(chi2_ndf_parallel[i]->at(j));
            max_chn_value.push_back(max_chn_value_parallel[i]->at(j));
            chn_sum.push_back(chn_sum_parallel[i]->at(j));
        }
    }

    LOG(INFO) << "Total number of events: " << fit_integral.size();


    auto xMax_integrated = 40000;
    gStyle->SetCanvasColor(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetStatColor(0);

    TH1D *h = new TH1D("h", "h", 200, 0, xMax_integrated);
    for (int i = 0; i < fit_integral.size(); i++){
        h->Fill(fit_integral.at(i)/25);
    }

    TCanvas *c = new TCanvas("c", "c", 1200, 900);
    h->SetTitle("Integrated charge distribution");
    // h->SetTitle("Amplitude distribution");
    h->GetXaxis()->SetTitle("Integrated charge [ADC]");
    // h->GetXaxis()->SetTitle("Amplitude [ADC]");
    h->GetYaxis()->SetTitle("Normalized count");
    h->Scale(1.0/h->Integral());
    h->GetXaxis()->SetRangeUser(0, xMax_integrated);
    h->SetLineWidth(3);
    h->SetLineStyle(1);
    h->SetStats(1);
    // fit with a 1-d gaussian
    auto gaus = new TF1("gaus", "gaus", 7000, 15000);
    h->Fit(gaus, "R");
    gaus->SetLineColor(kRed);
    gaus->SetLineWidth(3);
    gaus->SetLineStyle(2);

    // get results
    auto mean = gaus->GetParameter(1);
    auto sigma = gaus->GetParameter(2);
    auto mean_err = gaus->GetParError(1);
    auto sigma_err = gaus->GetParError(2);
    auto resolution = sigma * 100.0 /mean;
    // h->Draw("p");
    h->Draw("p");
    gaus->Draw("same");
    gPad -> Update();

    // show results on plot
    auto text = new TLatex();
    text->SetNDC();
    text->SetTextFont(43);
    text->SetTextSize(30);
    text->SetTextColor(kRed);
    text->DrawLatex(0.6, 0.7, Form("mu = %.2f #pm %.2f", mean, mean_err));
    text->DrawLatex(0.6, 0.65, Form("sigma = %.2f #pm %.2f", sigma, sigma_err));
    text->DrawLatex(0.6, 0.6, Form("Resolution = %.2f %%", resolution));
    // Transparent background
    c->SetGrid();

    auto _plot_name = "../pics/temp_distribution" + std::to_string(run_number) + ".png";
    c->SaveAs(_plot_name.c_str());

    auto _pass_rate = (double)_event_pass_chi2_cnt * 100 / (double)_total_event_cnt;
    LOG(INFO) << "Chi2 pass rate: " << _pass_rate << "%";

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