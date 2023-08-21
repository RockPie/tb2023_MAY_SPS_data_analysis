#include "SJ_includes.h"
#include <TPavesText.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations
    const int    run_number         = 2798;
    const int    n_parallel         = 10;
    const int    n_bins             = 200;
    const int    hist_xmax          = 100000;
    const bool   is_chi2_filtering      = false;
    const double chi2_ndf_threshold     = 200;
    const bool   is_sigma2_filtering    = false;
    const double sigma2_threshold       = 1000;
    const bool   is_distance_filtering  = false;
    const double distance_threshold     = 10;
    // * Fit range for run 2798
    // const double fit_xmin   = 38000;
    // const double fit_xmax   = 61000;
    const double fit_xmin   = 40000;
    const double fit_xmax   = 62000;
    const double fit_xmin_fit   = 45000;
    const double fit_xmax_fit   = 67000;
    const double fit_xmin_sum = 50000;
    const double fit_xmax_sum = 72000;
    // * Fit range for run 2799
    // const double fit_xmin   = 31500;
    // const double fit_xmax   = 51000;
    // const double fit_xmin   = 34500;
    // const double fit_xmax   = 54000;
    // const double fit_xmin_sum   = 44500;
    // const double fit_xmax_sum   = 64000;

    // * Fit range for run 2800
    // const double fit_xmin   = 23000;
    // const double fit_xmax   = 46000;
    // const double fit_xmin_sum   = 33000;
    // const double fit_xmax_sum   = 56000;
    // * Fit range for run 2801
    // const double fit_xmin   = 21000;
    // const double fit_xmax   = 35000;
    // const double fit_xmin   = 22000;
    // const double fit_xmax   = 37000;
    // * Fit range for run 2802
    // const double fit_xmin = 14000;
    // const double fit_xmax = 26000;
    // const double fit_xmin = 16000;
    // const double fit_xmax = 29000;
    // * Fit range for run 2803
    // const double fit_xmin = 9000;
    // const double fit_xmax = 18000;
    // const double fit_xmin = 10000;
    // const double fit_xmax = 19000;
    // * Fit range for run 2804
    // const double fit_xmin   = 7000;
    // const double fit_xmax   = 14000;
    // const double fit_xmin   = 7500;
    // const double fit_xmax   = 16000;
    // const double fit_xmin_sum   = 10500;
    // const double fit_xmax_sum   = 19000;
    // * Fit range for run 2805
    // const double fit_xmin   = 3500;
    // const double fit_xmax   = 9000;
    // const double fit_xmin   = 3500;
    // const double fit_xmax   = 11000;
    // * Fit range for run 2806
    // const double fit_xmin   = 27000;
    // const double fit_xmax   = 45000;
    // const double fit_xmin   = 28000;
    // const double fit_xmax   = 46000;
    // const double fit_xmin_sum   = 38000;
    // const double fit_xmax_sum   = 56000;

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

    std::vector<std::string> file_unbinned_file_name_array;
    for (int i = 0; i < n_parallel; i++){
        auto file_unbinned_file_name = "../cachedFiles/Run_" + std::to_string(run_number) + "_trp_fit_result_" + std::to_string(i+1) + ".root";
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
    std::vector<std::vector<double>*> fit_sigma1_parallel;
    std::vector<std::vector<double>*> fit_sigma2_parallel;
    std::vector<std::vector<double>*> fit_x0_1_parallel;
    std::vector<std::vector<double>*> fit_x0_2_parallel;
    std::vector<std::vector<double>*> fit_y0_1_parallel;
    std::vector<std::vector<double>*> fit_y0_2_parallel;
    std::vector<std::vector<double>*> chi2_ndf_parallel;
    std::vector<std::vector<double>*> max_chn_value_parallel;
    std::vector<std::vector<double>*> chn_sum_parallel;
    std::vector<std::vector<double>*> chn_sum_N_parallel;

    for (int i = 0; i < n_parallel; i++){
        std::vector<double> *fit_integral   = nullptr;
        std::vector<double> *fit_amp1       = nullptr;
        std::vector<double> *fit_amp2       = nullptr;
        std::vector<double> *fit_sigma1     = nullptr;
        std::vector<double> *fit_sigma2     = nullptr;
        std::vector<double> *fit_x0_1       = nullptr;
        std::vector<double> *fit_x0_2       = nullptr;
        std::vector<double> *fit_y0_1       = nullptr;
        std::vector<double> *fit_y0_2       = nullptr;
        std::vector<double> *chi2_ndf       = nullptr;
        std::vector<double> *max_chn_value  = nullptr;
        std::vector<double> *chn_sum        = nullptr;
        std::vector<double> *chn_sum_N      = nullptr;

        file_unbinned_tree_array[i]->SetBranchAddress("fit_integral", &fit_integral);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_amp1", &fit_amp1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_amp2", &fit_amp2);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_sigma1", &fit_sigma1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_sigma2", &fit_sigma2);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_x0_1", &fit_x0_1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_x0_2", &fit_x0_2);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_y0_1", &fit_y0_1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_y0_2", &fit_y0_2);
        file_unbinned_tree_array[i]->SetBranchAddress("chi2_ndf", &chi2_ndf);
        file_unbinned_tree_array[i]->SetBranchAddress("max_chn_value", &max_chn_value);
        file_unbinned_tree_array[i]->SetBranchAddress("chn_sum", &chn_sum);
        file_unbinned_tree_array[i]->SetBranchAddress("chn_sum_N", &chn_sum_N);

        int nentries = file_unbinned_tree_array[i]->GetEntry(0);

        fit_integral_parallel.push_back(fit_integral);
        fit_amp1_parallel.push_back(fit_amp1);
        fit_amp2_parallel.push_back(fit_amp2);
        fit_sigma1_parallel.push_back(fit_sigma1);
        fit_sigma2_parallel.push_back(fit_sigma2);
        fit_x0_1_parallel.push_back(fit_x0_1);
        fit_x0_2_parallel.push_back(fit_x0_2);
        fit_y0_1_parallel.push_back(fit_y0_1);
        fit_y0_2_parallel.push_back(fit_y0_2);
        chi2_ndf_parallel.push_back(chi2_ndf);
        max_chn_value_parallel.push_back(max_chn_value);
        chn_sum_parallel.push_back(chn_sum);
        chn_sum_N_parallel.push_back(chn_sum_N);
    }

    std::vector<double> fit_integral;
    std::vector<double> fit_amp1;
    std::vector<double> fit_amp2;
    std::vector<double> fit_sigma1;
    std::vector<double> fit_sigma2;
    std::vector<double> fit_x0_1;
    std::vector<double> fit_x0_2;
    std::vector<double> fit_y0_1;
    std::vector<double> fit_y0_2;
    std::vector<double> chi2_ndf;
    std::vector<double> max_chn_value;
    std::vector<double> chn_sum;
    std::vector<double> chn_sum_N;

    auto _total_event_cnt = 0;
    auto _event_pass_chi2_cnt = 0;

    for (int i = 0; i < n_parallel; i++){
        for (int j = 0; j < fit_integral_parallel[i]->size(); j++){
            _total_event_cnt++;
            if (chi2_ndf_parallel[i]->at(j) > chi2_ndf_threshold) {
                if (is_chi2_filtering)
                    continue;
            }
            if (fit_sigma2_parallel[i]->at(j) < sigma2_threshold) {
                if (is_sigma2_filtering)
                    continue;
            }
            auto distance = std::sqrt(std::pow(fit_x0_1_parallel[i]->at(j) - fit_x0_2_parallel[i]->at(j), 2) +
                                      std::pow(fit_y0_1_parallel[i]->at(j) - fit_y0_2_parallel[i]->at(j), 2));
            if (distance > distance_threshold) {
                if (is_distance_filtering)
                    continue;
            }
            _event_pass_chi2_cnt++;
            fit_integral.push_back(fit_integral_parallel[i]->at(j));
            fit_amp1.push_back(fit_amp1_parallel[i]->at(j));
            fit_amp2.push_back(fit_amp2_parallel[i]->at(j));
            fit_sigma1.push_back(fit_sigma1_parallel[i]->at(j));
            fit_sigma2.push_back(fit_sigma2_parallel[i]->at(j));
            fit_x0_1.push_back(fit_x0_1_parallel[i]->at(j));
            fit_x0_2.push_back(fit_x0_2_parallel[i]->at(j));
            fit_y0_1.push_back(fit_y0_1_parallel[i]->at(j));
            fit_y0_2.push_back(fit_y0_2_parallel[i]->at(j));
            chi2_ndf.push_back(chi2_ndf_parallel[i]->at(j));
            max_chn_value.push_back(max_chn_value_parallel[i]->at(j));
            chn_sum.push_back(chn_sum_parallel[i]->at(j));
            chn_sum_N.push_back(chn_sum_N_parallel[i]->at(j));
        }
    }

    LOG(INFO) << "Total number of events: " << fit_integral.size();


    gStyle->SetCanvasColor(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetStatColor(0);

    TH1D *h = new TH1D("h", "h", n_bins, 0, hist_xmax);
    TH1D *h_sum = new TH1D("h_sum", "h_sum", n_bins, 0, hist_xmax);
    TH1D *h_sumN = new TH1D("h_sumN", "h_sumN", n_bins, 0, hist_xmax);

    for (int i = 0; i < fit_integral.size(); i++){
        h->Fill(fit_integral.at(i)/25);
    }
    for (int i = 0; i < chn_sum.size(); i++){
        h_sum->Fill(chn_sum.at(i));
    }
    for (int i = 0; i < chn_sum_N.size(); i++){
        h_sumN->Fill(chn_sum_N.at(i));
    }

    TCanvas *c = new TCanvas("c", "c", 1800, 1400);
    h->SetTitle("Energy distribution comparison");
    // h->SetTitle("Amplitude distribution");
    h->GetXaxis()->SetTitle("Integrated charge [ADC]");
    // h->GetXaxis()->SetTitle("Amplitude [ADC]");
    h->GetYaxis()->SetTitle("Normalized count");
    h->Scale(1.0/h->Integral());
    h->GetXaxis()->SetRangeUser(0, hist_xmax);
    h->GetYaxis()->SetRangeUser(0.00003, 0.03);
    h->SetLineColor(kBlue);
    h->SetLineWidth(3);
    h->SetLineStyle(1);
    h->SetStats(0);

    h_sum->Scale(1.0/h_sum->Integral());
    h_sum->SetLineColor(kRed);
    h_sum->SetLineWidth(3);
    h_sum->SetLineStyle(1);
    h_sum->SetStats(0);

    h_sumN->Scale(1.0/h_sumN->Integral());
    h_sumN->SetLineColor(kOrange);
    h_sumN->SetLineWidth(3);
    h_sumN->SetLineStyle(1);
    h_sumN->SetStats(0);
    // fit with a 1-d gaussian
    auto gaus = new TF1("gaus", "gaus", fit_xmin_fit, fit_xmax_fit);
    auto gaus_sum = new TF1("gaus_sum", "gaus", fit_xmin, fit_xmax);
    auto gaus_sumN = new TF1("gaus_sum", "gaus", fit_xmin_sum, fit_xmax_sum);

    gaus->SetLineColor(kCyan);
    gaus->SetLineWidth(3);
    gaus->SetLineStyle(2);
    gaus_sum->SetLineColor(kViolet);
    gaus_sum->SetLineWidth(3);
    gaus_sum->SetLineStyle(2);
    gaus_sumN->SetLineColor(kGreen);
    gaus_sumN->SetLineWidth(3);
    gaus_sumN->SetLineStyle(2);

    h->Fit(gaus, "R");
    h_sum->Fit(gaus_sum, "R");
    h_sumN->Fit(gaus_sumN, "R");

    auto chi2_ndf_str = Form("#chi^{2}/NDF = %.2f", gaus->GetChisquare()/gaus->GetNDF());

    // get results
    auto mean = gaus->GetParameter(1);
    auto sigma = gaus->GetParameter(2);
    auto mean_err = gaus->GetParError(1);
    auto sigma_err = gaus->GetParError(2);
    auto resolution = sigma * 100.0 /mean;

    auto mean_sum = gaus_sum->GetParameter(1);
    auto sigma_sum = gaus_sum->GetParameter(2);
    auto mean_err_sum = gaus_sum->GetParError(1);
    auto sigma_err_sum = gaus_sum->GetParError(2);
    auto resolution_sum = sigma_sum * 100.0 /mean_sum;

    auto mean_sumN = gaus_sumN->GetParameter(1);
    auto sigma_sumN = gaus_sumN->GetParameter(2);
    auto mean_err_sumN = gaus_sumN->GetParError(1);
    auto sigma_err_sumN = gaus_sumN->GetParError(2);
    auto resolution_sumN = sigma_sumN * 100.0 /mean_sumN;
    
    // h->Draw("p");
    h->Draw("p");
    h_sum->Draw("p same");
    h_sumN->Draw("p same");
    // gaus->Draw("same");
    gPad -> Update();

    // show results on plot
    auto text = new TLatex();
    auto text_sum = new TLatex();
    auto text_sumN = new TLatex();
    text->SetNDC();
    text_sum->SetNDC();
    text_sumN->SetNDC();
    // align: 10: left, 20: center, 30: right
    // align: 1: bottom, 2: middle, 3: top
    text->SetTextAlign(33);
    text->SetTextFont(43);
    text->SetTextSize(32);
    text->SetTextColor(kBlack);

    text_sum->SetTextAlign(13); // align: 10: left, 20: center, 30: right
    text_sum->SetTextFont(43);
    text_sum->SetTextSize(32);
    text_sum->SetTextColor(kRed);

    text_sumN->SetTextAlign(13); // align: 10: left, 20: center, 30: right
    text_sumN->SetTextFont(43);
    text_sumN->SetTextSize(32);
    text_sumN->SetTextColor(kOrange+4);

    auto start_latex_x = 0.89;
    auto start_latex_y = 0.87;
    auto step = 0.03;
    auto step_cnt = 0;

    text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("Run %d", run_number));
    step_cnt++;
    text->SetTextColor(kBlue);
    text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("events= %lu", fit_integral.size()));
    step_cnt++;
    text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("mu = %.2f #pm %.2f", mean, mean_err));
    step_cnt++;
    text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("sigma = %.2f #pm %.2f", sigma, sigma_err));
    step_cnt++;
    text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("resolution = %.2f %%", resolution));
    step_cnt++;
    text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("chi2/ndf = %.2f / %d", gaus->GetChisquare(), gaus->GetNDF()));
    if (is_chi2_filtering) {
        step_cnt++;
        text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
            Form("chi2 pass rate = %.2f %%", (double)_event_pass_chi2_cnt * 100 / (double)_total_event_cnt));
    }
    if (is_sigma2_filtering) {
        step_cnt++;
        text->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
            Form("sigma2 pass rate = %.2f %%", (double)_event_pass_chi2_cnt * 100 / (double)_total_event_cnt));
    }

    start_latex_x = 0.13;
    start_latex_y = 0.64;
    step = 0.03;
    step_cnt = 0;

    text_sum->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("mu = %.2f #pm %.2f", mean_sum, mean_err_sum));
    step_cnt++;
    text_sum->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("sigma = %.2f #pm %.2f", sigma_sum, sigma_err_sum));
    step_cnt++;
    text_sum->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("resolution = %.2f %%", resolution_sum));
    step_cnt++;
    text_sum->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("chi2/ndf = %.2f / %d", gaus_sum->GetChisquare(), gaus_sum->GetNDF()));

    start_latex_x = 0.41;
    start_latex_y = 0.40;
    step = 0.03;
    step_cnt = 0;

    text_sumN->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("mu = %.2f #pm %.2f", mean_sumN, mean_err_sumN));
    step_cnt++;
    text_sumN->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("sigma = %.2f #pm %.2f", sigma_sumN, sigma_err_sumN));
    step_cnt++;
    text_sumN->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("resolution = %.2f %%", resolution_sumN));
    step_cnt++;
    text_sumN->DrawLatex(start_latex_x, start_latex_y - step_cnt*step, 
        Form("chi2/ndf = %.2f / %d", gaus_sumN->GetChisquare(), gaus_sumN->GetNDF()));

    // Transparent background
    c->SetGrid();
    c->SetLogy();

    auto legend = new TLegend(0.1, 0.75, 0.35, 0.9);
    legend->AddEntry(h, "Fitted", "lep");
    legend->AddEntry(h_sum, "Sum Normalized", "lep");
    legend->AddEntry(h_sumN, "Sum", "lep");

    legend->SetTextSize(0.03);
    legend->SetBorderSize(1);
    //legend->Draw();

    auto _plot_name = "../pics/temp2_histCompare" + std::to_string(run_number) + ".png";
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