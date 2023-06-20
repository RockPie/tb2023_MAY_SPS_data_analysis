#include "SJ_includes.h"
#include <TPavesText.h>
#include <TGraph.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations
    int run_number = 2806;
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
        auto file_unbinned_file_name = "../cachedFiles/Run_2806_fit_result_" + std::to_string(i+1) + ".root";
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

    for (int i = 0; i < n_parallel; i++){
        for (int j = 0; j < fit_integral_parallel[i]->size(); j++){
            //if (chi2_ndf_parallel[i]->at(j) > 15000) continue;
            fit_integral.push_back(fit_integral_parallel[i]->at(j));
            fit_amp1.push_back(fit_amp1_parallel[i]->at(j));
            fit_amp2.push_back(fit_amp2_parallel[i]->at(j));
            chi2_ndf.push_back(chi2_ndf_parallel[i]->at(j));
            max_chn_value.push_back(max_chn_value_parallel[i]->at(j));
            chn_sum.push_back(chn_sum_parallel[i]->at(j));
        }
    }

    LOG(INFO) << "Total number of events: " << fit_integral.size();

    // * Data read finished
    gStyle->SetCanvasColor(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetStatColor(0);

    TCanvas *c = new TCanvas("c", "c", 1200, 900);

    // * 2-D scatter plot
    auto scatter = new TGraph();
    for (int i = 0; i < n_dots; i++){
        scatter->SetPoint(i, fit_integral[i]/25, chn_sum[i]);
        //scatter->SetPoint(i, fit_amp1[i], max_chn_value[i]);
    }

    scatter->SetTitle("Integral vs. Channel Sum");
    scatter->GetXaxis()->SetTitle("Integrated Charge [ADC]");
    scatter->GetYaxis()->SetTitle("Channel Sum [ADC]");

    scatter->GetXaxis()->SetRangeUser(0, 100000);
    scatter->GetYaxis()->SetRangeUser(0, 100000);

    // scatter->SetTitle("Amp vs. Max Chn Value");
    // scatter->GetXaxis()->SetTitle("Gaussian # 1 Amp [ADC]");
    // scatter->GetYaxis()->SetTitle("Max Chn Value [ADC]");

    // scatter->GetXaxis()->SetRangeUser(0, 16000);
    // scatter->GetYaxis()->SetRangeUser(0, 16000);

    scatter->SetMarkerStyle(20);
    scatter->SetMarkerSize(0.4);
    scatter->SetMarkerColor(kBlack);

    // add info on top right
    scatter->Draw("AP");

    TLatex *tex = new TLatex(0.64, 0.85, "Run 2806");
    tex->SetNDC();
    tex->SetTextFont(42);
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->Draw();

    TLatex *tex2 = new TLatex(0.64, 0.8, "HG/LG mixed (4,000)");
    tex2->SetNDC();
    tex2->SetTextFont(42);
    tex2->SetTextSize(0.04);
    tex2->SetLineWidth(2);
    tex2->Draw();

    auto _n_dots_info = Form("N = %d", n_dots);

    TLatex *tex3 = new TLatex(0.64, 0.75, _n_dots_info);
    tex3->SetNDC();
    tex3->SetTextFont(42);
    tex3->SetTextSize(0.04);
    tex3->SetLineWidth(2);
    tex3->Draw();

    // Transparent background
    c->SetGrid();
    // c->SaveAs("../pics/integral_channelSum.png");
    c->SaveAs("../pics/temp.png");
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