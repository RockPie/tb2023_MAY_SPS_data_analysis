#include "SJ_datareading.h"

CAEN_data_reader::CAEN_data_reader():
    flag_caen_file_valid(false),
    flag_caen_file_opened(false),
    flag_frame_info_array_valid(false){
    LOG(INFO) << "CAEN reader created without file name";
}

CAEN_data_reader::CAEN_data_reader(const char *_file_name):
    flag_caen_file_valid(false),
    flag_caen_file_opened(false),
    flag_frame_info_array_valid(false){
    // Check if the file name is empty.
    if (strlen(_file_name) == 0){
        LOG(ERROR) << "File name is empty!";
        return;
    }
    auto _file_extension = strrchr(_file_name, '.');
    auto _file_extension_legit = _file_extension == std::string(".txt");
    if (!_file_extension_legit){
        LOG(ERROR) << "File type is not .txt!";
        return;
    }
    caen_file_name = new std::string(_file_name);
    this->run_number = this->get_run_number(caen_file_name);
    LOG(DEBUG) << "Run Number: " << this->run_number;
    this->flag_caen_file_valid = true;
    LOG(INFO) << "CAEN file name: " << *caen_file_name;
}

CAEN_data_reader::~CAEN_data_reader(){
    if (caen_file_name != nullptr)
        delete caen_file_name;
    if (caen_file_ptr != nullptr)
        delete caen_file_ptr;
    if (flag_frame_info_array_valid){
        frame_info_array->clear();
        frame_info_array->resize(0);
        delete frame_info_array;
    }
}

bool CAEN_data_reader::set_caen_file(const char *_file_name){
    if (flag_caen_file_opened){
        delete caen_file_ptr;
        flag_caen_file_opened = false;
        LOG(DEBUG) << "CAEN file closed";
    }
    if (flag_caen_file_valid){
        delete caen_file_name;
        flag_caen_file_valid = false;
        LOG(DEBUG) << "CAEN file name deleted";
    }
    if (strlen(_file_name) == 0){
        LOG(ERROR) << "File name is empty!";
        return false;
    }
    if (total_line_num != 0){
        total_line_num = 0;
        LOG(DEBUG) << "Total line number reset";
    }
    auto _file_extension = strrchr(_file_name, '.');
    auto _file_extension_legit = _file_extension == std::string(".txt");
    if (!_file_extension_legit){
        LOG(ERROR) << "File type is not txt!";
        return false;
    }
    caen_file_name = new std::string(_file_name);
    this->run_number = this->get_run_number(caen_file_name);
    LOG(DEBUG) << "Run Number: " << this->run_number;
    this->flag_caen_file_valid = true;
    LOG(INFO) << "New CAEN File Name: " << *caen_file_name;
    return true;
}

void CAEN_data_reader::print_caen_file_header(){
    if (!flag_caen_file_opened)
        if (!this->create_file_ptr())
            return;
    std::string _line;
    auto _line_count = 0;
    while (_line_count < CAEN_FILE_HEADER_LINE_NUM){
        std::getline(*caen_file_ptr, _line);
        LOG(INFO) << "line" << _line_count << " " << _line;
        _line_count++;
    }
    this->reset_file_ptr();
}

ULong64_t CAEN_data_reader::get_line_num(){
    if (!flag_caen_file_opened)
        if (!this->create_file_ptr())
            return 0;
    std::string _line;
    auto _line_count = 0;
    while (std::getline(*caen_file_ptr, _line))
        _line_count++;
    this->total_line_num = _line_count;
    return _line_count;
}

bool CAEN_data_reader::create_file_ptr(){
    if (!flag_caen_file_valid){
        LOG(ERROR) << "CAEN file is not valid!";
        return false;
    }
    if (flag_caen_file_opened){
        LOG(ERROR) << "CAEN file is already opened!";
        return false;
    }
    caen_file_ptr = new std::fstream(*caen_file_name);
    if (caen_file_ptr->is_open()){
        flag_caen_file_opened = true;
        return true;
    }
    else{
        LOG(ERROR) << "Unable to open file: " << caen_file_name;
        return false;
    }
}

