#include "SJ_includes.h"
#include <TPavesText.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
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

    auto file_unbinned_file1 = "../cachedFiles/Run_2806_fit_result_1.root";
    auto file_unbinned_file2 = "../cachedFiles/Run_2806_fit_result_2.root";
    auto file_unbinned_file3 = "../cachedFiles/Run_2806_fit_result_3.root";
    auto file_unbinned_file4 = "../cachedFiles/Run_2806_fit_result_4.root";
    auto file_unbinned_file5 = "../cachedFiles/Run_2806_fit_result_5.root";
    auto file_unbinned_file6 = "../cachedFiles/Run_2806_fit_result_6.root";
    auto file_unbinned_file7 = "../cachedFiles/Run_2806_fit_result_7.root";
    // auto file_unbinned_file8 = "../cachedFiles/Run_2806_fit_result_8.root";

    auto file_chi2_file1 = "../cachedFiles/Run_2806_chi2_ndf_1.root";
    auto file_chi2_file2 = "../cachedFiles/Run_2806_chi2_ndf_2.root";
    auto file_chi2_file3 = "../cachedFiles/Run_2806_chi2_ndf_3.root";
    auto file_chi2_file4 = "../cachedFiles/Run_2806_chi2_ndf_4.root";
    auto file_chi2_file5 = "../cachedFiles/Run_2806_chi2_ndf_5.root";
    auto file_chi2_file6 = "../cachedFiles/Run_2806_chi2_ndf_6.root";
    auto file_chi2_file7 = "../cachedFiles/Run_2806_chi2_ndf_7.root";
    // auto file_chi2_file8 = "../cachedFiles/Run_2806_chi2_ndf_8.root";

    TFile *f1 = new TFile(file_unbinned_file1);
    TFile *f2 = new TFile(file_unbinned_file2);
    TFile *f3 = new TFile(file_unbinned_file3);
    TFile *f4 = new TFile(file_unbinned_file4);
    TFile *f5 = new TFile(file_unbinned_file5);
    TFile *f6 = new TFile(file_unbinned_file6);
    TFile *f7 = new TFile(file_unbinned_file7);
    //TFile *f8 = new TFile(file_unbinned_file8);

    TFile *f_chi2_1 = new TFile(file_chi2_file1);
    TFile *f_chi2_2 = new TFile(file_chi2_file2);
    TFile *f_chi2_3 = new TFile(file_chi2_file3);
    TFile *f_chi2_4 = new TFile(file_chi2_file4);
    TFile *f_chi2_5 = new TFile(file_chi2_file5);
    TFile *f_chi2_6 = new TFile(file_chi2_file6);
    TFile *f_chi2_7 = new TFile(file_chi2_file7);
    //TFile *f_chi2_8 = new TFile(file_chi2_file8);

    TTree *t1 = (TTree*)f1->Get("arrayTree");
    TTree *t2 = (TTree*)f2->Get("arrayTree");
    TTree *t3 = (TTree*)f3->Get("arrayTree");
    TTree *t4 = (TTree*)f4->Get("arrayTree");
    TTree *t5 = (TTree*)f5->Get("arrayTree");
    TTree *t6 = (TTree*)f6->Get("arrayTree");
    TTree *t7 = (TTree*)f7->Get("arrayTree");
    //TTree *t8 = (TTree*)f8->Get("arrayTree");

    TTree *t_chi2_1 = (TTree*)f_chi2_1->Get("arrayTree");
    TTree *t_chi2_2 = (TTree*)f_chi2_2->Get("arrayTree");
    TTree *t_chi2_3 = (TTree*)f_chi2_3->Get("arrayTree");
    TTree *t_chi2_4 = (TTree*)f_chi2_4->Get("arrayTree");
    TTree *t_chi2_5 = (TTree*)f_chi2_5->Get("arrayTree");
    TTree *t_chi2_6 = (TTree*)f_chi2_6->Get("arrayTree");
    TTree *t_chi2_7 = (TTree*)f_chi2_7->Get("arrayTree");
    //TTree *t_chi2_8 = (TTree*)f_chi2_8->Get("arrayTree");

    std::vector<double> *v1 = nullptr;
    std::vector<double> *v2 = nullptr;
    std::vector<double> *v3 = nullptr;
    std::vector<double> *v4 = nullptr;
    std::vector<double> *v5 = nullptr;
    std::vector<double> *v6 = nullptr;
    std::vector<double> *v7 = nullptr;
    //std::vector<double> *v8 = nullptr;

    std::vector<double> *v_chi2_1 = nullptr;
    std::vector<double> *v_chi2_2 = nullptr;
    std::vector<double> *v_chi2_3 = nullptr;
    std::vector<double> *v_chi2_4 = nullptr;
    std::vector<double> *v_chi2_5 = nullptr;
    std::vector<double> *v_chi2_6 = nullptr;
    std::vector<double> *v_chi2_7 = nullptr;
    //std::vector<double> *v_chi2_8 = nullptr;

    t1->SetBranchAddress("array_data", &v1);
    t2->SetBranchAddress("array_data", &v2);
    t3->SetBranchAddress("array_data", &v3);
    t4->SetBranchAddress("array_data", &v4);
    t5->SetBranchAddress("array_data", &v5);
    t6->SetBranchAddress("array_data", &v6);
    t7->SetBranchAddress("array_data", &v7);
    //t8->SetBranchAddress("array_data", &v8);

    t_chi2_1->SetBranchAddress("array_data", &v_chi2_1);
    t_chi2_2->SetBranchAddress("array_data", &v_chi2_2);
    t_chi2_3->SetBranchAddress("array_data", &v_chi2_3);
    t_chi2_4->SetBranchAddress("array_data", &v_chi2_4);
    t_chi2_5->SetBranchAddress("array_data", &v_chi2_5);
    t_chi2_6->SetBranchAddress("array_data", &v_chi2_6);
    t_chi2_7->SetBranchAddress("array_data", &v_chi2_7);
    //t_chi2_8->SetBranchAddress("array_data", &v_chi2_8);

    int nentries1 = t1->GetEntry(0);
    int nentries2 = t2->GetEntry(0);
    int nentries3 = t3->GetEntry(0);
    int nentries4 = t4->GetEntry(0);
    int nentries5 = t5->GetEntry(0);
    int nentries6 = t6->GetEntry(0);
    int nentries7 = t7->GetEntry(0);
    //int nentries8 = t8->GetEntry(0);

    int nentries_chi2_1 = t_chi2_1->GetEntry(0);
    int nentries_chi2_2 = t_chi2_2->GetEntry(0);
    int nentries_chi2_3 = t_chi2_3->GetEntry(0);
    int nentries_chi2_4 = t_chi2_4->GetEntry(0);
    int nentries_chi2_5 = t_chi2_5->GetEntry(0);
    int nentries_chi2_6 = t_chi2_6->GetEntry(0);
    int nentries_chi2_7 = t_chi2_7->GetEntry(0);
    //int nentries_chi2_8 = t_chi2_8->GetEntry(0);

    auto xMax_integrated = 100000;
    auto xMax_amp = 8000;
    auto chi2_threshold = 15000;

    auto total_events = 0;
    auto total_events_passed = 0;

    std::vector<double> res_assembly;

    for (int i = 0; i < v1->size(); i++){
        total_events ++;
        if (v_chi2_1->at(i) < chi2_threshold){
            res_assembly.push_back(v1->at(i));
            total_events_passed ++;
        }
    }
    for (int i = 0; i < v2->size(); i++){
        total_events ++;
        if (v_chi2_2->at(i) < chi2_threshold){
            res_assembly.push_back(v2->at(i));
            total_events_passed ++;
        }
    }
    for (int i = 0; i < v3->size(); i++){
        total_events ++;
        if (v_chi2_3->at(i) < chi2_threshold){
            res_assembly.push_back(v3->at(i));
            total_events_passed ++;
        }
    }
    for (int i = 0; i < v4->size(); i++){
        total_events ++;
        if (v_chi2_4->at(i) < chi2_threshold){
            res_assembly.push_back(v4->at(i));
            total_events_passed ++;
        }
    }
    for (int i = 0; i < v5->size(); i++){
        total_events ++;
        if (v_chi2_5->at(i) < chi2_threshold){
            res_assembly.push_back(v5->at(i));
            total_events_passed ++;
        }
    }
    for (int i = 0; i < v6->size(); i++){
        total_events ++;
        if (v_chi2_6->at(i) < chi2_threshold){
            res_assembly.push_back(v6->at(i));
            total_events_passed ++;
        }
    }
    for (int i = 0; i < v7->size(); i++){
        total_events ++;
        if (v_chi2_7->at(i) < chi2_threshold){
            res_assembly.push_back(v7->at(i));
            total_events_passed ++;
        }
    }
    // for (int i = 0; i < v8->size(); i++){
    //     total_events ++;
    //     if (v_chi2_8->at(i) < chi2_threshold){
    //         res_assembly.push_back(v8->at(i));
    //         total_events_passed ++;
    //     }
    // }
    
    TH1D *h = new TH1D("h", "h", 200, 0, xMax_integrated);
    for (int i = 0; i < res_assembly.size(); i++){
        h->Fill(res_assembly.at(i)/25);
    }

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    h->SetTitle("Integrated charge distribution");
    h->GetXaxis()->SetTitle("Integrated charge [ADC]");
    h->GetXaxis()->SetTitle("Amplitude [ADC]");
    h->GetYaxis()->SetTitle("Normalized count");
    h->Scale(1.0/h->Integral());
    h->GetXaxis()->SetRangeUser(0, xMax_integrated);
    h->SetLineWidth(2);
    h->SetLineStyle(2);
    h->SetStats(1);
    // fit with a 1-d gaussian
    auto gaus = new TF1("gaus", "gaus", 27000, 50000);
    h->Fit(gaus, "R");
    gaus->SetLineColor(kRed);
    gaus->SetLineWidth(2);
    gaus->SetLineStyle(2);

    // get results
    auto mean = gaus->GetParameter(1);
    auto sigma = gaus->GetParameter(2);
    auto mean_err = gaus->GetParError(1);
    auto sigma_err = gaus->GetParError(2);
    // h->Draw("p");
    h->Draw("p");
    gaus->Draw("same");

    // show results on plot
    auto text = new TLatex();
    text->SetNDC();
    text->SetTextFont(43);
    text->SetTextSize(20);
    text->SetTextColor(kRed);
    text->DrawLatex(0.6, 0.7, Form("#mu = %.2f #pm %.2f", mean, mean_err));
    text->DrawLatex(0.6, 0.65, Form("#sigma = %.2f #pm %.2f", sigma, sigma_err));
    // Transparent background
    c->SetGrid();
    c->SaveAs("../pics/unbinned_fit_res.png");

    LOG(INFO) << "Total events: " << total_events;
    LOG(INFO) << "Total events passed: " << total_events_passed;
    LOG(INFO) << "Efficiency: " << (double)total_events_passed/total_events;

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