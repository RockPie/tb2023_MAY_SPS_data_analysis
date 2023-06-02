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
#define DEFAULT_ROOT_FILE_FOLDER     "../cachedFiles/"

#include <fstream>      // for file reading
#include <string>       // for std::string operations
#include <stdexcept>    // for std::stio
#include <optional>     // for std::optional
#include <vector>       // for std::vector
#include <TFile.h>      // for root file writing/reading
#include <TTree.h>      // for creating root tree
#include "easylogging++.h"  // for logging

INITIALIZE_EASYLOGGINGPP

class CAEN_data_reader {
public:
    struct FrameInfo {
        Short_t    nboards;   // 2-byte int
        Double_t  timestamp;  // 8-byte float
        Long_t    trigID;     // 8-byte int
        std::vector<Short_t> CH;   // 2-byte int
        std::vector<Short_t> HG_charge; // 2-byte int
        std::vector<Short_t> LG_charge; // 2-byte int 
        std::vector<Short_t> TS;        // 2-byte int 
        std::vector<Short_t> ToT;       // 2-byte int
    };
    
public: 
    // * Default constructor
    CAEN_data_reader();
    // * Constructor
    // * Param: _file_name: CAEN file name
    explicit CAEN_data_reader(const char *_file_name);
    ~CAEN_data_reader();

public:
    // * Set CAEN file name
    // * Param: _file_name: CAEN file name
    // * Note: this function will reset the file stream and total line number
    // * Return true if success, false if failed
    bool set_caen_file(const char *_file_name);

    // * Print CAEN file header
    // * Note: this function will read first 6 lines of the file
    void print_caen_file_header();

    // * Get line number of CAEN file
    // * Note: this function will read the whole file
    // * Return line number if success, INVALID_RUN_NUMBER if failed
    ULong64_t get_line_num();

    // * Extract frame info from CAEN file
    // * Param: _get_chn_val:  whether to extract channel value
    // *        _flag_end:     indicate whether the end of file is reached,
    // *                       mainly for extract_frame_info_array()
    // * Return std::optional<FrameInfo> if success, std::nullopt if failed
    std::optional<FrameInfo> extract_frame_info(bool _get_chn_val = true);
    std::optional<FrameInfo> extract_frame_info(bool &_flag_end, bool _get_chn_val);

    // * Extract frame info array from CAEN file
    // * Param: _n_frames:     number of frames to extract, default to INFINITE_FRAMES
    // *        _get_chn_val:  whether to extract channel value
    // * Return true if success, false if failed
    bool extract_frame_info_array(long _n_frames = INFINITE_FRAMES, bool _get_chn_val = true);

    // * Get std::vector<FrameInfo> pointer
    // * Return std::vector<FrameInfo> pointer if success, nullptr if failed
    inline std::vector<FrameInfo> * get_frame_info_array_ptr(){
        if (!flag_frame_info_array_valid) {
            LOG(ERROR) << "Frame info array is not valid for ptr return!";
            return nullptr;
        }
        return this->frame_info_array;
    };

    // * Write std::vector<FrameInfo> to root file
    // * Param: _root_file_name:        root file name
    // *        _frame_info_array_ptr:  pointer to std::vector<FrameInfo>
    // * Return true if success, false if failed
    bool write_frame_array2root_file(const char *_root_file_name, std::vector<FrameInfo> *_frame_info_array_ptr);
    bool write_frame_array2root_file();
    inline bool write_frame_array2root_file(const char *_root_file_name){
        if (!flag_frame_info_array_valid) {
            LOG(ERROR) << "Frame info array is not valid for saving!";
            return false;
        }
        return write_frame_array2root_file(_root_file_name, this->frame_info_array);
    };



private:
    bool create_file_ptr();
    inline Int_t get_run_number(){
        if (!flag_caen_file_valid){
            LOG(ERROR) << "CAEN file is not valid for run number!";
            return INVALID_RUN_NUMBER;
        }
        return get_run_number(this->caen_file_name);
    };
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