Int_t CAEN_data_reader::get_run_number(const std::string *_file_name){
    if (_file_name == nullptr){
        LOG(ERROR) << "File name is empty!";
        return INVALID_RUN_NUMBER;
    }
    std::string _delimiter_1 = "_";
    std::string _delimiter_2 = "Run";

    std::string _file_name_str = *_file_name;

    auto _pos_1 = _file_name_str.find(_delimiter_1);
    auto _pos_2 = _file_name_str.find(_delimiter_2);

    if (_pos_1 == std::string::npos || _pos_2 == std::string::npos){
        LOG(ERROR) << "File name is not valid!";
        return INVALID_RUN_NUMBER;
    }

    auto _run_number_str = _file_name_str.substr(_pos_2 + 3, _pos_1 - _pos_2 - 3);


    auto _res = INVALID_RUN_NUMBER;
    try {
        _res = std::stoi(_run_number_str);
    }
    catch (const std::invalid_argument &ia){
        LOG(ERROR) << "Invalid argument: " << ia.what();
        return INVALID_RUN_NUMBER;
    }
    catch (const std::out_of_range &oor){
        LOG(ERROR) << "Out of Range error: " << oor.what();
        return INVALID_RUN_NUMBER;
    }

    return _res;
}

// * Line types are:
// * HEADER       //********************
// * BOARD_NUM    Board X
// * TIMESTAMP    TS=xx.xx us
// * TRGID        TrgID=xx
// * DATA_TYPE    CH  LG  HG  Ts(ns)  ToT(ns)
// * DATA         x   x   x   x       x
// * DIVIDER      ----------------------------------
std::optional<CAEN_data_reader::FrameInfo> CAEN_data_reader::extract_frame_info(bool _get_chn_val){
    FrameInfo _frame_info;
    bool _dummy_flag = false;
    return this->extract_frame_info(_dummy_flag, _get_chn_val);
}

std::optional<CAEN_data_reader::FrameInfo> CAEN_data_reader::extract_frame_info(bool &_flag_end, bool _get_chn_val){
    FrameInfo _frame_info;
    if (!flag_caen_file_opened){
        LOG(ERROR) << "CAEN file is not opened!";
        return std::nullopt;
    }
    std::string _line;
    bool _divider_found     = false;
    bool _board_num_found   = false;
    bool _timestamp_found   = false;
    bool _trigid_found      = false;
    bool _data_type_found   = false;

    while(!_divider_found && !caen_file_ptr->eof()){
        std::getline(*caen_file_ptr, _line);
        // * Sort line type based on first two characters
        auto _line_type = _line.substr(0, 2);
        if (_line_type == "Bo"){
            try {
                _frame_info.nboards = std::stoi(_line.substr(6));
                _board_num_found = true;
            }
            catch (const std::invalid_argument& _err) {
                LOG(ERROR) << "Invalid board number with " << _line.substr(6);
            }
            catch (const std::out_of_range& _err) {
                LOG(ERROR) << "Out of range board number with " << _line.substr(6);
            }
        }
        else if (_line_type == "TS") {
            // TODO: make sure the length of 10 is enough
            try {
                _frame_info.timestamp = std::stod(_line.substr(3));
                _timestamp_found = true;
            }
            catch (const std::invalid_argument& _err) {
                LOG(ERROR) << "Invalid timestamp with " << _line.substr(3);
            }
            catch (const std::out_of_range& _err) {
                LOG(ERROR) << "Out of range timestamp with " << _line.substr(3);
            }
        }
        else if (_line_type == "Tr") {
            // TODO: make sure the length of 6 is enough
            try {
                _frame_info.trigID = std::stoi(_line.substr(6));
                _trigid_found = true;
            }
            catch (const std::invalid_argument& _err) {
                LOG(ERROR) << "Invalid trigger ID with " << _line.substr(6);
            }
            catch (const std::out_of_range& _err) {
                LOG(ERROR) << "Out of range trigger ID with " << _line.substr(6);
            }
        }
        else if (_line_type == "CH")
            _data_type_found = true;
        else if (_line_type == "--")
            _divider_found = true;
        else if (_line_type == "//")
            continue;
            
        else {
            // ! divide by space
            // TODO: implement this
            if (_get_chn_val){
                LOG_IF(!_data_type_found, WARNING) << "Data without type line!";
                std::istringstream _iss(_line);
                Short_t _CH         = INVALID_CHN_VALUE;
                Short_t _HG_charge  = INVALID_CHN_VALUE;
                Short_t _LG_charge  = INVALID_CHN_VALUE;
                Short_t _TS         = INVALID_CHN_VALUE;
                Short_t _ToT        = INVALID_CHN_VALUE;
                _iss >> _CH >> _LG_charge >> _HG_charge >> _TS >> _ToT;
                _frame_info.CH.push_back(_CH);
                _frame_info.LG_charge.push_back(_LG_charge);
                _frame_info.HG_charge.push_back(_HG_charge);
                _frame_info.TS.push_back(_TS);
                _frame_info.ToT.push_back(_ToT);
            }
            else {
                _frame_info.CH.push_back(INVALID_CHN_VALUE);
                _frame_info.LG_charge.push_back(INVALID_CHN_VALUE);
                _frame_info.HG_charge.push_back(INVALID_CHN_VALUE);
                _frame_info.TS.push_back(INVALID_CHN_VALUE);
                _frame_info.ToT.push_back(INVALID_CHN_VALUE);
            }
        }
    }
    if (caen_file_ptr->eof()) {
        LOG(WARNING) << "Frame: End of file reached!";
        _flag_end = true;
        return std::nullopt;
    }
    auto _all_found = _board_num_found && _timestamp_found && _trigid_found && _data_type_found;
    if (_all_found)
        return _frame_info;
    else {
        LOG_IF(!_board_num_found, WARNING)  << "Board number is not found!";
        LOG_IF(!_timestamp_found, WARNING)  << "Timestamp is not found!";
        LOG_IF(!_trigid_found, WARNING)     << "TrigID is not found!";
        LOG_IF(!_data_type_found, WARNING)  << "Data type is not found!";
        return std::nullopt;
    }
    return std::nullopt;
}

