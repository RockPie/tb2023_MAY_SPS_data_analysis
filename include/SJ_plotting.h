#pragma once

#include "SJ_eventbuilder.h"
#include "SJ_utilities.h"
#include "easylogging++.h"

#include <TColor.h>
#include <TGraph2D.h>
#include <TH2F.h>
#include <TH3F.h>

INITIALIZE_EASYLOGGINGPP

namespace SJPlot{
    TH2F* distribution_2d(const std::vector<std::vector<Short_t>> &_events_charges, const char* _name, const char* _title);

    TGraph2D* scatter_3d(const std::vector<Short_t> &_events_charges, const std::vector<std::vector<Short_t>> _mapping_coords, const char* _name, const char* _title);
    TGraph2D* scatter_3d(const std::vector<Short_t> &_events_charges, const char* _name, const char* _title);

    TH3F* distribution_3d(const std::vector<std::vector<Short_t>> &_events_charges, const char* _name, const char* _title);

};