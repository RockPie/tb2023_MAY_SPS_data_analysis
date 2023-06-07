#include "SJ_utilities.h"

char* SJUtil::create_filename(const char* _folder_path,  const char* _prefix, int _run_num, const char* _suffix, const char* _type){
    char* _file_name = new char[100];
    snprintf(_file_name, 100, "%s/%s%d%s%s", _folder_path, _prefix, _run_num, _suffix, _type);
    return _file_name;
}

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

SJUtil::PedestalInfo SJUtil::read_pedestal_csv_file(const char* _file_name){
    LOG(INFO) << "Reading pedestal info file: " << _file_name;
    SJUtil::PedestalInfo _pedestal_info;
    if (!std::filesystem::exists(_file_name)) {
        LOG(ERROR) << "File not found: " << _file_name;
        return _pedestal_info;
    }
    io::CSVReader <4> in(_file_name);
    in.read_header(io::ignore_extra_column, "BoardNum", "ChnNum", "HG_Pedestal", "LG_Pedestal");
    Short_t _board_num, _channel_num, _hg_pedestal, _lg_pedestal;
    while (in.read_row(_board_num, _channel_num, _hg_pedestal, _lg_pedestal)) {
        _pedestal_info.board_vec.push_back(_board_num);
        _pedestal_info.channel_vec.push_back(_channel_num);
        _pedestal_info.pedestal_HG_vec.push_back(_hg_pedestal);
        _pedestal_info.pedestal_LG_vec.push_back(_lg_pedestal);
    }
    return _pedestal_info;
}

std::vector<std::vector<Short_t>> SJUtil::generate_mapping_croodinate(const std::vector<std::vector<Short_t>> &_mapping) {
    auto _board_num_array   = _mapping[0];
    auto _channel_num_array = _mapping[1];
    auto _module_num_array  = _mapping[2];
    auto _col_array         = _mapping[3];
    auto _row_array         = _mapping[4];
    return generate_mapping_croodinate(_board_num_array, _channel_num_array, _module_num_array, _col_array, _row_array);
}

// * Assuming the central module is 7x7 of 5x5 pads
// * Others are 5x5 of 7x7 pads
std::vector<std::vector<Short_t>> SJUtil::generate_mapping_croodinate(
    const std::vector<Short_t> &_mapping_board_num_array,
    const std::vector<Short_t> &_mapping_channel_num_array,
    const std::vector<Short_t> &_mapping_module_num_array,
    const std::vector<Short_t> &_mapping_col_array,
    const std::vector<Short_t> &_mapping_row_array
){
    auto _array_size = _mapping_board_num_array.size();
    std::vector<Short_t> _uni_channel_num_array;
    std::vector<Short_t> _x_coord_array;
    std::vector<Short_t> _y_coord_array;
    std::vector<std::vector<Short_t>> _mapping_croodinate_array;

    // * Step 1. generate one-dimensional channel num
    for (auto i = 0; i < _array_size; i++)
        _uni_channel_num_array.push_back(_mapping_board_num_array[i] * 64 + _mapping_channel_num_array[i]);

    // * Step 2. generate x and y coordinate
    for (auto i = 0; i < _array_size; i++) {
        Short_t _x_base = 0;
        Short_t _y_base = 0;
        switch (_mapping_module_num_array[i])
        {
        case 0:{
            _x_base = 3;
            _y_base = 101;
            break;
        }
        case 1:{
            _x_base = 38;
            _y_base = 101;
            break;
        }
        case 2:{
            _x_base = 73;
            _y_base = 101;
            break;
        }
        case 3:{
            _x_base = 3;
            _y_base = 66;
            break;
        }
        case 4:{
            _x_base = 38;
            _y_base = 66;
            break;
        }
        case 5:{
            _x_base = 73;
            _y_base = 66;
            break;
        }
        case 6:{
            _x_base = 3;
            _y_base = 31;
            break;
        }
        case 7:{
            _x_base = 38;
            _y_base = 31;
            break;
        }
        case 8:{
            _x_base = 73;
            _y_base = 31;
            break;
        }
        default:
            LOG(ERROR) << "Mapping module number error: " << _mapping_module_num_array[i];
            break;
        }
        // * Central module
        if (_mapping_module_num_array[i] == 4){
            _x_coord_array.push_back(_x_base + _mapping_col_array[i] * 5);
            _y_coord_array.push_back(_y_base - _mapping_row_array[i] * 5);
        }
        // * Other modules
        else {
            _x_coord_array.push_back(_x_base + _mapping_col_array[i] * 7);
            _y_coord_array.push_back(_y_base - _mapping_row_array[i] * 7);
        }
    }
    _mapping_croodinate_array.push_back(_uni_channel_num_array);
    _mapping_croodinate_array.push_back(_x_coord_array);
    _mapping_croodinate_array.push_back(_y_coord_array);
    return _mapping_croodinate_array;
}


