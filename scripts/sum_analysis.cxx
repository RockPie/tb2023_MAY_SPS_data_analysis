#include "SJ_includes.h"

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations
    int run_number = 2819;

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

    // * Read event info (we do not need mappings)
    auto builder        = new CAEN_event_builder();
    builder->read_root_file2event_array(file_root_events_path);
    auto HGain_Sum = builder->get_HG_charge_sum_array();
    auto LGain_Sum = builder->get_LG_charge_sum_array();
    for (int i = 0; i < LGain_Sum.size(); i++)
        LGain_Sum[i] *= 9.7;

    // * Plot the sum of charges
    auto n_bins = 200;
    auto x_min  = 0;
    auto x_max  = 100000;
    auto canvas = new TCanvas("canvas", "canvas", 800, 600);
    auto th1d_HGain_Sum = new TH1D("th1d_HGain_Sum", "th1d_HGain_Sum", n_bins, x_min, x_max);
    auto th1d_LGain_Sum = new TH1D("th1d_LGain_Sum", "th1d_LGain_Sum", n_bins, x_min, x_max);
    for (int i = 0; i < HGain_Sum.size(); i++)
        th1d_HGain_Sum->Fill(HGain_Sum[i]);
    for (int i = 0; i < LGain_Sum.size(); i++)
        th1d_LGain_Sum->Fill(LGain_Sum[i]);

    // * Normalize
    th1d_HGain_Sum->Scale(1.0/th1d_HGain_Sum->Integral());
    th1d_LGain_Sum->Scale(1.0/th1d_LGain_Sum->Integral());

    // * Fit with 1-D Gaussian
    auto fit_func = new TF1("fit_func", "gaus", 34000, 52000);
    fit_func->SetLineColor(kGreen);
    fit_func->SetLineWidth(2);
    // set dashed line
    fit_func->SetLineStyle(2);

    auto fitResultHG = th1d_HGain_Sum->Fit(fit_func, "R S");

    auto fit_mean_HG = fit_func->GetParameter(1);
    auto fit_sigma_HG = fit_func->GetParameter(2);


    auto fitResultLG = th1d_LGain_Sum->Fit(fit_func, "R S");

    auto fit_mean_LG = fit_func->GetParameter(1);
    auto fit_sigma_LG = fit_func->GetParameter(2);


    // * Set style
    th1d_HGain_Sum->SetLineColor(kRed);
    th1d_LGain_Sum->SetLineColor(kBlue);

    th1d_HGain_Sum->SetLineWidth(2);
    th1d_LGain_Sum->SetLineWidth(2);

    th1d_HGain_Sum->SetStats(0);
    th1d_LGain_Sum->SetStats(0);

    th1d_HGain_Sum->SetTitle("ADC Sum over All Channels (run 2819)");

    th1d_HGain_Sum->GetXaxis()->SetTitle("Charge (ADC)");
    th1d_HGain_Sum->GetYaxis()->SetTitle("Normalized counts");

    th1d_HGain_Sum->Draw("p");
    th1d_LGain_Sum->Draw("p same");

    // * Draw legend
    auto legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(th1d_HGain_Sum, "High Gain", "l");
    legend->AddEntry(th1d_LGain_Sum, "Low Gain x 9.7", "l");
    legend->AddEntry(fit_func, "Gaussian Fit", "l");
    legend->Draw();

    // * Print fit results to the canvas
    auto text = new TLatex();
    text->SetNDC();
    text->SetTextSize(0.03);
    text->SetTextColor(kRed);
    text->DrawLatex(0.65, 0.6, Form("HG Fit Mean: %.2f", fit_mean_HG));
    text->DrawLatex(0.65, 0.55, Form("HG Fit Sigma: %.2f", fit_sigma_HG));

    text->SetTextColor(kBlue);
    text->DrawLatex(0.65, 0.5, Form("LG Fit Mean: %.2f", fit_mean_LG));
    text->DrawLatex(0.65, 0.45, Form("LG Fit Sigma: %.2f", fit_sigma_LG));

    // * Update canvas

    
    // log scale
    // canvas->SetLogy();
    canvas->Update();
    // * add grid
    canvas->SetGrid();
    canvas->SaveAs("../cachedFiles/sum_analysis_run2819.png");

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