#include "SJ_includes.h"

// TODO: Fix cmakelists.txt with correct project name

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

    auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
    auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
    auto pedestalInfo   = SJUtil::read_pedestal_csv_file(file_pedestal_path);
    
    // * My analysis
    auto builder        = new CAEN_event_builder();
    builder->read_root_file2event_array(file_root_events_path);
    // * chn_HLG_data
    // * ---------- chn0
    // *       |        |--- std::vector<Short_t> chn0_HG_data
    // *       |        |--- std::vector<Short_t> chn0_LG_data
    // *       |              |--- event0:  chn0_LG_data[0]
    // *       |              |--- event1:  chn0_LG_data[1]
    // *       |              |--- ...
    // *       |              |--- eventN:  chn0_LG_data[N]
    // *       |--- chn1
    // *       |--- ...
    // *       |--- chnN
    std::vector<std::vector<std::vector<Short_t>>> chn_HLG_data;
    for (int chn = 0; chn < 256; chn++){
        std::vector<std::vector<Short_t>> chn_data;
        std::vector<Short_t> chn_data_h;
        std::vector<Short_t> chn_data_l;
        chn_data.push_back(chn_data_h);
        chn_data.push_back(chn_data_l);
        chn_HLG_data.push_back(chn_data);
    }

    auto events_info  = builder->get_event_array_ptr();
    auto events_valid = builder->get_event_valid_array_ptr();

    auto total_events = events_info->size();
    LOG(INFO) << "Total events: " << total_events;

    for (auto i=0; i<total_events; i++){
        if (events_valid->at(i) == false) continue;
        //* limit value to 0~4096
        auto max_hg_value = std::max_element(events_info->at(i).HG_charges.begin(), 
            events_info->at(i).HG_charges.end());
        auto max_lg_value = std::max_element(events_info->at(i).LG_charges.begin(),
            events_info->at(i).LG_charges.end());
        if (*max_hg_value > 4096) continue;
        if (*max_lg_value > 4096) continue;
        auto event = events_info->at(i);
        for (int chn = 0; chn < 256; chn++){
            auto chn_data_h = event.HG_charges[chn];
            chn_HLG_data[chn][0].push_back(chn_data_h);
            auto chn_data_l = event.LG_charges[chn];
            chn_HLG_data[chn][1].push_back(chn_data_l);
        }
    }

    delete builder;

    // * Draw them ALL!
    LOG(INFO) << "Drawing...";
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    TMultiGraph *mg = new TMultiGraph();

    std::vector<Double_t> slope_list;
    std::vector<Double_t> slope_err_list;
    std::vector<Double_t> offset_list;
    std::vector<Double_t> offset_err_list;

    // * Create color list for 256 channels
    std::vector<int> colors;
    for (int i = 0; i < 256; i++){
        colors.push_back(i+1);
    }
    for (int chn = 0; chn < 256; chn++){
        auto chn_data_h = chn_HLG_data[chn][0];
        auto chn_data_l = chn_HLG_data[chn][1];
        auto chn_data_h_size = chn_data_h.size();
        TVectorD x(chn_data_h_size);
        TVectorD y(chn_data_h_size);
        for (int i = 0; i < chn_data_h_size; i++){
            x[i] = chn_data_h[i];
            y[i] = chn_data_l[i];
        }
        TGraph *gr = new TGraph(x,y);
        //* Set graph points with one call
        gr->GetXaxis()->SetLimits(0, 4096);
        gr->GetYaxis()->SetLimits(0, 512);
        gr->SetMarkerStyle(20);
        gr->SetMarkerSize(0.5);
        gr->SetMarkerColor(TColor::GetColor(colors[chn]));
        // * DO LINEAR FITTING
        if (x.Max() >= 600)
            TF1 *f1 = new TF1("f1", "pol1", 400, 2048);
        else {
            TF1 *f1 = new TF1("f1", "pol1", 0, x.Max());
            LOG(INFO) << "Channel " << chn << " max value: " << x.Max();
        }
        gr->Fit("f1", "QR"); // * R: Use the range specified in the function range,
        // * Get fitting results
        auto fit_results = gr->GetFunction("f1");
        auto fit_results_slope = fit_results->GetParameter(1);
        auto fit_results_intercept = fit_results->GetParameter(0);
        slope_list.push_back(1/fit_results_slope);
        offset_list.push_back(fit_results_intercept);
        slope_err_list.push_back(fit_results->GetParError(1));
        offset_err_list.push_back(fit_results->GetParError(0));
        mg->Add(gr);
    }

    // * Draw
    mg->GetXaxis()->SetTitle("HG");
    mg->GetYaxis()->SetTitle("LG");
    mg->SetTitle("HG vs LG");
    // * X and Y axis
    mg->GetXaxis()->SetLimits(0, 4096);
    mg->GetYaxis()->SetLimits(0, 512);
    // * Draw the first graph
    mg->Draw("AP");
    c1->Update();
    c1->SaveAs("../pics/HG_vs_LG.png");
    // * Save slope and offset
    TFile *slope_offset_file = new TFile("../cachedFiles/slope_offset.root", "RECREATE");
    TTree *slope_offset_tree = new TTree("slope_offset_tree", "slope_offset_tree");
    Double_t slope;
    Double_t slope_err;
    Double_t offset;
    Double_t offset_err;
    slope_offset_tree->Branch("slope", &slope, "slope/D");
    slope_offset_tree->Branch("offset", &offset, "offset/D");
    slope_offset_tree->Branch("slope_err", &slope_err, "slope_err/D");
    slope_offset_tree->Branch("offset_err", &offset_err, "offset_err/D");

    for (int i = 0; i < 256; i++){
        slope = slope_list[i];
        offset = offset_list[i];
        slope_err = slope_err_list[i];
        offset_err = offset_err_list[i];
        slope_offset_tree->Fill();
    }
    // slope_offset_tree->Write();

    // slope_offset_file->Close();

    // * draw slope distribution
    TFile* mapFile = new TFile("../cachedFiles/slope_map.root", "RECREATE");
    TCanvas *c2 = new TCanvas("c2", "c2", 1200, 900);
    std::vector<Short_t> _slope_short_array;
    for (auto _double_value: slope_list)
        _slope_short_array.push_back(Short_t(_double_value*1000));
    auto _mapped_slope = SJUtil::map1d_to_2d(_slope_short_array, mapping_coords);
    
    auto _scatter_3d = SJPlot::scatter_3d_raw(_mapped_slope, "slopedistri", "slopedistri");
    c2->cd();
    _scatter_3d->Draw("pcol");
    c2->Update();
    c2->Write();
    mapFile->Close();

    LOG(INFO) << "Program finished";
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