bool CAEN_data_reader::extract_frame_info_array(long _n_frames, bool _get_chn_val){
    if (!flag_caen_file_opened)
        if (!this->create_file_ptr()){
            LOG(ERROR) << "CAEN file cannot be opened!";
            return false;
        }
    // ! this check is not precise
    auto _frame_requested_too_large = _n_frames * DEFAULT_CHANNEL_NUMBER >= this->total_line_num 
                                   && this->total_line_num != 0
                                   && _n_frames != INFINITE_FRAMES;
    LOG_IF(_frame_requested_too_large, WARNING) << "Requested frames are too large!";
    this->reset_file_ptr();
    if (!flag_frame_info_array_valid)
        this->frame_info_array = new std::vector<FrameInfo>;
    else
        this->frame_info_array->clear();

    bool _flag_file_end = false;
    LOG(INFO) << "Extracting frame info...";
    
    if (_n_frames == INFINITE_FRAMES)
        while (!_flag_file_end){
            auto _frame_info = this->extract_frame_info(_flag_file_end, _get_chn_val);
            if (_frame_info.has_value())
                this->frame_info_array->push_back(_frame_info.value());
            else
                LOG(WARNING) << "One extraction failed!";
            if (_flag_file_end) {
                LOG(WARNING) << "Frame array: End of file reached!";
                break;
            }
        }
    else
        for (auto i = 0; i < _n_frames; i++){
            auto _frame_info = this->extract_frame_info(_flag_file_end, _get_chn_val);
            if (_frame_info.has_value())
                this->frame_info_array->push_back(_frame_info.value());
            else
                LOG(WARNING) << "One extraction failed!";
            if (_flag_file_end) {
                LOG(WARNING) << "Frame array: End of file reached!";
                break;
            }
        }
    LOG(INFO) << "Frame info extraction finished";
    auto _frame_info_array_size = this->frame_info_array->size();
    LOG(INFO) << "Frame info array size: " << _frame_info_array_size;
    if (_frame_info_array_size == 0)
        return false;
    else
        flag_frame_info_array_valid = true;
    return true;
}

bool CAEN_data_reader::write_frame_array2root_file(){
    auto _run_number = this->get_run_number();
    std::string _root_file_name_str = DEFAULT_ROOT_FILE_FOLDER;
    _root_file_name_str = _root_file_name_str + "Run_";
    _root_file_name_str = _root_file_name_str + std::to_string(_run_number) + ".root";

    return this->write_frame_array2root_file(_root_file_name_str.c_str());
}

