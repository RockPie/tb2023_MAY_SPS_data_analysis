// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 03-06-2023
// ***********************************************************

#pragma once

#define INVALID_2D_VALUE            -1
// #define ENABLE_WARNING

#define DEFAULT_PREFIX_TXT          "Run"
#define DEFAULT_EXTENSION_TXT       ".txt"
#define DEFAULT_PREFIX_ROOT         "Run_"
#define DEFAULT_EXTENSION_ROOT      ".root"

#include <vector>           // for std::vector
#include <string>           // for std::string
#include <TTree.h>          // for root tree
#include "easylogging++.h"  // for logging
#include "csv.h"            // for csv file reading



namespace SJUtil{
    template <typename T>
    struct DataSet2D
    {
        /* coord */
        std::vector<Short_t> x_vec;
        std::vector<Short_t> y_vec;
        /* data */
        std::vector<T> value_vec;
    };
    

    // * Create a file name 
    // * Param: _folder_path: folder path
    // *        _prefix: file name prefix
    // *        _run_num: run number
    // *        _suffix: file name suffix
    // *        _type: file type
    // * Return: a char pointer to the file name
    char* create_filename(const char* _folder_path,  const char* _prefix, int _run_num, const char* _suffix, const char* _type);
    inline char* create_filename_CAEN(const char* _folder_path, int _run_num){
        return create_filename(_folder_path, DEFAULT_PREFIX_TXT, _run_num, "_list", DEFAULT_EXTENSION_TXT);
    }
    inline char* create_filename_frames(const char* _folder_path, int _run_num) {
        return create_filename(_folder_path, DEFAULT_PREFIX_ROOT, _run_num, "_frames", DEFAULT_EXTENSION_ROOT);
    }
    inline char* create_filename_events(const char* _folder_path, int _run_num){
        return create_filename(_folder_path, DEFAULT_PREFIX_ROOT, _run_num, "_events", DEFAULT_EXTENSION_ROOT);
    }
    inline char* create_filename_results(const char* _folder_path, int _run_num){
        return create_filename(_folder_path, DEFAULT_PREFIX_ROOT, _run_num, "_results", DEFAULT_EXTENSION_ROOT);
    }

    // * Read mapping csv file
    // * Param: _file_name: csv file name
    // * Return: a vector of vectors, each vector contains a column of the csv file
    std::vector<std::vector<Short_t>> read_mapping_csv_file(const char* _file_name);

    // * Generate mapping coordinates
    // * Param: _mapping: a vector of vectors, each vector contains a column of the csv file
    // * Return: a vector of vectors, they are:
    // *          1. unique channel number
    // *          2. x coordinates
    // *          3. y coordinates
    std::vector<std::vector<Short_t>> generate_mapping_croodinate(
        const std::vector<std::vector<Short_t>> &_mapping);
    std::vector<std::vector<Short_t>> generate_mapping_croodinate(
        const std::vector<Short_t> &_mapping_board_num_array,
        const std::vector<Short_t> &_mapping_channel_num_array,
        const std::vector<Short_t> &_mapping_module_num_array,
        const std::vector<Short_t> &_mapping_col_array,
        const std::vector<Short_t> &_mapping_row_array
    );

    // * Generate 2-d mapping coordinates
    // * Param: _mapping_coords: a vector of vectors, they are:
    // *          1. unique channel number
    // *          2. x coordinates
    // *          3. y coordinates
    // *        _1d_values: a vector of 1-d values
    // * Return: a vector of vectors, they are:
    // *          1. x coordinates
    // *          2. y coordinates
    // *          3. value for the coordinates
    // std::vector<std::vector<Short_t>> map1d_to_2d(const std::vector<Short_t> &_1d_values, 
    //     const std::vector<std::vector<Short_t>> &_mapping_coords);
    // std::vector<std::vector<Short_t>> map1d_to_2d(const std::vector<Short_t> &_1d_values, 
    //     const std::vector<Short_t> &_mapping_uni_chn, 
    //     const std::vector<Short_t> &_mapping_x_coord, 
    //     const std::vector<Short_t> &_mapping_y_coord);



    template <typename T>
    DataSet2D<T> map1d_to_2d(
        const std::vector<T> &_1d_values, 
        const std::vector<Short_t> &_mapping_uni_chn, 
        const std::vector<Short_t> &_mapping_x_coord, 
        const std::vector<Short_t> &_mapping_y_coord){
        auto _length_values = _1d_values.size();
        auto _length_mapping = _mapping_uni_chn.size();
        #ifdef ENABLE_WARNING
            LOG_IF(_length_values != _length_mapping, WARNING) << "Mapping array size not match: "  << _length_values << " vs " << _length_mapping;
        #endif
        std::vector<Short_t> _x_coord_array;
        std::vector<Short_t> _y_coord_array;
        std::vector<T> _z_coord_array;

        DataSet2D<T> _mapping_2d_array;

        for (auto i = 0; i < _length_values; i++) {
            auto _index = std::find(_mapping_uni_chn.begin(), _mapping_uni_chn.end(), i) -  _mapping_uni_chn.begin();
            if (_index < _length_mapping) {
                _x_coord_array.push_back(_mapping_x_coord[_index]);
                _y_coord_array.push_back(_mapping_y_coord[_index]);
                _z_coord_array.push_back(_1d_values[i]);
                // LOG(DEBUG) << "Channel: " << i << " -> (" << _mapping_x_coord[_index] << ", " << _mapping_y_coord[_index] << ")";
            }
            else {
                #ifdef ENABLE_WARNING
                    LOG(WARNING) << "Cannot find channel: " << i;
                #endif
                //_x_coord_array.push_back(INVALID_2D_VALUE);
                //_y_coord_array.push_back(INVALID_2D_VALUE);
                //_z_coord_array.push_back(T(INVALID_2D_VALUE));
            }
        }
        _mapping_2d_array.x_vec     = _x_coord_array;
        _mapping_2d_array.y_vec     = _y_coord_array;
        _mapping_2d_array.value_vec = _z_coord_array;
        return _mapping_2d_array;
    };

    template <typename T>
    DataSet2D<T> map1d_to_2d(
        const std::vector<T> &_1d_values, 
        const std::vector<std::vector<Short_t>> &_mapping_coords){
        auto _uni_channel_num_array = _mapping_coords[0];
        auto _x_coord_array         = _mapping_coords[1];
        auto _y_coord_array         = _mapping_coords[2];
        return map1d_to_2d(_1d_values, _uni_channel_num_array, _x_coord_array, _y_coord_array);
    };
}