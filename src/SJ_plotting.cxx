#include "SJ_plotting.h"

// * input data _events_charges is like:
// * _events_charges -- [event0]
// *                |     |--- [channel0, channel1, ..., channel248]
// *                |--- [event1]
// *                |--- [event2]
// *                |--- ...
TH2D *SJPlot::distribution_2d(const std::vector<std::vector<Short_t>> &_events_charges, const char *_name, const char *_title){
    std::vector<Long_t> _chn_sums;
    _chn_sums.resize(_events_charges[0].size());
    for (auto _value : _chn_sums) _value = 0;

    for (auto _event_charges : _events_charges)
        for (auto i = 0; i < _event_charges.size(); i++)
            _chn_sums[i] += _event_charges[i];

    auto _mapping_info  = SJUtil::read_mapping_csv_file(DEFAULT_MAPPING_FILE_PATH);
    auto _mapped_coords = SJUtil::generate_mapping_croodinate(_mapping_info);

    SJUtil::DataSet2D<Long_t> _mapped_values = SJUtil::map1d_to_2d (_chn_sums, _mapped_coords);

    auto _x_array = _mapped_values.x_vec;
    auto _y_array = _mapped_values.y_vec;
    auto _z_array = _mapped_values.value_vec;

    std::vector<Double_t> _z_normalized_array;
    Double_t _array_length = Double_t(_z_array.size());
    for (auto _z_value: _z_array){
        if (_z_value == INVALID_2D_VALUE) _z_normalized_array.push_back(INVALID_2D_VALUE);
        else _z_normalized_array.push_back(_z_value / _array_length);
    }

    auto _hist_ptr = new TH2D(_name, _title, 105, 0, 105, 105, 0, 105);
    // * Set each bin value according to the nearest point
    for (auto i = 0; i < 105; i++)
        for (auto j = 0; j < 105; j++){
            auto _z = Double_t(INVALID_2D_VALUE);
            auto _min_dist_x = 105;
            auto _min_dist_y = 105;
            if (i >= 35 && i < 70 && 
                j >= 35 && j < 70){
                //* Central module
                for (auto k=0; k<_x_array.size();k++)
                    if (_x_array[k] >= 35 && _x_array[k] < 70 && 
                        _y_array[k] >= 35 && _y_array[k] < 70 && _z_normalized_array[k] != INVALID_2D_VALUE){
                        auto _dist_x = abs(i+1-_x_array[k]);
                        auto _dist_y = abs(j-1-_y_array[k]);
                        if (_dist_x <= _min_dist_x && _dist_y<=_min_dist_y){
                            _min_dist_x = _dist_x;
                            _min_dist_y = _dist_y;
                            _z = _z_normalized_array[k] / 25.0;
                        }
                    }
            }
            else {
                for (auto k=0; k<_x_array.size();k++)
                    if (((_x_array[k] < 35 || _x_array[k] >= 70) || 
                         (_y_array[k] < 35 || _y_array[k] >= 70)) && _z_normalized_array[k] != INVALID_2D_VALUE){
                        auto _dist_x = abs(i-_x_array[k]);
                        auto _dist_y = abs(j-_y_array[k]);
                        if (_dist_x <= _min_dist_x && _dist_y<=_min_dist_y){
                            _min_dist_x = _dist_x;
                            _min_dist_y = _dist_y;
                            _z = _z_normalized_array[k] / 49.0;
                        }
                    }
            }
            _hist_ptr->SetBinContent(i+1, j+1, _z);
        }

    // Set color
    _hist_ptr->SetContour(100);
    


    return _hist_ptr;
}

TH3F* SJPlot::distribution_3d(const std::vector<std::vector<Short_t>> &_events_charges, const char* _name, const char* _title){
    std::vector<Long_t> _chn_sums;
    _chn_sums.resize(_events_charges[0].size());
    for (auto _value : _chn_sums) _value = 0;

    for (auto _event_charges : _events_charges)
        for (auto i = 0; i < _event_charges.size(); i++)
            _chn_sums[i] += _event_charges[i];

    auto _mapping_info  = SJUtil::read_mapping_csv_file(DEFAULT_MAPPING_FILE_PATH);
    auto _mapped_coords = SJUtil::generate_mapping_croodinate(_mapping_info);

    SJUtil::DataSet2D<Long_t> _mapped_values = SJUtil::map1d_to_2d (_chn_sums, _mapped_coords);

    auto _x_array = _mapped_values.x_vec;
    auto _y_array = _mapped_values.y_vec;
    auto _z_array = _mapped_values.value_vec;

    auto _max_z = *std::max_element(_z_array.begin(), _z_array.end());


    auto _h_distri_ptr = new TH3F(_name, _title, 105, 0, 105, 105, 0, 105, 100, 0, _max_z);

    auto _array_size = _x_array.size();
    for (auto i = 0; i < _array_size; i++){
        if (_z_array[i] == INVALID_2D_VALUE) continue;
        _h_distri_ptr->SetBinContent(_x_array[i] + _y_array[i] * 105, double(_z_array[i]));
        LOG(DEBUG) << "x: " << _x_array[i] << ", y: " << _y_array[i] << ", z: " << _z_array[i];
    }

    _h_distri_ptr->GetXaxis()->SetTitle("x");
    _h_distri_ptr->GetYaxis()->SetTitle("y");
    _h_distri_ptr->GetZaxis()->SetTitle("z");

    _h_distri_ptr->SetFillColor(kRed);
    _h_distri_ptr->SetLineColor(kRed);
    _h_distri_ptr->SetBarWidth(100);
    _h_distri_ptr->SetBarOffset(0);

    return _h_distri_ptr;
}

