#pragma once

#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"
#include "easylogging++.h"

#include <TColor.h>
#include <TGraph2D.h>
#include <TGraph2DErrors.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TLine.h>

INITIALIZE_EASYLOGGINGPP

namespace SJPlot{
    TH2D* distribution_2d(const std::vector<std::vector<Short_t>> &_events_charges, const char* _name, const char* _title);

    TGraph2D* scatter_3d(
        const std::vector<Short_t> &_events_charges, 
        const std::vector<std::vector<Short_t>> _mapping_coords, 
        const char* _name, 
        const char* _title);
    TGraph2D* scatter_3d(const std::vector<Short_t> &_events_charges, const char* _name, const char* _title);

    template <typename T>
    TGraph2D* scatter_3d_raw(
        const SJUtil::DataSet2D<T> &_mapped_events, 
        const char* _name, 
        const char* _title){
        auto _graph_ptr = new TGraph2D();
        _graph_ptr->SetMarkerStyle(20);
        _graph_ptr->SetMarkerSize(1);
        _graph_ptr->SetMarkerColor(kRed);

        _graph_ptr->SetTitle(_title);
        _graph_ptr->SetName(_name);

        for (auto i = 0; i < _mapped_events.x_vec.size(); i++) {
            _graph_ptr->SetPoint(i, 
                Double_t(_mapped_events.x_vec[i]),
                Double_t(_mapped_events.y_vec[i]), 
                Double_t(_mapped_events.value_vec[i]));
        }

        _graph_ptr->GetXaxis()->SetTitle("x");
        _graph_ptr->GetYaxis()->SetTitle("y");
        _graph_ptr->GetZaxis()->SetTitle("adc");

        _graph_ptr->GetXaxis()->SetRangeUser(0, 105);
        _graph_ptr->GetYaxis()->SetRangeUser(0, 105);
        // _graph_ptr->Draw("pcol");
        return _graph_ptr;
    };

    template <typename T>
    TGraph2DErrors* scatter_3d_raw_errors(
        const SJUtil::DataSet2D<T> &_mapped_events, 
        const char* _name, 
        const char* _title){
        auto _graph_ptr = new TGraph2DErrors();
        _graph_ptr->SetMarkerStyle(20);
        _graph_ptr->SetMarkerSize(1);
        _graph_ptr->SetMarkerColor(kRed);

        _graph_ptr->SetTitle(_title);
        _graph_ptr->SetName(_name);

        for (auto i = 0; i < _mapped_events.x_vec.size(); i++) {
            _graph_ptr->SetPoint(i, 
                Double_t(_mapped_events.x_vec[i]),
                Double_t(_mapped_events.y_vec[i]), 
                Double_t(_mapped_events.value_vec[i]));
                _graph_ptr->SetPointError(i, 0, 0, 7.235);
            // _graph_ptr->SetPointError(i, 1.44338, 1.44338, 7.235);
            // * 5/sqrt(2) for quantization error for position

        }

        _graph_ptr->GetXaxis()->SetTitle("x");
        _graph_ptr->GetYaxis()->SetTitle("y");
        _graph_ptr->GetZaxis()->SetTitle("adc");

        _graph_ptr->GetXaxis()->SetRangeUser(0, 105);
        _graph_ptr->GetYaxis()->SetRangeUser(0, 105);
        // _graph_ptr->Draw("pcol");
        return _graph_ptr;
    };

    TGraph2D* scatter_3d_double_raw(
        const SJUtil::DataSet2D<Short_t> &_mapped_events_0, 
        const SJUtil::DataSet2D<Short_t> &_mapped_events_1, 
        const char* _name, 
        const char* _title);

    TH3F* distribution_3d(const std::vector<std::vector<Short_t>> &_events_charges, const char* _name, const char* _title);

    inline TLine* add_horizontal_line(const Double_t _y, const Double_t _x_min, const Double_t _x_max, const Color_t _color, const Style_t _style, const Width_t _width){
        auto _line_ptr = new TLine(_x_min, _y, _x_max, _y);
        _line_ptr->SetLineColor(_color);
        _line_ptr->SetLineStyle(_style);
        _line_ptr->SetLineWidth(_width);
        _line_ptr->Draw();
        return _line_ptr;
    };
    inline TLine* add_vertical_line(const Double_t _x, const Double_t _y_min, const Double_t _y_max, const Color_t _color, const Style_t _style, const Width_t _width){
        auto _line_ptr = new TLine(_x, _y_min, _x, _y_max);
        _line_ptr->SetLineColor(_color);
        _line_ptr->SetLineStyle(_style);
        _line_ptr->SetLineWidth(_width);
        _line_ptr->Draw();
        return _line_ptr;
    };
};