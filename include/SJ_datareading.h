// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 01-06-2023
// ***********************************************************

#ifndef SJ_DATAREADING_H
#define SJ_DATAREADING_H

#define CAEN_FILE_HEADER_LINE_NUM    6
#define INVALID_RUN_NUMBER          -1
#define INVALID_CHN_VALUE           -1
#define DEFAULT_BOARD_NUMBER         4
#define DEFAULT_CHANNEL_NUMBER       64
#define INFINITE_FRAMES             -1

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>    // for std::stio
#include <map>
#include <optional>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include "easylogging++.h"  // for logging

INITIALIZE_EASYLOGGINGPP

class CAEN_data_reader {
public:
    struct FrameInfo {
        Char_t    nboards;    // 1-byte int
        Double_t  timestamp;  // 8-byte float
        Long_t    trigID;     // 8-byte int
        std::vector<Short_t> HG_charge; // 2-byte int
        std::vector<Short_t> LG_charge; // 2-byte int 
        std::vector<Short_t> TS;        // 2-byte int 
        std::vector<Short_t> ToT;       // 2-byte int
    };

    struct Event {
        Int_t id;         // 4-byte int
        Double_t ts0;     // 8-byte float
        Double_t ts1;     // 8-byte float
        std::map<UShort_t, UShort_t> charge0; // 2-byte int
        std::map<UShort_t, UShort_t> charge1; // 2-byte int
    };
    
public: 
    CAEN_data_reader();
    explicit CAEN_data_reader(const char *_file_name);
    ~CAEN_data_reader();

public:
    bool set_caen_file(const char *_file_name);
    void print_caen_file_header();
    ULong64_t get_line_num();
    std::optional<FrameInfo> extract_frame_info(bool _get_chn_val = true);
    std::optional<FrameInfo> extract_frame_info(bool &_flag_end, bool _get_chn_val);
    bool extract_frame_info_array(long _n_frames = INFINITE_FRAMES, bool _get_chn_val = true);

    inline std::vector<FrameInfo> * get_frame_info_array_ptr(){
        if (!flag_frame_info_array_valid) {
            LOG(ERROR) << "Frame info array is not valid!";
            return nullptr;
        }
        return this->frame_info_array;
    };

private:
    bool create_file_ptr();
    Int_t get_run_number(const std::string *_file_name);
    inline void reset_file_ptr(){
        if (!flag_caen_file_opened) {
            LOG(ERROR) << "CAEN file is not opened!";
            return;
        }
        this->caen_file_ptr->clear();
        this->caen_file_ptr->seekg(0, std::ios::beg);
    };
    
private:
    std::string  *caen_file_name;
    std::fstream *caen_file_ptr;
    std::vector<FrameInfo> *frame_info_array;
    Int_t run_number = INVALID_RUN_NUMBER;
    Int_t decode_board_number;
    Int_t decode_channel_number;
    ULong64_t total_line_num = 0;
    bool  flag_caen_file_opened;
    bool  flag_caen_file_valid;
    bool  flag_frame_info_array_valid;
};

#endif // SJ_DATAREADING_H