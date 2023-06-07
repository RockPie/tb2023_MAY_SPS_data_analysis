#pragma once

#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"
#include "easylogging++.h"

#include <TColor.h>
#include <TGraph2D.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TLine.h>

INITIALIZE_EASYLOGGINGPP

namespace SJPlot{
    TH2F* distribution_2d(const std::vector<std::vector<Short_t>> &_events_charges, const char* _name, const char* _title);

    TGraph2D* scatter_3d(
        const std::vector<Short_t> &_events_charges, 
        const std::vector<std::vector<Short_t>> _mapping_coords, 
        const char* _name, 
        const char* _title);
    TGraph2D* scatter_3d(const std::vector<Short_t> &_events_charges, const char* _name, const char* _title);

    TGraph2D* scatter_3d_raw(
        const SJUtil::DataSet2D<Short_t> &_mapped_events, 
        const char* _name, 
        const char* _title);

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