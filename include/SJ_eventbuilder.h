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
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

#include "SJ_datareading.h"
#include "SJ_utilities.h"
#include "easylogging++.h"  // for logging

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
    bool reconstruct_event(std::vector<CAEN_data_reader::FrameInfo> *_frame_array_ptr, int _frame_num = EVENT_FRAME_NUM, bool _pedestal_subtraction=false);

    // * Get the HG charge sum of all channels in the event
    // * return: HG charge sum of all channels in the event
    std::vector<Int_t> get_HG_charge_sum_array();
    // * Get the LG charge sum of all channels in the event
    // * return: LG charge sum of all channels in the event
    std::vector<Int_t> get_LG_charge_sum_array();

    bool write_event_array2root_file(const char *_root_file_name);
    bool read_root_file2event_array(const char *_root_file_name);

    inline std::vector<EventInfo> *get_event_array_ptr(){return event_array_ptr;}

    inline std::vector<bool> *get_event_valid_array_ptr(){return event_valid_array_ptr;}

    // * Copy pedestal info
    inline void set_pedestal_info(SJUtil::PedestalInfo _pedestal_info){pedestal_info = _pedestal_info;}

private:
    std::vector<EventInfo> *event_array_ptr;
    std::vector<bool> *event_valid_array_ptr;
    SJUtil::PedestalInfo pedestal_info;
};