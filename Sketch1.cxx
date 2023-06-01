#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "easylogging++.h"
#include "SJ_datareading.h"

int main(int argc, char* argv[]){
    // --------------------- Initialize easylogging++ -----------------------
    START_EASYLOGGINGPP(argc, argv);
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%H:%m:%s} [%level] (%fbase) %msg");
    defaultConf.set(el::Level::Info,    el::ConfigurationType::Format, "%datetime{%H:%m:%s} [\033[1;34m%level\033[0m] (%fbase) %msg");
    defaultConf.set(el::Level::Warning, el::ConfigurationType::Format, "%datetime{%H:%m:%s} [\033[1;33m%level\033[0m] (%fbase) %msg");
    defaultConf.set(el::Level::Error,   el::ConfigurationType::Format, "%datetime{%H:%m:%s} [\033[1;31m%level\033[0m] (%fbase) %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
    // ---------------------------------------------------------------------
    CAEN_data_reader *reader = new CAEN_data_reader("../dataFiles/Run2806_list.txt");
    // CAEN_data_reader *reader = new CAEN_data_reader();
    // reader->set_caen_file("../dataFiles/Run2806_list.txt");
    reader->print_caen_file_header();
    // LOG(INFO) << "Line number: " << reader->get_line_num();
    delete reader;
    return 0;
}