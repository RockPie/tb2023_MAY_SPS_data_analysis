// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 02-06-2023
// ***********************************************************

#pragma once

#define EVENT_FRAME_NUM  4
#define INFINITE_FRAMES -1
#define DEFAULT_MAPPING_FILE_PATH "../Mapping_tb2023SPS.csv"

#include <vector>
#include <list>
#include <unordered_map>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

#include "SJ_datareading.h"
#include "easylogging++.h"  // for logging

INITIALIZE_EASYLOGGINGPP

class CAEN_event_builder{
public:
    struct EventInfo{
        Long_t trigID;
        std::vector<Double_t> timestamps;
        std::vector<Short_t>  HG_charges;
        std::vector<Short_t>  LG_charges;
    };
public:
    CAEN_event_builder();
    ~CAEN_event_builder();

public:
    // * Reconstruct event from frame array
    // * Param: _frame_array_ptr: pointer to the frame array
    // *        _frame_num: number of frames to be reconstructed
    // * return: true if success, false if failed
    bool reconstruct_event(std::vector<CAEN_data_reader::FrameInfo> *_frame_array_ptr, int _frame_num = EVENT_FRAME_NUM);

    std::vector<Int_t> get_event_sum_array();

    bool write_event_array2root_file(const char *_root_file_name);
    bool read_root_file2event_array(const char *_root_file_name);

private:
    std::vector<EventInfo> *event_array_ptr;
    std::vector<bool> *event_valid_array_ptr;
};