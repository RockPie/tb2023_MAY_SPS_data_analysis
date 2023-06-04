// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 03-06-2023
// ***********************************************************

#pragma once

#define INVALID_2D_VALUE            -1

#include <vector>           // for std::vector
#include <TTree.h>          // for root tree
#include "easylogging++.h"  // for logging
#include "csv.h"            // for csv file reading


namespace SJUtil{
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
    std::vector<std::vector<Short_t>> generate_mapping_croodinate(const std::vector<std::vector<Short_t>> &_mapping);
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
    std::vector<std::vector<Short_t>> map1d_to_2d(const std::vector<Short_t> &_1d_values, 
        const std::vector<std::vector<Short_t>> &_mapping_coords);
    std::vector<std::vector<Short_t>> map1d_to_2d(const std::vector<Short_t> &_1d_values, 
        const std::vector<Short_t> &_mapping_uni_chn, 
        const std::vector<Short_t> &_mapping_x_coord, 
        const std::vector<Short_t> &_mapping_y_coord);
}