#include "SJ_includes.h"
#include <TPavesText.h>
#include <TGraphErrors.h>

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();   // * set easylogging++ configurations

    const int    n_parallel             = 8;
    const bool   is_chi2_filtering      = false;
    const double chi2_ndf_threshold     = 15000;
    auto file_energyfittingconfig_path  = "../dataFiles/EnergyConfig.csv";
    auto file_rado_MC_path              = "../dataFiles/Rado_Results/MC.txt";
    auto file_rado_May_Mixed_path       = "../dataFiles/Rado_Results/Data_May_Reso_HG_Mixed_Reso.txt";
    auto file_rado_May_LG_path          = "../dataFiles/Rado_Results/Data_May_lg_Reso.txt";

    // * Read text file data
    std::vector<double> rado_MC_energy, rado_MC_reso, rado_MC_reso_err;
    std::vector<double> rado_May_Mixed_energy, rado_May_Mixed_reso, rado_May_Mixed_reso_err;
    std::vector<double> rado_May_LG_energy, rado_May_LG_reso, rado_May_LG_reso_err;

    std::ifstream file_rado_MC(file_rado_MC_path);
    std::ifstream file_rado_May_Mixed(file_rado_May_Mixed_path);
    std::ifstream file_rado_May_LG(file_rado_May_LG_path);

    double _rado_MC_energy, _rado_MC_reso, _rado_MC_reso_err;
    double _rado_May_Mixed_energy, _rado_May_Mixed_reso, _rado_May_Mixed_reso_err;
    double _rado_May_LG_energy, _rado_May_LG_reso, _rado_May_LG_reso_err;

    // data is delimited by comma, and each line ends with a newline
    auto delimiter = ',';
    auto line_ending = '\n';

    std::string line;
    std::getline(file_rado_MC, line, line_ending);
    // get double values from string
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        _rado_MC_energy = std::stod(token);
        rado_MC_energy.push_back(_rado_MC_energy);
    }
    std::getline(file_rado_MC, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_MC_reso = std::stod(token);
        rado_MC_reso.push_back(_rado_MC_reso);
    }
    std::getline(file_rado_MC, line, line_ending);
    std::getline(file_rado_MC, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_MC_reso_err = std::stod(token);
        rado_MC_reso_err.push_back(_rado_MC_reso_err);
    }

    std::getline(file_rado_May_Mixed, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_May_Mixed_energy = std::stod(token);
        rado_May_Mixed_energy.push_back(_rado_May_Mixed_energy);
    }

    std::getline(file_rado_May_Mixed, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_May_Mixed_reso = std::stod(token);
        rado_May_Mixed_reso.push_back(_rado_May_Mixed_reso);
    }

    std::getline(file_rado_May_Mixed, line, line_ending);
    std::getline(file_rado_May_Mixed, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_May_Mixed_reso_err = std::stod(token);
        rado_May_Mixed_reso_err.push_back(_rado_May_Mixed_reso_err);
    }

    std::getline(file_rado_May_LG, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_May_LG_energy = std::stod(token);
        rado_May_LG_energy.push_back(_rado_May_LG_energy);
    }

    std::getline(file_rado_May_LG, line, line_ending);
    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_May_LG_reso = std::stod(token);
        rado_May_LG_reso.push_back(_rado_May_LG_reso);
    }

    std::getline(file_rado_May_LG, line, line_ending);
    std::getline(file_rado_May_LG, line, line_ending);

    ss = std::stringstream(line);
    while (std::getline(ss, token, delimiter)) {
        _rado_May_LG_reso_err = std::stod(token);
        rado_May_LG_reso_err.push_back(_rado_May_LG_reso_err);
    }

    

    std::vector<int> run_number_array = {2798, 2799, 2801, 2802, 2803, 2804, 2805, 2806};
    std::vector<int> run_energy_array = {300,  350,  200,  150,  100,  80,   60,   250}; // * in GeV

    if (run_number_array.size() != run_energy_array.size()){
        LOG(ERROR) << "run_number_array.size() != run_energy_array.size()";
        return 1;
    }
    std::vector<Int_t> ERC_run_number, ERC_fit_xmin, ERC_fit_xmax, ERC_bin_number, ERC_hist_xmax;
    io::CSVReader <5> in(file_energyfittingconfig_path);
    in.read_header(io::ignore_extra_column, "Run_Number", "Fit_xmin", "Fit_xmax", "Bin_Number", "Hist_xmax");
    Int_t _run_number, _fit_xmin, _fit_xmax, _bin_number, _hist_xmax;
    while (in.read_row(_run_number, _fit_xmin, _fit_xmax, _bin_number, _hist_xmax)) {
        ERC_run_number.push_back(_run_number);
        ERC_fit_xmin.push_back(_fit_xmin);
        ERC_fit_xmax.push_back(_fit_xmax);
        ERC_bin_number.push_back(_bin_number);
        ERC_hist_xmax.push_back(_hist_xmax);
    }

    // * sort run_number_array and run_energy_array according to run_energy_array
    for (int i = 0; i < run_energy_array.size(); i++){
        for (int j = i + 1; j < run_energy_array.size(); j++){
            if (run_energy_array[i] > run_energy_array[j]){
                std::swap(run_energy_array[i], run_energy_array[j]);
                std::swap(run_number_array[i], run_number_array[j]);
            }
        }
    }

    std::vector<Double_t> mean_vec;
    std::vector<Double_t> mean_err_vec;
    std::vector<Double_t> sigma_vec;
    std::vector<Double_t> sigma_err_vec;
    std::vector<Double_t> resolution_vec;
    std::vector<Double_t> resolution_err_vec;

    for (int i = 0; i < run_number_array.size(); i++){
        LOG(INFO) << "Fitting run " << run_number_array[i] << " (" << run_energy_array[i] << " GeV)";
        int _run_number  = run_number_array[i];
        int _n_bins      = 200;
        int _hist_xmax   = 100000;
        double _fit_xmin = 5000;
        double _fit_xmax = 10000;

        auto run_index = std::find(ERC_run_number.begin(), ERC_run_number.end(), _run_number) - ERC_run_number.begin();

        _n_bins = ERC_bin_number[run_index];
        _hist_xmax = ERC_hist_xmax[run_index];
        _fit_xmin = ERC_fit_xmin[run_index];
        _fit_xmax = ERC_fit_xmax[run_index];

        std::vector<std::string> file_unbinned_file_name_array;
        for (int i = 0; i < n_parallel; i++){
            auto file_unbinned_file_name = "../cachedFiles/FullRuns_220623/Run_" + std::to_string(_run_number) + "_fit_result_" + std::to_string(i+1) + ".root";
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

        std::vector<std::vector<double>*> fit_integral_parallel, fit_amp1_parallel, fit_amp2_parallel, chi2_ndf_parallel, max_chn_value_parallel, chn_sum_parallel;

        for (int i = 0; i < n_parallel; i++){
            std::vector<double> *fit_integral   = nullptr;
            std::vector<double> *fit_amp1       = nullptr;
            std::vector<double> *fit_amp2       = nullptr;
            std::vector<double> *chi2_ndf       = nullptr;
            std::vector<double> *max_chn_value  = nullptr;
            std::vector<double> *chn_sum        = nullptr;

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

        std::vector<double> uni_fit_integral, uni_fit_amp1, uni_fit_amp2, uni_chi2_ndf, uni_max_chn_value, uni_chn_sum;

        auto _total_event_cnt = 0;
        auto _event_pass_chi2_cnt = 0;

        for (int i = 0; i < n_parallel; i++){
            for (int j = 0; j < fit_integral_parallel[i]->size(); j++){
                _total_event_cnt++;
                if (chi2_ndf_parallel[i]->at(j) > chi2_ndf_threshold)
                    if (is_chi2_filtering)
                        continue;
                _event_pass_chi2_cnt++;
                uni_fit_integral.push_back(fit_integral_parallel[i]->at(j));
                uni_fit_amp1.push_back(fit_amp1_parallel[i]->at(j));
                uni_fit_amp2.push_back(fit_amp2_parallel[i]->at(j));
                uni_chi2_ndf.push_back(chi2_ndf_parallel[i]->at(j));
                uni_max_chn_value.push_back(max_chn_value_parallel[i]->at(j));
                uni_chn_sum.push_back(chn_sum_parallel[i]->at(j));
            }
        }

        for (int i = 0; i < n_parallel; i++){
            file_unbinned_file_array[i]->Close();
        }

        fit_integral_parallel.clear();
        fit_amp1_parallel.clear();  
        fit_amp2_parallel.clear();
        chi2_ndf_parallel.clear();
        max_chn_value_parallel.clear();
        chn_sum_parallel.clear();

        LOG(INFO) << "Total number of events in run " << _run_number << " is " << uni_fit_integral.size();

        TH1D *h = new TH1D("h", "h", _n_bins, 0, _hist_xmax);
        for (int i = 0; i < uni_fit_integral.size(); i++){
            h->Fill(uni_fit_integral.at(i)/25);
        }

        h->SetTitle("Integrated charge distribution");
        // h->SetTitle("Amplitude distribution");
        h->GetXaxis()->SetTitle("Integrated charge [ADC]");
        // h->GetXaxis()->SetTitle("Amplitude [ADC]");
        h->GetYaxis()->SetTitle("Normalized count");
        h->Scale(1.0/h->Integral());
        h->GetXaxis()->SetRangeUser(0, _hist_xmax);
        h->SetLineWidth(3);
        h->SetLineStyle(1);
        h->SetStats(0);

        // fit with a 1-d gaussian
        auto gaus = new TF1("gaus", "gaus", _fit_xmin, _fit_xmax);
        h->Fit(gaus, "ER");
        gaus->SetLineColor(kRed);
        gaus->SetLineWidth(3);
        gaus->SetLineStyle(2);

        auto chi2_ndf_str = Form("#chi^{2}/NDF = %.2f", gaus->GetChisquare()/gaus->GetNDF());

        // get results
        auto mean = gaus->GetParameter(1);
        auto sigma = gaus->GetParameter(2);
        auto mean_err = gaus->GetParError(1);
        auto sigma_err = gaus->GetParError(2);
        auto resolution = sigma * 100.0 /mean;
        // auto resolution_err = 100 * std::abs(sigma / mean) * sqrt(pow(sigma_err / sigma, 2) + pow(mean_err / mean, 2));
        auto resolution_err = 100 * std::abs(sigma_err / sigma - mean_err / mean);

        mean_vec.push_back(mean);
        sigma_vec.push_back(sigma);
        resolution_vec.push_back(resolution);
        mean_err_vec.push_back(mean_err);
        sigma_err_vec.push_back(sigma_err);
        resolution_err_vec.push_back(resolution_err);
        // h->Draw("p");
        auto _pass_rate = (double)_event_pass_chi2_cnt * 100 / (double)_total_event_cnt;
        LOG(INFO) << "Run " << _run_number << " pass rate: " << _pass_rate << "%";
        delete gaus;
        delete h;
    }

    // * Draw resolution vs. energy
    auto canvas = new TCanvas("canvas", "canvas", 1400, 700);
    // set high dpi
    gStyle->SetOptTitle(0); // remove title
    // gStyle->SetOptFit(1011);

    auto gr                 = new TGraphErrors();
    auto gr_rado_MC         = new TGraphErrors();
    auto gr_rado_MayMixed   = new TGraphErrors();
    auto gr_rado_MayLG      = new TGraphErrors();

    for (int i = 0; i < run_energy_array.size(); i++){
        gr->SetPoint(i, run_energy_array.at(i), resolution_vec.at(i)/Double_t(100));
        gr->SetPointError(i, 0, resolution_err_vec.at(i)/Double_t(100));
    }
    
    for (int i = 0; i < rado_MC_energy.size(); i++){
        gr_rado_MC->SetPoint(i, rado_MC_energy.at(i), rado_MC_reso.at(i));
        gr_rado_MC->SetPointError(i, 0, rado_MC_reso_err.at(i));
    }

    for (int i = 0; i < rado_May_Mixed_energy.size(); i++){
        gr_rado_MayMixed->SetPoint(i, rado_May_Mixed_energy.at(i), rado_May_Mixed_reso.at(i));
        gr_rado_MayMixed->SetPointError(i, 0, rado_May_Mixed_reso_err.at(i));
    }

    for (int i = 0; i < rado_May_LG_energy.size(); i++){
        gr_rado_MayLG->SetPoint(i, rado_May_LG_energy.at(i), rado_May_LG_reso.at(i));
        gr_rado_MayLG->SetPointError(i, 0, rado_May_LG_reso_err.at(i));
    }

    gr->SetTitle("2-D Dual Gaussian Fit");
    gr_rado_MC->SetTitle("Mento Carlo");
    gr_rado_MayMixed->SetTitle("tb2023 May Mixed");
    gr_rado_MayLG->SetTitle("tb2023 May LG");

    gr_rado_MC->GetXaxis()->SetTitle("Energy [GeV]");
    gr_rado_MC->GetYaxis()->SetTitle("#sigma (E) / E");

    // x axis range
    gr->GetXaxis()->SetLimits(0, 400);
    gr->GetXaxis()->SetRangeUser(0, 400);
    gr->GetYaxis()->SetLimits(0.07, 0.4);
    gr->GetYaxis()->SetRangeUser(0.07, 0.4);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.5);
    gr->SetMarkerColor(kCyan-2);
    gr->SetLineStyle(1);

    gr_rado_MC->GetXaxis()->SetLimits(0, 400);
    gr_rado_MC->GetXaxis()->SetRangeUser(0, 400);
    gr_rado_MC->GetYaxis()->SetLimits(0.07,0.4);
    gr_rado_MC->GetYaxis()->SetRangeUser(0.07,0.4);
    gr_rado_MC->SetMarkerStyle(22);
    gr_rado_MC->SetMarkerSize(2);
    gr_rado_MC->SetMarkerColor(kRed);
    gr_rado_MC->SetLineStyle(3);

    gr_rado_MayMixed->GetXaxis()->SetRangeUser(0, 400);
    gr_rado_MayMixed->GetYaxis()->SetRangeUser(0.07, 0.4);
    gr_rado_MayMixed->SetMarkerStyle(21);
    gr_rado_MayMixed->SetMarkerSize(1.5);
    gr_rado_MayMixed->SetMarkerColor(kOrange-6);
    gr_rado_MayMixed->SetLineStyle(7);


    gr_rado_MayLG->GetXaxis()->SetRangeUser(0, 400);
    gr_rado_MayLG->GetYaxis()->SetRangeUser(0.07, 0.4);
    gr_rado_MayLG->SetMarkerStyle(47);
    gr_rado_MayLG->SetMarkerSize(2);
    gr_rado_MayLG->SetMarkerColor(kBlack);
    gr_rado_MayLG->SetLineStyle(4);

    // set error bar style
    gr->SetLineWidth(3);
    gr->SetLineColor(kCyan-2);
    gr->SetLineStyle(1);

    gr_rado_MC->SetLineWidth(3);
    gr_rado_MC->SetLineColor(kRed);
    gr_rado_MC->SetLineStyle(1);

    gr_rado_MayMixed->SetLineWidth(3);
    gr_rado_MayMixed->SetLineColor(kOrange-6);
    gr_rado_MayMixed->SetLineStyle(1);

    gr_rado_MayLG->SetLineWidth(3);
    gr_rado_MayLG->SetLineColor(kBlack);
    gr_rado_MayLG->SetLineStyle(1);

    auto er_func = new TF1("er_func", SJFunc::energy_resolution_func, 30, 370, 3);
    auto er_func_rado_MC = new TF1("er_func_rado_MC", SJFunc::energy_resolution_func, 30, 370, 3);
    auto er_func_rado_MayMixed = new TF1("er_func_rado_MayMixed", SJFunc::energy_resolution_func, 30, 370, 3);
    auto er_func_rado_MayLG = new TF1("er_func_rado_MayLG", SJFunc::energy_resolution_func, 30, 370, 3);

    er_func->SetParNames("p0", "p1", "p3");
    er_func->SetParameters(1, 0, 0.15);
    er_func_rado_MC->SetParNames("p0", "p1", "p3");
    er_func_rado_MC->SetParameters(1, 0, 0.15);
    er_func_rado_MayMixed->SetParNames("p0", "p1", "p3");
    er_func_rado_MayMixed->SetParameters(1, 0, 0.15);
    er_func_rado_MayLG->SetParNames("p0", "p1", "p3");
    er_func_rado_MayLG->SetParameters(1, 0, 0.15);

    er_func->SetLineColor(kCyan-2);
    er_func->SetLineWidth(3);

    er_func_rado_MC->SetLineColor(kRed);
    er_func_rado_MC->SetLineWidth(3);

    er_func_rado_MayMixed->SetLineColor(kOrange-6);
    er_func_rado_MayMixed->SetLineWidth(3);

    er_func_rado_MayLG->SetLineColor(kBlack);
    er_func_rado_MayLG->SetLineWidth(3);
    // make dashed line
    er_func->SetLineStyle(1);
    er_func_rado_MC->SetLineStyle(3);
    er_func_rado_MayMixed->SetLineStyle(7);
    er_func_rado_MayLG->SetLineStyle(4);

    auto fit_option = "ER";
    // * print all gr data points
    for (auto i = 0; i < gr->GetN(); i++){
        auto x = gr->GetX()[i];
        auto y = gr->GetY()[i];
        LOG(DEBUG) << "x: " << x << ", y: " << y;
    }
    gr->Fit(er_func, fit_option);
    gr_rado_MC->Fit(er_func_rado_MC, fit_option);
    gr_rado_MayMixed->Fit(er_func_rado_MayMixed, fit_option);
    gr_rado_MayLG->Fit(er_func_rado_MayLG, fit_option);

    // * print point-by-point chi2/ndf
    auto x_val = std::vector<double>();
    x_val.push_back(60);
    x_val.push_back(80);
    x_val.push_back(100);
    x_val.push_back(150);
    x_val.push_back(200);
    x_val.push_back(250);
    x_val.push_back(300);
    x_val.push_back(350);

    // * Manual calculation of chi2/ndf
    auto chi2_manual_gr = 0.0;
    for (auto dot: x_val){
        auto y_pred = er_func->Eval(dot);
        auto y = gr->Eval(dot);
        LOG(DEBUG) << "x: " << dot << ", y_pred: " << y_pred << ", y: " << y;
        chi2_manual_gr += std::pow((y - y_pred), 2) / er_func->GetParameter(2) / er_func->GetParameter(2);
    }
    LOG(DEBUG) << "chi2_manual_gr: " << chi2_manual_gr << " auto chi2:" << er_func->GetChisquare();

    auto chi2_manual_rado_MC = 0.0;
    for (auto dot: x_val){
        auto y_pred = er_func_rado_MC->Eval(dot);
        auto y = gr_rado_MC->Eval(dot);
        chi2_manual_rado_MC += std::pow((y - y_pred), 2) / y / y;
    }
    LOG(DEBUG) << "chi2_manual_rado_MC: " << chi2_manual_rado_MC << " auto chi2:" << er_func_rado_MC->GetChisquare();

    auto chi2_manual_rado_MayMixed = 0.0;
    for (auto dot: x_val){
        auto y_pred = er_func_rado_MayMixed->Eval(dot);
        auto y = gr_rado_MayMixed->Eval(dot);
        chi2_manual_rado_MayMixed += std::pow((y - y_pred), 2) / y / y;
    }
    LOG(DEBUG) << "chi2_manual_rado_MayMixed: " << chi2_manual_rado_MayMixed << " auto chi2:" << er_func_rado_MayMixed->GetChisquare();

    auto chi2_manual_rado_MayLG = 0.0;
    for (auto dot: x_val){
        auto y_pred = er_func_rado_MayLG->Eval(dot);
        auto y = gr_rado_MayLG->Eval(dot);
        chi2_manual_rado_MayLG += std::pow((y - y_pred), 2) / y / y;
    }
    LOG(DEBUG) << "chi2_manual_rado_MayLG: " << chi2_manual_rado_MayLG << " auto chi2:" << er_func_rado_MayLG->GetChisquare();

    auto chi2ndf_info_1 = "Chi2/NDF = " + std::to_string(er_func->GetChisquare()) + "/" + std::to_string(er_func->GetNDF());
    auto chi2ndf_info_2 = "Chi2/NDF = " + std::to_string(er_func_rado_MC->GetChisquare()) + "/" + std::to_string(er_func_rado_MC->GetNDF());
    auto chi2ndf_info_3 = "Chi2/NDF = " + std::to_string(er_func_rado_MayMixed->GetChisquare()) + "/" + std::to_string(er_func_rado_MayMixed->GetNDF());
    auto chi2ndf_info_4 = "Chi2/NDF = " + std::to_string(er_func_rado_MayLG->GetChisquare()) + "/" + std::to_string(er_func_rado_MayLG->GetNDF());

    // auto chi2ndf_info_1 = "Chi2/NDF = " + std::to_string(chi2_manual_gr) + "/" + std::to_string(er_func->GetNDF());
    // auto chi2ndf_info_2 = "Chi2/NDF = " + std::to_string(chi2_manual_rado_MC) + "/" + std::to_string(er_func_rado_MC->GetNDF());
    // auto chi2ndf_info_3 = "Chi2/NDF = " + std::to_string(chi2_manual_rado_MayMixed) + "/" + std::to_string(er_func_rado_MayMixed->GetNDF());
    // auto chi2ndf_info_4 = "Chi2/NDF = " + std::to_string(chi2_manual_rado_MayLG) + "/" + std::to_string(er_func_rado_MayLG->GetNDF());

    auto p0_info_1 = "p0 = " + std::to_string(er_func->GetParameter(0)) + " #pm " + std::to_string(er_func->GetParError(0));
    auto p0_info_2 = "p0 = " + std::to_string(er_func_rado_MC->GetParameter(0)) + " #pm " + std::to_string(er_func_rado_MC->GetParError(0));
    auto p0_info_3 = "p0 = " + std::to_string(er_func_rado_MayMixed->GetParameter(0)) + " #pm " + std::to_string(er_func_rado_MayMixed->GetParError(0));
    auto p0_info_4 = "p0 = " + std::to_string(er_func_rado_MayLG->GetParameter(0)) + " #pm " + std::to_string(er_func_rado_MayLG->GetParError(0));

    auto p1_info_1 = "p1 = " + std::to_string(er_func->GetParameter(1)) + " #pm " + std::to_string(er_func->GetParError(1));
    auto p1_info_2 = "p1 = " + std::to_string(er_func_rado_MC->GetParameter(1)) + " #pm " + std::to_string(er_func_rado_MC->GetParError(1));
    auto p1_info_3 = "p1 = " + std::to_string(er_func_rado_MayMixed->GetParameter(1)) + " #pm " + std::to_string(er_func_rado_MayMixed->GetParError(1));
    auto p1_info_4 = "p1 = " + std::to_string(er_func_rado_MayLG->GetParameter(1)) + " #pm " + std::to_string(er_func_rado_MayLG->GetParError(1));

    auto p2_info_1 = "p2 = " + std::to_string(er_func->GetParameter(2)) + " #pm " + std::to_string(er_func->GetParError(2));
    auto p2_info_2 = "p2 = " + std::to_string(er_func_rado_MC->GetParameter(2)) + " #pm " + std::to_string(er_func_rado_MC->GetParError(2));
    auto p2_info_3 = "p2 = " + std::to_string(er_func_rado_MayMixed->GetParameter(2)) + " #pm " + std::to_string(er_func_rado_MayMixed->GetParError(2));
    auto p2_info_4 = "p2 = " + std::to_string(er_func_rado_MayLG->GetParameter(2)) + " #pm " + std::to_string(er_func_rado_MayLG->GetParError(2));

    
    // auto watermark = new TPaveText(0.1, 0.2, 0.8, 0.8, "NDC");
    auto watermark = new TLatex(40.85, 0.15, "Very Preliminary");
    gr_rado_MC->Draw("AP");

    watermark->SetTextSize(0.18);
    watermark->SetTextColorAlpha(17, 0.4);
    watermark->SetTextAlign(13);
    watermark->SetTextAngle(24); // 45 degrees
    
    
    gr_rado_MayMixed->Draw("P");
    gr_rado_MayLG->Draw("P");
    gr->Draw("P");
    
    auto info_box_1 = new TPaveText(0.7, 0.77, 0.9, 0.9, "NDC");
    info_box_1->AddText(chi2ndf_info_2.c_str());
    info_box_1->AddText(p0_info_2.c_str());
    info_box_1->AddText(p1_info_2.c_str());
    info_box_1->AddText(p2_info_2.c_str());
    info_box_1->SetTextFont(42);
    info_box_1->SetTextSize(0.03);
    info_box_1->SetTextColor(kRed);
    info_box_1->SetFillColor(kWhite);
    info_box_1->SetBorderSize(1);

    auto info_box_2 = new TPaveText(0.7, 0.64, 0.9, 0.77, "NDC");
    info_box_2->AddText(chi2ndf_info_3.c_str());
    info_box_2->AddText(p0_info_3.c_str());
    info_box_2->AddText(p1_info_3.c_str());
    info_box_2->AddText(p2_info_3.c_str());
    info_box_2->SetTextFont(42);
    info_box_2->SetTextSize(0.03);
    info_box_2->SetTextColor(kOrange-6);
    info_box_2->SetFillColor(kWhite);
    info_box_2->SetBorderSize(1);

    auto info_box_3 = new TPaveText(0.7, 0.51, 0.9, 0.64, "NDC");
    info_box_3->AddText(chi2ndf_info_4.c_str());
    info_box_3->AddText(p0_info_4.c_str());
    info_box_3->AddText(p1_info_4.c_str());
    info_box_3->AddText(p2_info_4.c_str());
    info_box_3->SetTextFont(42);
    info_box_3->SetTextSize(0.03);
    info_box_3->SetTextColor(kBlack);
    info_box_3->SetFillColor(kWhite);
    info_box_3->SetBorderSize(1);

    auto info_box_4 = new TPaveText(0.7, 0.38, 0.9, 0.51, "NDC");
    info_box_4->AddText(chi2ndf_info_1.c_str());
    info_box_4->AddText(p0_info_1.c_str());
    info_box_4->AddText(p1_info_1.c_str());
    info_box_4->AddText(p2_info_1.c_str());
    info_box_4->SetTextFont(42);
    info_box_4->SetTextSize(0.03);
    info_box_4->SetTextColor(kCyan-2);
    info_box_4->SetFillColor(kWhite);
    info_box_4->SetBorderSize(1);


    info_box_1->Draw();
    info_box_2->Draw();
    info_box_3->Draw();
    info_box_4->Draw();

    auto info_box_fit_func = new TPaveText(0.3, 0.7, 0.5, 0.9, "NDC");
    info_box_fit_func->AddText("fit function:  #sqrt{(#frac{p_{0}}{#sqrt{E}})^{2} + (#frac{p_{1}}{E})^{2} + p_{2}^{2}}");
    info_box_fit_func->SetTextFont(42);
    info_box_fit_func->SetTextSize(0.04);
    info_box_fit_func->SetTextColor(kBlack);
    info_box_fit_func->SetFillColorAlpha(kWhite, 0);
    info_box_fit_func->SetBorderSize(0);
    info_box_fit_func->Draw();

    // er_func->Draw("same");
    canvas->SetGrid();
    // remove legend
    auto legend = new TLegend(0.55, 0.7, 0.7, 0.9);
    legend->AddEntry(gr_rado_MC, "MC", "lep");
    legend->AddEntry(gr_rado_MayMixed, "May Mixed", "lep");
    legend->AddEntry(gr_rado_MayLG, "May LG", "lep");
    legend->AddEntry(gr, "2D Fitting", "lep");
    legend->SetTextSize(0.03);
    legend->Draw();

    watermark->Draw();
    canvas->SaveAs("../pics/resolution_vs_energy.png");

    LOG(INFO) << "Chi2/ndf: " << er_func->GetChisquare()/er_func->GetNDF();

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