TGraph2D* SJPlot::scatter_3d(const std::vector<Short_t> &_events_charges, const std::vector<std::vector<Short_t>> _mapping_coords, const char* _name, const char* _title){
    auto _graph_ptr = new TGraph2D();
    _graph_ptr->SetMarkerStyle(20);
    _graph_ptr->SetMarkerSize(2);
    _graph_ptr->SetMarkerColor(kRed);

    _graph_ptr->SetTitle(_title);
    _graph_ptr->SetName(_name);

    auto _twoD_values = SJUtil::map1d_to_2d(_events_charges, _mapping_coords);

    auto _noise_subtracted = SJUtil::noise_subtracted_data(_twoD_values, 20);

    for (auto i = 0; i < _noise_subtracted.x_vec.size(); i++){
        _graph_ptr->SetPoint(i, _noise_subtracted.x_vec[i], _noise_subtracted.y_vec[i], _noise_subtracted.value_vec[i]);
        // LOG(DEBUG) << "x: " << _twoD_values.x_vec[i] << ", y: " << _twoD_values.y_vec[i] << ", z: " << _twoD_values.value_vec[i];
    }

    _graph_ptr->GetXaxis()->SetTitle("x");
    _graph_ptr->GetYaxis()->SetTitle("y");
    _graph_ptr->GetZaxis()->SetTitle("adc");

    _graph_ptr->GetXaxis()->SetRangeUser(0, 105);
    _graph_ptr->GetYaxis()->SetRangeUser(0, 105);
    // _graph_ptr->Draw("pcol");
    return _graph_ptr;
}

TGraph2D* SJPlot::scatter_3d(const std::vector<Short_t> &_events_charges, const char* _name, const char* _title){
    auto _mapping_coords = SJUtil::generate_mapping_croodinate(SJUtil::read_mapping_csv_file(DEFAULT_MAPPING_FILE_PATH));
    return scatter_3d(_events_charges, _mapping_coords, _name, _title);
}

// TGraph2D* SJPlot::scatter_3d_raw(
//     const SJUtil::DataSet2D<Short_t> &_mapped_events, 
//     const char* _name, 
//     const char* _title){
//     auto _graph_ptr = new TGraph2D();
//     _graph_ptr->SetMarkerStyle(20);
//     _graph_ptr->SetMarkerSize(1);
//     _graph_ptr->SetMarkerColor(kRed);

//     _graph_ptr->SetTitle(_title);
//     _graph_ptr->SetName(_name);

//     for (auto i = 0; i < _mapped_events.x_vec.size(); i++)
//         _graph_ptr->SetPoint(i, 
//             _mapped_events.x_vec[i],
//             _mapped_events.y_vec[i], 
//             _mapped_events.value_vec[i]);

//     _graph_ptr->GetXaxis()->SetTitle("x");
//     _graph_ptr->GetYaxis()->SetTitle("y");
//     _graph_ptr->GetZaxis()->SetTitle("adc");

//     _graph_ptr->GetXaxis()->SetRangeUser(0, 105);
//     _graph_ptr->GetYaxis()->SetRangeUser(0, 105);
//     // _graph_ptr->Draw("pcol");
//     return _graph_ptr;
// }

TGraph2D* SJPlot::scatter_3d_double_raw(
    const SJUtil::DataSet2D<Short_t> &_mapped_events0, 
    const SJUtil::DataSet2D<Short_t> &_mapped_events1, 
    const char* _name, 
    const char* _title){
    auto _graph_ptr = new TGraph2D();
    _graph_ptr->SetMarkerStyle(20);
    _graph_ptr->SetMarkerSize(1);
    // _graph_ptr->SetMarkerColor(kBlue);

    _graph_ptr->SetTitle(_title);
    _graph_ptr->SetName(_name);

    auto _data_length0 = _mapped_events0.value_vec.size();
    auto _data_length1 = _mapped_events1.value_vec.size();

    for (auto i = 0; i < _data_length0; i++){
        _graph_ptr->SetPoint(i, 
            _mapped_events0.x_vec[i],
            _mapped_events0.y_vec[i], 
            _mapped_events0.value_vec[i]);
    }


    for (auto i = 0; i < _data_length1; i++){
        _graph_ptr->SetPoint(i+_data_length0, 
            _mapped_events1.x_vec[i],
            _mapped_events1.y_vec[i], 
            _mapped_events1.value_vec[i]*9);
    }

    _graph_ptr->GetXaxis()->SetTitle("x");
    _graph_ptr->GetYaxis()->SetTitle("y");
    _graph_ptr->GetZaxis()->SetTitle("adc");

    _graph_ptr->GetXaxis()->SetRangeUser(0, 105);
    _graph_ptr->GetYaxis()->SetRangeUser(0, 105);
    // _graph_ptr->Draw("pcol");
    return _graph_ptr;
}