#include "SJ_includes.h"

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
    SJUtil::create_filename("../cachedFiles", 
        DEFAULT_PREFIX_ROOT, run_number, "_mixed_fit_res", DEFAULT_EXTENSION_ROOT);
    auto file_relative_gain_path    = "../dataFiles/slope_offset.root";

    auto file_roofit_res_file1_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_1.root";
    auto file_roofit_res_file2_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_2.root";
    auto file_roofit_res_file3_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_3.root";
    auto file_roofit_res_file4_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_4.root";
    auto file_roofit_res_file5_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_5.root";
    auto file_roofit_res_file6_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_6.root";
    auto file_roofit_res_file7_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roofit_res_7.root";

    auto file_roofit_chi2_file1_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_1.root";
    auto file_roofit_chi2_file2_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_2.root";
    auto file_roofit_chi2_file3_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_3.root";
    auto file_roofit_chi2_file4_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_4.root";
    auto file_roofit_chi2_file5_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_5.root";
    auto file_roofit_chi2_file6_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_6.root";
    auto file_roofit_chi2_file7_path = "../cachedFiles/Run_2806_Roofit_Res_1106/Run_2806_Roochi2_res_7.root";

    // * Load data
    TFile *file_roofit_res_1 = new TFile(file_roofit_res_file1_path);
    TFile *file_roofit_res_2 = new TFile(file_roofit_res_file2_path);
    TFile *file_roofit_res_3 = new TFile(file_roofit_res_file3_path);
    TFile *file_roofit_res_4 = new TFile(file_roofit_res_file4_path);
    TFile *file_roofit_res_5 = new TFile(file_roofit_res_file5_path);
    TFile *file_roofit_res_6 = new TFile(file_roofit_res_file6_path);
    TFile *file_roofit_res_7 = new TFile(file_roofit_res_file7_path);

    TTree *tree_roofit_res_1 = (TTree*)file_roofit_res_1->Get("arrayTree");
    TTree *tree_roofit_res_2 = (TTree*)file_roofit_res_2->Get("arrayTree");
    TTree *tree_roofit_res_3 = (TTree*)file_roofit_res_3->Get("arrayTree");
    TTree *tree_roofit_res_4 = (TTree*)file_roofit_res_4->Get("arrayTree");
    TTree *tree_roofit_res_5 = (TTree*)file_roofit_res_5->Get("arrayTree");
    TTree *tree_roofit_res_6 = (TTree*)file_roofit_res_6->Get("arrayTree");
    TTree *tree_roofit_res_7 = (TTree*)file_roofit_res_7->Get("arrayTree");

    // * Load data from chi2
    TFile *file_roofit_chi2_1 = new TFile(file_roofit_chi2_file1_path);
    TFile *file_roofit_chi2_2 = new TFile(file_roofit_chi2_file2_path);
    TFile *file_roofit_chi2_3 = new TFile(file_roofit_chi2_file3_path);
    TFile *file_roofit_chi2_4 = new TFile(file_roofit_chi2_file4_path);
    TFile *file_roofit_chi2_5 = new TFile(file_roofit_chi2_file5_path);
    TFile *file_roofit_chi2_6 = new TFile(file_roofit_chi2_file6_path);
    TFile *file_roofit_chi2_7 = new TFile(file_roofit_chi2_file7_path);

    TTree *tree_roofit_chi2_1 = (TTree*)file_roofit_chi2_1->Get("arrayTree");
    TTree *tree_roofit_chi2_2 = (TTree*)file_roofit_chi2_2->Get("arrayTree");
    TTree *tree_roofit_chi2_3 = (TTree*)file_roofit_chi2_3->Get("arrayTree");
    TTree *tree_roofit_chi2_4 = (TTree*)file_roofit_chi2_4->Get("arrayTree");
    TTree *tree_roofit_chi2_5 = (TTree*)file_roofit_chi2_5->Get("arrayTree");
    TTree *tree_roofit_chi2_6 = (TTree*)file_roofit_chi2_6->Get("arrayTree");
    TTree *tree_roofit_chi2_7 = (TTree*)file_roofit_chi2_7->Get("arrayTree");

    // * Set branch address
    std::vector<double> *res_1 = nullptr;
    std::vector<double> *res_2 = nullptr;
    std::vector<double> *res_3 = nullptr;
    std::vector<double> *res_4 = nullptr;
    std::vector<double> *res_5 = nullptr;
    std::vector<double> *res_6 = nullptr;
    std::vector<double> *res_7 = nullptr;

    tree_roofit_res_1->SetBranchAddress("array_data", &res_1);
    tree_roofit_res_2->SetBranchAddress("array_data", &res_2);
    tree_roofit_res_3->SetBranchAddress("array_data", &res_3);
    tree_roofit_res_4->SetBranchAddress("array_data", &res_4);
    tree_roofit_res_5->SetBranchAddress("array_data", &res_5);
    tree_roofit_res_6->SetBranchAddress("array_data", &res_6);
    tree_roofit_res_7->SetBranchAddress("array_data", &res_7);

    // * Set branch address for chi2
    std::vector<double> *chi2_1 = nullptr;
    std::vector<double> *chi2_2 = nullptr;  
    std::vector<double> *chi2_3 = nullptr;
    std::vector<double> *chi2_4 = nullptr;
    std::vector<double> *chi2_5 = nullptr;
    std::vector<double> *chi2_6 = nullptr;
    std::vector<double> *chi2_7 = nullptr;

    tree_roofit_chi2_1->SetBranchAddress("array_data", &chi2_1);
    tree_roofit_chi2_2->SetBranchAddress("array_data", &chi2_2);
    tree_roofit_chi2_3->SetBranchAddress("array_data", &chi2_3);
    tree_roofit_chi2_4->SetBranchAddress("array_data", &chi2_4);
    tree_roofit_chi2_5->SetBranchAddress("array_data", &chi2_5);
    tree_roofit_chi2_6->SetBranchAddress("array_data", &chi2_6);
    tree_roofit_chi2_7->SetBranchAddress("array_data", &chi2_7);

    // * Get entries
    int entries_1 = tree_roofit_res_1->GetEntry(0);
    int entries_2 = tree_roofit_res_2->GetEntry(0);
    int entries_3 = tree_roofit_res_3->GetEntry(0);
    int entries_4 = tree_roofit_res_4->GetEntry(0);
    int entries_5 = tree_roofit_res_5->GetEntry(0);
    int entries_6 = tree_roofit_res_6->GetEntry(0);
    int entries_7 = tree_roofit_res_7->GetEntry(0);

    int entries_chi2_1 = tree_roofit_chi2_1->GetEntry(0);
    int entries_chi2_2 = tree_roofit_chi2_2->GetEntry(0);
    int entries_chi2_3 = tree_roofit_chi2_3->GetEntry(0);
    int entries_chi2_4 = tree_roofit_chi2_4->GetEntry(0);
    int entries_chi2_5 = tree_roofit_chi2_5->GetEntry(0);
    int entries_chi2_6 = tree_roofit_chi2_6->GetEntry(0);
    int entries_chi2_7 = tree_roofit_chi2_7->GetEntry(0);


    double chi2_threshold = 30000;
    double chi2_threshold_pass_cnt = 0;
    double chi2_threshold_fail_cnt = 0;

    std::vector<double> all_res;
    for (int i = 0; i < res_1->size(); i++){
        if (chi2_1->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_1->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    for (int i = 0; i < res_2->size(); i++){
        if (chi2_2->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_2->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    for (int i = 0; i < res_3->size(); i++){
        if (chi2_3->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_3->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    for (int i = 0; i < res_4->size(); i++){
        if (chi2_4->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_4->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    for (int i = 0; i < res_5->size(); i++){
        if (chi2_5->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_5->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    for (int i = 0; i < res_6->size(); i++){
        if (chi2_6->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_6->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    for (int i = 0; i < res_7->size(); i++){
        if (chi2_7->at(i) < chi2_threshold){
            chi2_threshold_pass_cnt++;
            all_res.push_back(res_7->at(i));
        }
        else{
            chi2_threshold_fail_cnt++;
        }
    }

    // * Create histogram
    TH1D *hist_res = new TH1D("hist_res", "hist_res", 500, 0, 4000);
    for (auto _res : all_res){
        hist_res->Fill(_res);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    hist_res->Draw();
    c1->SaveAs("../pics/roofit_res.png");

    file_roofit_res_1 -> Close();
    file_roofit_res_2 -> Close();
    file_roofit_res_3 -> Close();
    file_roofit_res_4 -> Close();
    file_roofit_res_5 -> Close();
    file_roofit_res_6 -> Close();
    file_roofit_res_7 -> Close();

    LOG(INFO) << "chi2_threshold_pass_cnt: " << chi2_threshold_pass_cnt;
    LOG(INFO) << "chi2_threshold_fail_cnt: " << chi2_threshold_fail_cnt;


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