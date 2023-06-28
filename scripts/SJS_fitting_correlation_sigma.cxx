#include "SJ_includes.h"
#include <TPavesText.h>
#include <TGraph.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations
    int run_number = 2798;
    int n_dots = 2400;
    int n_parallel = 10;

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
    std::vector<std::vector<double>*> max_error_parallel;
    std::vector<std::vector<double>*> fit_sigma1_parallel;
    std::vector<std::vector<double>*> fit_sigma2_parallel;
    std::vector<std::vector<double>*> fit_x0_1_parallel;
    std::vector<std::vector<double>*> fit_x0_2_parallel;
    std::vector<std::vector<double>*> fit_y0_1_parallel;
    std::vector<std::vector<double>*> fit_y0_2_parallel;

    for (int i = 0; i < n_parallel; i++){
        std::vector<double> *fit_integral = nullptr;
        std::vector<double> *fit_amp1 = nullptr;
        std::vector<double> *fit_amp2 = nullptr;
        std::vector<double> *chi2_ndf = nullptr;
        std::vector<double> *max_chn_value = nullptr;
        std::vector<double> *chn_sum = nullptr;
        std::vector<double> *max_error = nullptr;
        std::vector<double> *fit_sigma1 = nullptr;
        std::vector<double> *fit_sigma2 = nullptr;
        std::vector<double> *fit_x0_1 = nullptr;
        std::vector<double> *fit_x0_2 = nullptr;
        std::vector<double> *fit_y0_1 = nullptr;
        std::vector<double> *fit_y0_2 = nullptr;

        file_unbinned_tree_array[i]->SetBranchAddress("fit_integral", &fit_integral);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_amp1", &fit_amp1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_amp2", &fit_amp2);
        file_unbinned_tree_array[i]->SetBranchAddress("chi2_ndf", &chi2_ndf);
        file_unbinned_tree_array[i]->SetBranchAddress("max_chn_value", &max_chn_value);
        file_unbinned_tree_array[i]->SetBranchAddress("chn_sum", &chn_sum);
        file_unbinned_tree_array[i]->SetBranchAddress("max_error", &max_error);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_sigma1", &fit_sigma1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_sigma2", &fit_sigma2);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_x0_1", &fit_x0_1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_x0_2", &fit_x0_2);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_y0_1", &fit_y0_1);
        file_unbinned_tree_array[i]->SetBranchAddress("fit_y0_2", &fit_y0_2);

        int nentries = file_unbinned_tree_array[i]->GetEntry(0);

        fit_integral_parallel.push_back(fit_integral);
        fit_amp1_parallel.push_back(fit_amp1);
        fit_amp2_parallel.push_back(fit_amp2);
        chi2_ndf_parallel.push_back(chi2_ndf);
        max_chn_value_parallel.push_back(max_chn_value);
        chn_sum_parallel.push_back(chn_sum);
        max_error_parallel.push_back(max_error);
        fit_sigma1_parallel.push_back(fit_sigma1);
        fit_sigma2_parallel.push_back(fit_sigma2);
        fit_x0_1_parallel.push_back(fit_x0_1);
        fit_x0_2_parallel.push_back(fit_x0_2);
        fit_y0_1_parallel.push_back(fit_y0_1);
        fit_y0_2_parallel.push_back(fit_y0_2);
    }

    std::vector<double> fit_integral;
    std::vector<double> fit_amp1;
    std::vector<double> fit_amp2;
    std::vector<double> chi2_ndf;
    std::vector<double> max_chn_value;
    std::vector<double> chn_sum;
    std::vector<double> max_error;
    std::vector<double> fit_sigma1;
    std::vector<double> fit_sigma2;
    std::vector<double> fit_x0_1;
    std::vector<double> fit_x0_2;
    std::vector<double> fit_y0_1;
    std::vector<double> fit_y0_2;

    std::vector<double> chn_sum_chi2_0;
    std::vector<double> chn_sum_chi2_1;
    std::vector<double> chn_sum_chi2_2;
    std::vector<double> chn_sum_chi2_3;
    std::vector<double> chn_sum_chi2_4;

    std::vector<double> fit_sigma1_chi2_0;
    std::vector<double> fit_sigma1_chi2_1;
    std::vector<double> fit_sigma1_chi2_2;
    std::vector<double> fit_sigma1_chi2_3;
    std::vector<double> fit_sigma1_chi2_4;

    std::vector<double> fit_sigma2_chi2_0;
    std::vector<double> fit_sigma2_chi2_1;
    std::vector<double> fit_sigma2_chi2_2;
    std::vector<double> fit_sigma2_chi2_3;
    std::vector<double> fit_sigma2_chi2_4;

    std::vector<double> fit_distance_chi2_0;
    std::vector<double> fit_distance_chi2_1;
    std::vector<double> fit_distance_chi2_2;
    std::vector<double> fit_distance_chi2_3;
    std::vector<double> fit_distance_chi2_4;

    std::vector<double> fit_integral_chi2_0;
    std::vector<double> fit_integral_chi2_1;
    std::vector<double> fit_integral_chi2_2;
    std::vector<double> fit_integral_chi2_3;
    std::vector<double> fit_integral_chi2_4;

    const double chi2_ndf_cut0 = 7400;
    const double chi2_ndf_cut1 = 11300;
    const double chi2_ndf_cut2 = 17000;
    const double chi2_ndf_cut3 = 27000;


    for (int i = 0; i < n_parallel; i++){
        for (int j = 0; j < n_dots; j++){
           
            fit_integral.push_back(fit_integral_parallel[i]->at(j));
            fit_amp1.push_back(fit_amp1_parallel[i]->at(j));
            fit_amp2.push_back(fit_amp2_parallel[i]->at(j));
            chi2_ndf.push_back(chi2_ndf_parallel[i]->at(j));
            max_chn_value.push_back(max_chn_value_parallel[i]->at(j));
            chn_sum.push_back(chn_sum_parallel[i]->at(j));
            max_error.push_back(max_error_parallel[i]->at(j));
            fit_sigma1.push_back(fit_sigma1_parallel[i]->at(j));
            fit_sigma2.push_back(fit_sigma2_parallel[i]->at(j));
            fit_x0_1.push_back(fit_x0_1_parallel[i]->at(j));
            fit_x0_2.push_back(fit_x0_2_parallel[i]->at(j));
            fit_y0_1.push_back(fit_y0_1_parallel[i]->at(j));
            fit_y0_2.push_back(fit_y0_2_parallel[i]->at(j));

            if (chi2_ndf_parallel[i]->at(j) < chi2_ndf_cut0){
                chn_sum_chi2_0.push_back(chn_sum_parallel[i]->at(j));
                fit_integral_chi2_0.push_back(fit_integral_parallel[i]->at(j));
                fit_sigma1_chi2_0.push_back(fit_sigma1_parallel[i]->at(j));
                fit_sigma2_chi2_0.push_back(fit_sigma2_parallel[i]->at(j));
                fit_distance_chi2_0.push_back(sqrt(pow(fit_x0_1_parallel[i]->at(j) - fit_x0_2_parallel[i]->at(j), 2) + pow(fit_y0_1_parallel[i]->at(j) - fit_y0_2_parallel[i]->at(j), 2)));
            }
            else {
                if (chi2_ndf_parallel[i]->at(j) < chi2_ndf_cut1){
                    chn_sum_chi2_1.push_back(chn_sum_parallel[i]->at(j));
                    fit_integral_chi2_1.push_back(fit_integral_parallel[i]->at(j));
                    fit_sigma1_chi2_1.push_back(fit_sigma1_parallel[i]->at(j));
                    fit_sigma2_chi2_1.push_back(fit_sigma2_parallel[i]->at(j));
                    fit_distance_chi2_1.push_back(sqrt(pow(fit_x0_1_parallel[i]->at(j) - fit_x0_2_parallel[i]->at(j), 2) + pow(fit_y0_1_parallel[i]->at(j) - fit_y0_2_parallel[i]->at(j), 2)));
                }
                else {
                    if (chi2_ndf_parallel[i]->at(j) < chi2_ndf_cut2){
                        chn_sum_chi2_2.push_back(chn_sum_parallel[i]->at(j));
                        fit_integral_chi2_2.push_back(fit_integral_parallel[i]->at(j));
                        fit_sigma1_chi2_2.push_back(fit_sigma1_parallel[i]->at(j));
                        fit_sigma2_chi2_2.push_back(fit_sigma2_parallel[i]->at(j));
                        fit_distance_chi2_2.push_back(sqrt(pow(fit_x0_1_parallel[i]->at(j) - fit_x0_2_parallel[i]->at(j), 2) + pow(fit_y0_1_parallel[i]->at(j) - fit_y0_2_parallel[i]->at(j), 2)));
                    }
                    else {
                        if (chi2_ndf_parallel[i]->at(j) < chi2_ndf_cut3){
                            chn_sum_chi2_3.push_back(chn_sum_parallel[i]->at(j));
                            fit_integral_chi2_3.push_back(fit_integral_parallel[i]->at(j));
                            fit_sigma1_chi2_3.push_back(fit_sigma1_parallel[i]->at(j));
                            fit_sigma2_chi2_3.push_back(fit_sigma2_parallel[i]->at(j));
                            fit_distance_chi2_3.push_back(sqrt(pow(fit_x0_1_parallel[i]->at(j) - fit_x0_2_parallel[i]->at(j), 2) + pow(fit_y0_1_parallel[i]->at(j) - fit_y0_2_parallel[i]->at(j), 2)));
                        }
                        else {
                            chn_sum_chi2_4.push_back(chn_sum_parallel[i]->at(j));
                            fit_integral_chi2_4.push_back(fit_integral_parallel[i]->at(j));
                            fit_sigma1_chi2_4.push_back(fit_sigma1_parallel[i]->at(j));
                            fit_sigma2_chi2_4.push_back(fit_sigma2_parallel[i]->at(j));
                            fit_distance_chi2_4.push_back(sqrt(pow(fit_x0_1_parallel[i]->at(j) - fit_x0_2_parallel[i]->at(j), 2) + pow(fit_y0_1_parallel[i]->at(j) - fit_y0_2_parallel[i]->at(j), 2)));
                        }
                    }
                }
            }
        }
    }

    LOG(INFO) << "Total number of events: " << fit_integral.size();
    LOG(INFO) << "Total number of events in chi2 cut 0: " << chn_sum_chi2_0.size();
    LOG(INFO) << "Total number of events in chi2 cut 1: " << chn_sum_chi2_1.size();
    LOG(INFO) << "Total number of events in chi2 cut 2: " << chn_sum_chi2_2.size();
    LOG(INFO) << "Total number of events in chi2 cut 3: " << chn_sum_chi2_3.size();
    LOG(INFO) << "Total number of events in chi2 cut 4: " << chn_sum_chi2_4.size();

    double rate_chi2_0 = double(chn_sum_chi2_0.size()) / double(fit_integral.size()) * 100.0;
    double rate_chi2_1 = double(chn_sum_chi2_1.size()) / double(fit_integral.size()) * 100.0;
    double rate_chi2_2 = double(chn_sum_chi2_2.size()) / double(fit_integral.size()) * 100.0;
    double rate_chi2_3 = double(chn_sum_chi2_3.size()) / double(fit_integral.size()) * 100.0;
    double rate_chi2_4 = double(chn_sum_chi2_4.size()) / double(fit_integral.size()) * 100.0;

    // * Data read finished
    gStyle->SetCanvasColor(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetStatColor(0);

    TCanvas *c = new TCanvas("c", "c", 1600, 1400);

    // * 2-D scatter plot
    auto scatter_chi2_0 = new TGraph();
    auto scatter_chi2_1 = new TGraph();
    auto scatter_chi2_2 = new TGraph();
    auto scatter_chi2_3 = new TGraph();
    auto scatter_chi2_4 = new TGraph();


    for (int i = 0; i < chn_sum_chi2_0.size(); i++){
        //scatter_chi2_0->SetPoint(i, fit_integral_chi2_0[i]/25, chn_sum_chi2_0[i]);
        scatter_chi2_0->SetPoint(i, fit_distance_chi2_0[i], fit_sigma2_chi2_0[i]);
    }

    for (int i = 0; i < chn_sum_chi2_1.size(); i++){
        //scatter_chi2_1->SetPoint(i, fit_integral_chi2_1[i]/25, chn_sum_chi2_1[i]);
        scatter_chi2_1->SetPoint(i, fit_distance_chi2_1[i], fit_sigma2_chi2_1[i]);
    }

    for (int i = 0; i < chn_sum_chi2_2.size(); i++){
        //scatter_chi2_2->SetPoint(i, fit_integral_chi2_2[i]/25, chn_sum_chi2_2[i]);
        scatter_chi2_2->SetPoint(i, fit_distance_chi2_2[i], fit_sigma2_chi2_2[i]);
    }

    for (int i = 0; i < chn_sum_chi2_3.size(); i++){
        //scatter_chi2_3->SetPoint(i, fit_integral_chi2_3[i]/25, chn_sum_chi2_3[i]);
        scatter_chi2_3->SetPoint(i, fit_distance_chi2_3[i], fit_sigma2_chi2_3[i]);
    }

    for (int i = 0; i < chn_sum_chi2_4.size(); i++){
        //scatter_chi2_4->SetPoint(i, fit_integral_chi2_4[i]/25, chn_sum_chi2_4[i]);
        scatter_chi2_4->SetPoint(i, fit_distance_chi2_4[i], fit_sigma2_chi2_4[i]);
    }

    LOG(DEBUG) << "Drawing...";

    // distance vs. integral
    scatter_chi2_4->SetTitle("#sigma_{2} vs. Distance");
    scatter_chi2_4->GetXaxis()->SetTitle("Distance");
    scatter_chi2_4->GetYaxis()->SetTitle("#sigma_{2}");


    scatter_chi2_0->GetXaxis()->SetRangeUser(0, 110000);
    scatter_chi2_0->GetYaxis()->SetRangeUser(0, 110000);
    scatter_chi2_4->GetXaxis()->SetRangeUser(0, 35);
    scatter_chi2_4->GetYaxis()->SetRangeUser(0, 35);

    scatter_chi2_0->SetMarkerStyle(20);
    scatter_chi2_1->SetMarkerStyle(20);
    scatter_chi2_2->SetMarkerStyle(20);
    scatter_chi2_3->SetMarkerStyle(20);
    scatter_chi2_4->SetMarkerStyle(20);

    scatter_chi2_0->SetMarkerSize(1.3);
    scatter_chi2_1->SetMarkerSize(1.3);
    scatter_chi2_2->SetMarkerSize(1.3);
    scatter_chi2_3->SetMarkerSize(1.3);
    scatter_chi2_4->SetMarkerSize(1.3);

    scatter_chi2_0->SetMarkerColorAlpha(kAzure,     0.7);
    scatter_chi2_1->SetMarkerColorAlpha(kCyan+1,    0.7);
    scatter_chi2_2->SetMarkerColorAlpha(kTeal+9,    0.7);
    scatter_chi2_3->SetMarkerColorAlpha(kOrange-3,  0.7);
    scatter_chi2_4->SetMarkerColorAlpha(kPink-2,    0.7);

    // add info on top right
    scatter_chi2_4->Draw("AP");
    scatter_chi2_3->Draw("P");
    scatter_chi2_2->Draw("P");
    scatter_chi2_1->Draw("P");
    scatter_chi2_0->Draw("P");

    LOG(DEBUG) << "Drawing finished";

    auto run_info = Form("Run %d", run_number);
    TLatex *tex = new TLatex(0.64, 0.87, run_info);
    tex->SetNDC();
    tex->SetTextFont(42);
    tex->SetTextSize(0.03);
    tex->SetLineWidth(2);
    tex->Draw();

    TLatex *tex2 = new TLatex(0.64, 0.82, "HG/LG mixed (1,500)");
    tex2->SetNDC();
    tex2->SetTextFont(42);
    tex2->SetTextSize(0.03);
    tex2->SetLineWidth(2);
    tex2->Draw();

    auto _n_dots_info = Form("N = %d", n_dots*n_parallel);

    TLatex *tex3 = new TLatex(0.64, 0.77, _n_dots_info);
    tex3->SetNDC();
    tex3->SetTextFont(42);
    tex3->SetTextSize(0.03);
    tex3->SetLineWidth(2);
    tex3->Draw();

    // Transparent background
    auto legend = new TLegend(0.1, 0.9, 0.35, 0.70);
    legend->SetBorderSize(2);

    auto str_chi2_0 = std::to_string(rate_chi2_0) + "% Best";
    auto str_chi2_1 = std::to_string(rate_chi2_1) + "% Better";
    auto str_chi2_2 = std::to_string(rate_chi2_2) + "% Middle";
    auto str_chi2_3 = std::to_string(rate_chi2_3) + "% Worse";
    auto str_chi2_4 = std::to_string(rate_chi2_4) + "% Worst";

    legend->AddEntry(scatter_chi2_0, str_chi2_0.c_str(), "p");
    legend->AddEntry(scatter_chi2_1, str_chi2_1.c_str(), "p");
    legend->AddEntry(scatter_chi2_2, str_chi2_2.c_str(), "p");
    legend->AddEntry(scatter_chi2_3, str_chi2_3.c_str(), "p");
    legend->AddEntry(scatter_chi2_4, str_chi2_4.c_str(), "p");

    legend->Draw();

    c->SetGrid(2, 2);
    // log x axis
    c->SetLogx();
    // c->SaveAs("../pics/integral_channelSum.png");
    LOG(INFO) << "Saving figure...";
    c->SaveAs("../pics/temp14.png");

    delete tex;
    delete tex2;
    delete c;
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