bool CAEN_data_reader::write_frame_array2root_file(const char *_root_file_name, std::vector<FrameInfo> *_frame_info_array_ptr){
    if (strlen(_root_file_name) == 0) {
        LOG(ERROR) << "Root file name is empty!";
        return false;
    }
    
    TFile *_root_file_ptr = new TFile(_root_file_name, "RECREATE");
    if (_root_file_ptr->IsZombie()) {
        LOG(ERROR) << "Root file cannot be opened!";
        return false;
    }

    auto _frame_info_array_size = _frame_info_array_ptr->size();
    LOG(INFO) << "Writing frame info array to root file...";

    TTree *_tree_ptr = new TTree("frames", "Frame data");

    Short_t _board_num;
    Double_t _timestamp;
    Int_t _trigID;
    std::vector<Short_t> _CH;
    std::vector<Short_t> _HG_charge;
    std::vector<Short_t> _LG_charge;
    std::vector<Short_t> _TS;
    std::vector<Short_t> _ToT;

    // ! create branches
    _tree_ptr->Branch("board_num", &_board_num);
    _tree_ptr->Branch("timestamp", &_timestamp);
    _tree_ptr->Branch("trigID",    &_trigID);
    _tree_ptr->Branch("CH",        &_CH);
    _tree_ptr->Branch("HG_charge", &_HG_charge);
    _tree_ptr->Branch("LG_charge", &_LG_charge);
    _tree_ptr->Branch("TS",        &_TS);
    _tree_ptr->Branch("ToT",       &_ToT);

    for (const auto& _frame: *_frame_info_array_ptr){
        _board_num  = _frame.nboards;
        _timestamp  = _frame.timestamp;
        _trigID     = _frame.trigID;
        _CH         = _frame.CH;
        _HG_charge  = _frame.HG_charge;
        _LG_charge  = _frame.LG_charge;
        _TS         = _frame.TS;
        _ToT        = _frame.ToT;
        _tree_ptr->Fill();
    }

    _tree_ptr->Write();
    _root_file_ptr->Close();

    LOG(INFO) << "Frame info array is written to root file!";
    return true;
}

bool CAEN_data_reader::read_root_file2frame_array(const char *_root_file_name){
    reset_frame_vector();
    TFile *_root_file_ptr = new TFile(_root_file_name, "READ");
    if (_root_file_ptr->IsZombie()) {
        LOG(ERROR) << "Root frame file cannot be opened!";
        return false;
    }

    TTree *_tree_ptr = (TTree*)_root_file_ptr->Get("frames");

    Short_t _board_num;
    Double_t _timestamp;
    Int_t _trigID;
    std::vector<Short_t> *_CH_ptr = nullptr;
    std::vector<Short_t> *_HG_charge_ptr = nullptr;
    std::vector<Short_t> *_LG_charge_ptr = nullptr;
    std::vector<Short_t> *_TS_ptr = nullptr;
    std::vector<Short_t> *_ToT_ptr = nullptr;

    _tree_ptr->SetBranchAddress("board_num", &_board_num);
    _tree_ptr->SetBranchAddress("timestamp", &_timestamp);
    _tree_ptr->SetBranchAddress("trigID",    &_trigID);
    _tree_ptr->SetBranchAddress("CH",        &_CH_ptr);
    _tree_ptr->SetBranchAddress("HG_charge", &_HG_charge_ptr);
    _tree_ptr->SetBranchAddress("LG_charge", &_LG_charge_ptr);
    _tree_ptr->SetBranchAddress("TS",        &_TS_ptr);
    _tree_ptr->SetBranchAddress("ToT",       &_ToT_ptr);

    auto _n_entries = _tree_ptr->GetEntries();
    LOG(INFO) << "Number of frames: " << _n_entries;

    for (auto i = 0; i < _n_entries; i++){
        _tree_ptr->GetEntry(i);
        FrameInfo _frame_info;
        _frame_info.nboards     = _board_num;
        _frame_info.timestamp   = _timestamp;
        _frame_info.trigID      = _trigID;
        _frame_info.CH          = *_CH_ptr;
        _frame_info.HG_charge   = *_HG_charge_ptr;
        _frame_info.LG_charge   = *_LG_charge_ptr;
        _frame_info.TS          = *_TS_ptr;
        _frame_info.ToT         = *_ToT_ptr;
        this->frame_info_array->push_back(_frame_info);
    }

    _root_file_ptr->Close();

    auto _frame_info_array_size = this->frame_info_array->size();
    LOG(INFO) << "Frame info array size: " << _frame_info_array_size;
    if (_frame_info_array_size == 0)
        return false;
    else
        flag_frame_info_array_valid = true;

    return true;
}

