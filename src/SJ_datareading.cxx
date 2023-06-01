#include "../include/SJ_datareading.h"

CAEN_data_reader::CAEN_data_reader():
    flag_caen_file_valid(false){
    LOG(INFO) << "CAEN reader created without file name";
}

CAEN_data_reader::CAEN_data_reader(const char *_file_name):
    flag_caen_file_valid(false){
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
    while (std::getline(*caen_file_ptr, _line) && _line_count < CAEN_FILE_HEADER_LINE_NUM){
        LOG(INFO) << "line" << _line_count << " " << _line;
        _line_count++;
    }
}

ULong64_t CAEN_data_reader::get_line_num(){
    if (!flag_caen_file_opened)
        if (!this->create_file_ptr())
            return 0;
    std::string _line;
    auto _line_count = 0;
    while (std::getline(*caen_file_ptr, _line))
        _line_count++;
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

    return std::stoi(_run_number_str);
}