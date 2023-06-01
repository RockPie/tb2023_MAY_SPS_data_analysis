// *****************************************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 01-06-2023
// *****************************************************************************


#ifndef SJ_DATAREADING_H
#define SJ_DATAREADING_H

#define CAEN_FILE_HEADER_LINE_NUM 6

#include <iostream>
#include <fstream>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

class CAEN_data_reader {
public: 
    CAEN_data_reader();
    CAEN_data_reader(const char *_file_name);
    ~CAEN_data_reader();
public:
    bool set_caen_file(const char *_file_name);
    void print_caen_file_header();
private:
    bool create_file_ptr();
private:
    std::string  *caen_file_name;
    std::fstream *caen_file_ptr;
    bool  flag_caen_file_opened;
    bool  flag_caen_file_valid;
};

#endif // SJ_DATAREADING_H