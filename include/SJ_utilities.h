// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 01-06-2023
// ***********************************************************

#pragma once

#include <vector>           // for std::vector
#include <TTree.h>          // for root tree
#include "easylogging++.h"  // for logging
#include "csv.h"            // for csv file reading

namespace SJUtil{
    // * Read mapping csv file
    // * Param: _file_name: csv file name
    // * Return: a vector of vectors, each vector contains a column of the csv file
    std::vector<std::vector<Short_t>> read_mapping_csv_file(const char* _file_name);
}