bool CAEN_data_reader::ian_rootfile_adapter(const char *_root_file_name){
    if (frame_info_array != nullptr)
        LOG(WARNING) << "Frame info array is not empty!";
    reset_frame_vector();
    TFile *_root_file_ptr = new TFile(_root_file_name, "READ");
    if (_root_file_ptr->IsZombie()) {
        LOG(ERROR) << "Root frame file cannot be opened!";
        return false;
    }
    TTree *_tree_ptr = (TTree*)_root_file_ptr->Get("hcaldata");

    Long64_t trigID;
    Float_t board0_timestamp;
    Float_t board1_timestamp;
    Float_t board2_timestamp;
    Float_t board3_timestamp;

    UShort_t    board0_CHID_ptr [64];
    Int_t       board0_LG_ptr   [64];
    Int_t       board0_HG_ptr   [64];
    Float_t     board0_TS_ptr   [64];
    Float_t     board0_ToT_ptr  [64];
    UShort_t    board1_CHID_ptr [64];
    Int_t       board1_LG_ptr   [64];
    Int_t       board1_HG_ptr   [64];
    Float_t     board1_TS_ptr   [64];
    Float_t     board1_ToT_ptr  [64];
    UShort_t    board2_CHID_ptr [64];
    Int_t       board2_LG_ptr   [64];
    Int_t       board2_HG_ptr   [64];
    Float_t     board2_TS_ptr   [64];
    Float_t     board2_ToT_ptr  [64];
    UShort_t    board3_CHID_ptr [64];
    Int_t       board3_LG_ptr   [64];
    Int_t       board3_HG_ptr   [64];
    Float_t     board3_TS_ptr   [64];
    Float_t     board3_ToT_ptr  [64];

    _tree_ptr->SetBranchAddress("trgid", &trigID);
    _tree_ptr->SetBranchAddress("buffer_board0_global_ts", &board0_timestamp);
    _tree_ptr->SetBranchAddress("buffer_board1_global_ts", &board1_timestamp);
    _tree_ptr->SetBranchAddress("buffer_board2_global_ts", &board2_timestamp);
    _tree_ptr->SetBranchAddress("buffer_board3_global_ts", &board3_timestamp);

    _tree_ptr->SetBranchAddress("buffer_board0_chid", &board0_CHID_ptr);
    _tree_ptr->SetBranchAddress("buffer_board0_lg",   &board0_LG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board0_hg",   &board0_HG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board0_ts",   &board0_TS_ptr);
    _tree_ptr->SetBranchAddress("buffer_board0_tot",  &board0_ToT_ptr);

    _tree_ptr->SetBranchAddress("buffer_board1_chid", &board1_CHID_ptr);
    _tree_ptr->SetBranchAddress("buffer_board1_lg",   &board1_LG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board1_hg",   &board1_HG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board1_ts",   &board1_TS_ptr);
    _tree_ptr->SetBranchAddress("buffer_board1_tot",  &board1_ToT_ptr);

    _tree_ptr->SetBranchAddress("buffer_board2_chid", &board2_CHID_ptr);
    _tree_ptr->SetBranchAddress("buffer_board2_lg",   &board2_LG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board2_hg",   &board2_HG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board2_ts",   &board2_TS_ptr);
    _tree_ptr->SetBranchAddress("buffer_board2_tot",  &board2_ToT_ptr);

    _tree_ptr->SetBranchAddress("buffer_board3_chid", &board3_CHID_ptr);
    _tree_ptr->SetBranchAddress("buffer_board3_lg",   &board3_LG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board3_hg",   &board3_HG_ptr);
    _tree_ptr->SetBranchAddress("buffer_board3_ts",   &board3_TS_ptr);
    _tree_ptr->SetBranchAddress("buffer_board3_tot",  &board3_ToT_ptr);

    auto n_entries = _tree_ptr->GetEntries();
    LOG(INFO) << "Total entries: " << n_entries;

    for (auto i = 0; i < n_entries; i++) {
        _tree_ptr->GetEntry(i);
        // LOG(INFO) << "Entry: " << i;
        FrameInfo _frame_info0, _frame_info1, _frame_info2, _frame_info3;

        std::vector<Short_t> _CHID_vec0;
        std::vector<Short_t> _LG_vec0;
        std::vector<Short_t> _HG_vec0;
        std::vector<Short_t> _TS_vec0;
        std::vector<Short_t> _ToT_vec0;

        std::vector<Short_t> _CHID_vec1;
        std::vector<Short_t> _LG_vec1;
        std::vector<Short_t> _HG_vec1;
        std::vector<Short_t> _TS_vec1;
        std::vector<Short_t> _ToT_vec1;

        std::vector<Short_t> _CHID_vec2;
        std::vector<Short_t> _LG_vec2;
        std::vector<Short_t> _HG_vec2;
        std::vector<Short_t> _TS_vec2;
        std::vector<Short_t> _ToT_vec2;

        std::vector<Short_t> _CHID_vec3;
        std::vector<Short_t> _LG_vec3;
        std::vector<Short_t> _HG_vec3;
        std::vector<Short_t> _TS_vec3;
        std::vector<Short_t> _ToT_vec3;

        // LOG(DEBUG) << "board0_timestamp: " << board0_timestamp;

        for (auto j = 0; j < 64; j++) {
            _CHID_vec0.push_back(Short_t(board0_CHID_ptr[j]));
            _LG_vec0.push_back(Short_t(board0_LG_ptr[j]));
            _HG_vec0.push_back(Short_t(board0_HG_ptr[j]));
            _TS_vec0.push_back(Short_t(board0_TS_ptr[j]));
            _ToT_vec0.push_back(Short_t(board0_ToT_ptr[j]));
        }

        for (auto j = 0; j < 64; j++) {
            _CHID_vec1.push_back(Short_t(board1_CHID_ptr[j]));
            _LG_vec1.push_back(Short_t(board1_LG_ptr[j]));
            _HG_vec1.push_back(Short_t(board1_HG_ptr[j]));
            _TS_vec1.push_back(Short_t(board1_TS_ptr[j]));
            _ToT_vec1.push_back(Short_t(board1_ToT_ptr[j]));
        }

        for (auto j = 0; j < 64; j++) {
            _CHID_vec2.push_back(Short_t(board2_CHID_ptr[j]));
            _LG_vec2.push_back(Short_t(board2_LG_ptr[j]));
            _HG_vec2.push_back(Short_t(board2_HG_ptr[j]));
            _TS_vec2.push_back(Short_t(board2_TS_ptr[j]));
            _ToT_vec2.push_back(Short_t(board2_ToT_ptr[j]));
        }

        for (auto j = 0; j < 64; j++) {
            _CHID_vec3.push_back(Short_t(board3_CHID_ptr[j]));
            _LG_vec3.push_back(Short_t(board3_LG_ptr[j]));
            _HG_vec3.push_back(Short_t(board3_HG_ptr[j]));
            _TS_vec3.push_back(Short_t(board3_TS_ptr[j]));
            _ToT_vec3.push_back(Short_t(board3_ToT_ptr[j]));
        }

        _frame_info0.trigID     = Int_t(trigID);
        _frame_info0.nboards    = 0;
        _frame_info0.timestamp  = Double_t(board0_timestamp);
        _frame_info0.CH         = _CHID_vec0;
        _frame_info0.LG_charge  = _LG_vec0;
        _frame_info0.HG_charge  = _HG_vec0;
        _frame_info0.TS         = _TS_vec0;
        _frame_info0.ToT        = _ToT_vec0;

        _frame_info1.trigID     = Int_t(trigID);
        _frame_info1.nboards    = 1;
        _frame_info1.timestamp  = Double_t(board1_timestamp);
        _frame_info1.CH         = _CHID_vec1;
        _frame_info1.LG_charge  = _LG_vec1;
        _frame_info1.HG_charge  = _HG_vec1;
        _frame_info1.TS         = _TS_vec1;
        _frame_info1.ToT        = _ToT_vec1;

        _frame_info2.trigID     = Int_t(trigID);
        _frame_info2.nboards    = 2;
        _frame_info2.timestamp  = Double_t(board2_timestamp);
        _frame_info2.CH         = _CHID_vec2;
        _frame_info2.LG_charge  = _LG_vec2;
        _frame_info2.HG_charge  = _HG_vec2;
        _frame_info2.TS         = _TS_vec2;
        _frame_info2.ToT        = _ToT_vec2;

        _frame_info3.trigID     = Int_t(trigID);
        _frame_info3.nboards    = 3;
        _frame_info3.timestamp  = Double_t(board3_timestamp);
        _frame_info3.CH         = _CHID_vec3;
        _frame_info3.LG_charge  = _LG_vec3;
        _frame_info3.HG_charge  = _HG_vec3;
        _frame_info3.TS         = _TS_vec3;
        _frame_info3.ToT        = _ToT_vec3;

        this->frame_info_array->push_back(_frame_info0);
        this->frame_info_array->push_back(_frame_info1);
        this->frame_info_array->push_back(_frame_info2);
        this->frame_info_array->push_back(_frame_info3);
    }

    _root_file_ptr->Close();

    auto _frame_info_array_size = this->frame_info_array->size();
    LOG(INFO) << "Frame info array size: " << _frame_info_array_size;
    if (_frame_info_array_size == 0)
        return false;
    else
        flag_frame_info_array_valid = true;

    return true;
}