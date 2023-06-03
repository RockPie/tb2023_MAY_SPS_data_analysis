#include "SJ_utilities.h"

std::vector<std::vector<Short_t>> SJUtil::read_mapping_csv_file(const char* _file_name) {
    LOG(INFO) << "Reading mapping file: " << _file_name;
    if (!std::filesystem::exists(_file_name)) {
        LOG(ERROR) << "File not found: " << _file_name;
        return std::vector<std::vector<Short_t>>();
    }

    std::vector<std::vector<Short_t>> _mapping_array_res;
    io::CSVReader <5> in(_file_name);
    in.read_header(io::ignore_extra_column, "BoardNum", "ChannelNum", "ModuleNum", "Col", "Row");
    Short_t _board_num, _channel_num, _module_num, _col, _row;
    std::vector<Short_t> _board_num_array, _channel_num_array, _module_num_array, _col_array, _row_array;
    while (in.read_row(_board_num, _channel_num, _module_num, _col, _row)) {
        _board_num_array.push_back(_board_num);
        _channel_num_array.push_back(_channel_num);
        _module_num_array.push_back(_module_num);
        _col_array.push_back(_col);
        _row_array.push_back(_row);
    }
    _mapping_array_res.push_back(_board_num_array);
    _mapping_array_res.push_back(_channel_num_array);
    _mapping_array_res.push_back(_module_num_array);
    _mapping_array_res.push_back(_col_array);
    _mapping_array_res.push_back(_row_array);
    return _mapping_array_res;
}