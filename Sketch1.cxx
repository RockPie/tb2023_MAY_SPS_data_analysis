#include "easylogging++.h"
#include "SJ_datareading.h"

void set_easylogger(); // set easylogging++ configurations

int main(int argc, char* argv[]){
    // --------------------- Initialize easylogging++ -----------------------
    START_EASYLOGGINGPP(argc, argv);
    set_easylogger();
    // ---------------------------------------------------------------------
    // CAEN_data_reader *reader = new CAEN_data_reader("../dataFiles/Run2819_list.txt");
    // reader->print_caen_file_header();
    // reader->extract_frame_info_array(INFINITE_FRAMES, true);
    // std::vector<CAEN_data_reader::FrameInfo> * array_ptr = reader->get_frame_info_array_ptr();
    // LOG(INFO) << "Finished";
    // reader->write_frame_array2root_file();
    // delete reader;

    CAEN_data_reader *reader = new CAEN_data_reader();
    reader->read_root_file2frame_array("../cachedFiles/Run_2819.root");
    LOG(INFO) << "Finished";
    delete reader;
    return 0;
}

void set_easylogger(){
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "%datetime{%H:%m:%s}[%levshort] (%fbase) %msg");
    defaultConf.set(el::Level::Info,    el::ConfigurationType::Format, 
        "%datetime{%H:%m:%s}[\033[1;34m%levshort\033[0m] (%fbase) %msg");
    defaultConf.set(el::Level::Warning, el::ConfigurationType::Format, 
        "%datetime{%H:%m:%s}[\033[1;33m%levshort\033[0m] (%fbase) %msg");
    defaultConf.set(el::Level::Error,   el::ConfigurationType::Format, 
        "%datetime{%H:%m:%s}[\033[1;31m%levshort\033[0m] (%fbase) %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
}