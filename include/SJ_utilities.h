// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 03-06-2023
// ***********************************************************

#pragma once

#define INVALID_2D_VALUE            0
#define INVALID_1D_VALUE            -1
// #define ENABLE_WARNING

#define DEFAULT_PREFIX_TXT          "Run"
#define DEFAULT_EXTENSION_TXT       ".txt"
#define DEFAULT_PREFIX_ROOT         "Run_"
#define DEFAULT_EXTENSION_ROOT      ".root"

#define AREA_CENTRAL_MODULE         25
#define AREA_OUTER_MODULE           49

#include <vector>           // for std::vector
#include <string>           // for std::string
#include <TFile.h>          
#include <TTree.h>          // for root tree
#include <TH2D.h>           // for 2d histogram
#include "easylogging++.h"  // for logging
#include "csv.h"            // for csv file reading

namespace SJUtil{
    template <typename T>
    struct DataSet2D{
        /* coord */
        std::vector<Short_t> x_vec;
        std::vector<Short_t> y_vec;
        /* data */
        std::vector<T> value_vec;

        DataSet2D<T> operator*(int multiplier) const {
           DataSet2D<T> result = *this;
           for (auto& value : result.value_vec) {
               value *= multiplier;
           }
           return result;
        }   

        DataSet2D<T> operator*(double multiplier) const {
           DataSet2D<T> result = *this;
           for (auto& value : result.value_vec) {
               value = T(value * multiplier);
           }
           return result;
        }   
    };

    template <typename T>
    struct DataErrorSet2D{
        /* coord */
        std::vector<Short_t> x_vec;
        std::vector<Short_t> y_vec;
        /* data */
        std::vector<T> value_vec;
        /* error */
        std::vector<T> error_vec;

        DataSet2D<T> operator*(int multiplier) const {
           DataSet2D<T> result = *this;
           for (auto& value : result.value_vec) {
               value *= multiplier;
           }
           return result;
        }   

        DataSet2D<T> operator*(double multiplier) const {
           DataSet2D<T> result = *this;
           for (auto& value : result.value_vec) {
               value = T(value * multiplier);
           }
           return result;
        }   
    };


    struct PedestalInfo {
        std::vector<Short_t> board_vec;
        std::vector<Short_t> channel_vec;
        std::vector<Short_t> pedestal_HG_vec;
        std::vector<Short_t> pedestal_LG_vec;
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

    // * Read .csv file containing pedestal information
    // * Param: _file_name: csv file name
    // * Return: a struct containing pedestal information
    PedestalInfo read_pedestal_csv_file(const char* _file_name);

    // * Write .root file for saving fitting information
    // ! NOTE: this can only save parameters from gaussian2D
    bool write_fitted_data_file(const char* _file_name, const std::vector<double*>& _fitted_data);

    bool write_double_array_to_file(const char* _file_name, const std::vector<double>& _array_data);

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

    // * Generate 2-d mapping coordinates
    // * Param: _mapping_coords: a vector of vectors, they are:
    // *          1. unique channel number
    // *          2. x coordinates
    // *          3. y coordinates
    // *        _1d_values: a vector of 1-d values
    // *        _1d_errors: a vector of 1-d errors
    // * Return: a vector of vectors, they are:
    // *          1. x coordinates
    // *          2. y coordinates
    // *          3. value for the coordinates
    // *          4. error for the coordinates
    template <typename T>
    DataErrorSet2D<T> map1d_to_2d(
        const std::vector<T> &_1d_values, 
        const std::vector<T> &_1d_errors,
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
        std::vector<T> _z_error_array;

        DataErrorSet2D<T> _mapping_2d_array;

        for (auto i = 0; i < _length_values; i++) {
            auto _index = std::find(_mapping_uni_chn.begin(), _mapping_uni_chn.end(), i) -  _mapping_uni_chn.begin();
            if (_index < _length_mapping) {
                _x_coord_array.push_back(_mapping_x_coord[_index]);
                _y_coord_array.push_back(_mapping_y_coord[_index]);
                _z_coord_array.push_back(_1d_values[i]);
                _z_error_array.push_back(_1d_errors[i]);
                // LOG(DEBUG) << "Channel: " << i << " -> (" << _mapping_x_coord[_index] << ", " << _mapping_y_coord[_index] << ")";
            }
        }
        _mapping_2d_array.x_vec     = _x_coord_array;
        _mapping_2d_array.y_vec     = _y_coord_array;
        _mapping_2d_array.value_vec = _z_coord_array;
        _mapping_2d_array.error_vec = _z_error_array;
        return _mapping_2d_array;
    };

    template <typename T>
    DataErrorSet2D<T> map1d_to_2d(
        const std::vector<T> &_1d_values, 
        const std::vector<T> &_1d_errors,
        const std::vector<std::vector<Short_t>> &_mapping_coords){
        auto _uni_channel_num_array = _mapping_coords[0];
        auto _x_coord_array         = _mapping_coords[1];
        auto _y_coord_array         = _mapping_coords[2];
        return map1d_to_2d(_1d_values, _1d_errors, _uni_channel_num_array, _x_coord_array, _y_coord_array);
    };

    // * Normalize the data to the area of the central module
    template <typename T>
    DataSet2D<T> area_normalized_data(const DataSet2D<T> & _mapped_data){
        auto _central_x_min  = 34;
        auto _central_x_max  = 70;
        auto _central_y_min  = 34;
        auto _central_y_max  = 70;
        Double_t _area_ratio = double(AREA_CENTRAL_MODULE) / double(AREA_OUTER_MODULE);
        DataSet2D<T>_copy_mapped_data;
        auto _data_length = _mapped_data.value_vec.size();
        for (auto i=0; i<_data_length; i++){
            auto _x = _mapped_data.x_vec[i];
            auto _y = _mapped_data.y_vec[i];
            _copy_mapped_data.x_vec.push_back(_x);
            _copy_mapped_data.y_vec.push_back(_y);
            if (_x > _central_x_min && _x < _central_x_max && _y > _central_y_min && _y < _central_y_max)
                _copy_mapped_data.value_vec.push_back(_mapped_data.value_vec[i]);
            else
                _copy_mapped_data.value_vec.push_back(T(double(_mapped_data.value_vec[i]) * _area_ratio));
        }
        return _copy_mapped_data;
    }

    template <typename T>
    DataErrorSet2D<T> area_normalized_data(const DataErrorSet2D<T> & _mapped_data){
        auto _central_x_min  = 34;
        auto _central_x_max  = 70;
        auto _central_y_min  = 34;
        auto _central_y_max  = 70;
        Double_t _area_ratio = Double_t(AREA_CENTRAL_MODULE) / Double_t(AREA_OUTER_MODULE);
        DataErrorSet2D<T>_copy_mapped_data;
        auto _data_length = _mapped_data.value_vec.size();
        for (auto i=0; i<_data_length; i++){
            auto _x = _mapped_data.x_vec[i];
            auto _y = _mapped_data.y_vec[i];
            _copy_mapped_data.x_vec.push_back(_x);
            _copy_mapped_data.y_vec.push_back(_y);
            if (_x > _central_x_min && _x < _central_x_max && _y > _central_y_min && _y < _central_y_max) {
                _copy_mapped_data.value_vec.push_back(_mapped_data.value_vec[i]);
                _copy_mapped_data.error_vec.push_back(_mapped_data.error_vec[i]);
            }
            else {
                _copy_mapped_data.value_vec.push_back(T(double(_mapped_data.value_vec[i]) * _area_ratio));
                _copy_mapped_data.error_vec.push_back(T(double(_mapped_data.error_vec[i]) * _area_ratio));
            }
        }
        return _copy_mapped_data;
    }

    // * Get the max value and its position in the 2-d mapping coordinates
    // * This function is used to get an initial value for the fitting
    // * Param: _mapped_data: a mapped event data
    // * Return: a pointer to an array, they are:
    // *          1. x coordinates
    // *          2. y coordinates
    // *          3. value for the coordinates
    template <typename T>
    int* map_max_point_index(const DataSet2D<T> & _mapped_data){
        if (_mapped_data.value_vec.size() == 0) {
            LOG(ERROR) << "Empty mapped data";
            return nullptr;
        }
        auto _map_values = _mapped_data.value_vec;
        auto _max_value = *std::max_element(_map_values.begin(), _map_values.end());
        auto _max_index = std::find(_map_values.begin(), _map_values.end(), _max_value) - _map_values.begin();
        int* _max_index_array = new int[3];
        _max_index_array[0] = _mapped_data.x_vec[_max_index];
        _max_index_array[1] = _mapped_data.y_vec[_max_index];
        _max_index_array[2] = int(_max_value);
        return _max_index_array;
    };

    template <typename T>
    int* map_max_point_index(const DataErrorSet2D<T> & _mapped_data){
        if (_mapped_data.value_vec.size() == 0) {
            LOG(ERROR) << "Empty mapped data";
            return nullptr;
        }
        auto _map_values = _mapped_data.value_vec;
        auto _max_value = *std::max_element(_map_values.begin(), _map_values.end());
        auto _max_index = std::find(_map_values.begin(), _map_values.end(), _max_value) - _map_values.begin();
        int* _max_index_array = new int[3];
        _max_index_array[0] = _mapped_data.x_vec[_max_index];
        _max_index_array[1] = _mapped_data.y_vec[_max_index];
        _max_index_array[2] = int(_max_value);
        return _max_index_array;
    };

    // * Get a new 2-d mapping coordinates without data points under
    // * noise floor
    // * Param: _mapped_data: a mapped event data
    // *        _noise_floor: noise floor value
    // * Return: filtered 2-d mapping coordinates
    template <typename T>
    DataSet2D<T> noise_subtracted_data(const DataSet2D<T> & _mapped_data, Short_t _noise_floor){
        if (_noise_floor < 0) {
            LOG(ERROR) << "Noise floor should be positive";
            return _mapped_data;
        }
        auto _original_data_length = _mapped_data.value_vec.size();
        DataSet2D<T> _noise_subtracted_data;
        for (auto i = 0; i < _original_data_length; i++) {
            auto _noise_subtracted_value = _mapped_data.value_vec[i] - _noise_floor;
            if (_noise_subtracted_value >= 0) {
                _noise_subtracted_data.value_vec.push_back(_noise_subtracted_value);
                _noise_subtracted_data.x_vec.push_back(_mapped_data.x_vec[i]);
                _noise_subtracted_data.y_vec.push_back(_mapped_data.y_vec[i]);
            }
        }
        auto _noise_subtracted_data_length = _noise_subtracted_data.value_vec.size();
        // LOG_IF(_noise_subtracted_data_length < 10, WARNING) << "Noise subtracted data length is too small: " << _noise_subtracted_data_length;
        return _noise_subtracted_data;
    };

    template <typename T>
    DataErrorSet2D<T> noise_subtracted_data(const DataErrorSet2D<T> & _mapped_data, Short_t _noise_floor){
        if (_noise_floor < 0) {
            LOG(ERROR) << "Noise floor should be positive";
            return _mapped_data;
        }
        auto _original_data_length = _mapped_data.value_vec.size();
        DataErrorSet2D<T> _noise_subtracted_data;
        for (auto i = 0; i < _original_data_length; i++) {
            auto _noise_subtracted_value = _mapped_data.value_vec[i] - _noise_floor;
            if (_noise_subtracted_value >= 0) {
                _noise_subtracted_data.value_vec.push_back(_noise_subtracted_value);
                _noise_subtracted_data.x_vec.push_back(_mapped_data.x_vec[i]);
                _noise_subtracted_data.y_vec.push_back(_mapped_data.y_vec[i]);
                _noise_subtracted_data.error_vec.push_back(_mapped_data.error_vec[i]);
            }
        }
        auto _noise_subtracted_data_length = _noise_subtracted_data.value_vec.size();
        // LOG_IF(_noise_subtracted_data_length < 10, WARNING) << "Noise subtracted data length is too small: " << _noise_subtracted_data_length;
        return _noise_subtracted_data;
    };

    template <typename T>
    DataErrorSet2D<T> geo_cutout_data(const DataErrorSet2D<T> & _mapped_data, Double_t _x_min, Double_t _x_max, Double_t _y_min, Double_t _y_max){
        auto _original_data_length = _mapped_data.value_vec.size();
        DataErrorSet2D<T> _cutted_data;

        for (auto i = 0; i < _original_data_length; i++) {
            auto _original_x = _mapped_data.x_vec[i];
            auto _original_y = _mapped_data.y_vec[i];
            if (_original_x >= _x_min && _original_x <= _x_max && _original_y >= _y_min && _original_y <= _y_max) {
                _cutted_data.value_vec.push_back(_mapped_data.value_vec[i]);
                _cutted_data.x_vec.push_back(_original_x);
                _cutted_data.y_vec.push_back(_original_y);
                _cutted_data.error_vec.push_back(_mapped_data.error_vec[i]);
            }
        }
        auto _cutted_data_length = _cutted_data.value_vec.size();
        // LOG_IF(_cutted_data_length < 10, WARNING) << "Cutted data length is too small: " << _cutted_data_length;

        return _cutted_data;
    };

    template <typename T>
    DataSet2D<T> substitued_data(
        const DataSet2D<T> & _hg_mapped_data,
        const DataSet2D<T> & _lg_mapped_data,
        T _substitute_threshold,
        T _gain_ratio){
        auto _copy_hg_mapped_data = _hg_mapped_data;
        auto _hg_mapped_data_length = _copy_hg_mapped_data.value_vec.size();
        for (auto i=0; i<_hg_mapped_data_length; i++){
            if (_copy_hg_mapped_data.value_vec[i] > _substitute_threshold){
                auto _target_x = _copy_hg_mapped_data.x_vec[i];
                auto _target_y = _copy_hg_mapped_data.y_vec[i];
                // * Find the corresponding low gain data
                auto _lg_mapped_data_length = _lg_mapped_data.value_vec.size();
                bool _is_found = false;
                for (auto j=0; j<_lg_mapped_data_length; j++){
                    if (_lg_mapped_data.x_vec[j] == _target_x && _lg_mapped_data.y_vec[j] == _target_y){
                        // LOG(DEBUG) << "Substitute (" << _target_x << ", " << _target_y << ") from "<< _copy_hg_mapped_data.value_vec[i] << " to " << _lg_mapped_data.value_vec[j] * _gain_ratio;
                        _is_found = true;
                        _copy_hg_mapped_data.value_vec[i] = _lg_mapped_data.value_vec[j] * _gain_ratio;
                        break;
                    }
                }
                if (!_is_found){
                    // LOG(WARNING) << "Cannot find the corresponding low gain data for (" << _target_x << ", " << _target_y << ")";
                }
            }
        }
        return _copy_hg_mapped_data;
    }

    std::vector<std::vector<Double_t>> read_relative_gain_root_file(const char* _file_name);

    template <typename T>
    std::vector<Double_t> gain_multiplication(
        const std::vector<Double_t> & _gain_vec,
        const std::vector<Double_t> & _offset_vec,
        const std::vector<T> & _data_vec){
        std::vector<Double_t> _multiplied_data_vec;
        auto _gain_vec_length = _gain_vec.size();
        auto _offset_vec_length = _offset_vec.size();
        auto _multiplication_vec_length = _data_vec.size();
        if (_gain_vec_length != _multiplication_vec_length){
            LOG(ERROR) << "Gain vector length (" << _gain_vec_length << ") is not equal to multiplication vector length (" << _multiplication_vec_length << ")";
            return _multiplied_data_vec;
        }
        if (_offset_vec_length != _multiplication_vec_length){
            LOG(ERROR) << "Offset vector length (" << _offset_vec_length << ") is not equal to multiplication vector length (" << _multiplication_vec_length << ")";
            return _multiplied_data_vec;
        }
        for (auto i=0; i<_multiplication_vec_length; i++){
            _multiplied_data_vec.push_back(_gain_vec[i] * Double_t(_data_vec[i]) + _offset_vec[i]);
        }
        return _multiplied_data_vec;
    };

    std::vector<Double_t> gain_error_multiplication(
        const std::vector<Double_t> & _gain_vec,
        const std::vector<Double_t> & _offset_vec,
        Double_t _data_vec);

    TH2D* get_2d_histogram(
        const std::vector<Short_t>  &_x_vec,
        const std::vector<Short_t>  &_y_vec,
        const std::vector<Double_t> &_value_vec,
        const char* _hist_name,
        const char* _hist_title
    );

    template <typename T>
    SJUtil::DataErrorSet2D<T> add_error(const SJUtil::DataSet2D<T> &_data_wo_error ,T _error_val){
        SJUtil::DataErrorSet2D<T> _data_w_error;
        auto _data_length = _data_wo_error.value_vec.size();
        for (auto i=0; i<_data_length; i++){
            _data_w_error.value_vec.push_back(_data_wo_error.value_vec[i]);
            _data_w_error.x_vec.push_back(_data_wo_error.x_vec[i]);
            _data_w_error.y_vec.push_back(_data_wo_error.y_vec[i]);
            _data_w_error.error_vec.push_back(_error_val);
        }
        return _data_w_error;
    };

    template <typename T>
    SJUtil::DataErrorSet2D<T> substitued_data_error(
        const DataErrorSet2D<T> & _hg_mapped_data_err,
        const DataErrorSet2D<T> & _lg_mapped_data_err,
        T _substitute_threshold,
        T _gain_ratio){
        auto _copy_hg_mapped_data = _hg_mapped_data_err;
        auto _hg_mapped_data_length = _copy_hg_mapped_data.value_vec.size();
        for (auto i=0; i<_hg_mapped_data_length; i++){
            if (_copy_hg_mapped_data.value_vec[i] > _substitute_threshold){
                auto _target_x = _copy_hg_mapped_data.x_vec[i];
                auto _target_y = _copy_hg_mapped_data.y_vec[i];
                // * Find the corresponding low gain data
                auto _lg_mapped_data_length = _lg_mapped_data_err.value_vec.size();
                bool _is_found = false;
                for (auto j=0; j<_lg_mapped_data_length; j++){
                    if (_lg_mapped_data_err.x_vec[j] == _target_x && _lg_mapped_data_err.y_vec[j] == _target_y){
                        _is_found = true;
                        _copy_hg_mapped_data.value_vec[i] = _lg_mapped_data_err.value_vec[j] * _gain_ratio;
                        _copy_hg_mapped_data.error_vec[i] = _lg_mapped_data_err.error_vec[j] * _gain_ratio;
                        break;
                    }
                }
                if (!_is_found){
                    // LOG(WARNING) << "Cannot find the corresponding low gain data for (" << _target_x << ", " << _target_y << ")";
                }
            }
        }
        return _copy_hg_mapped_data;
    };

    template <typename T>
    std::vector<Double_t> get_CoM(const DataErrorSet2D<T> &_data){
        std::vector<Double_t> _CoM;
        auto _data_length = _data.value_vec.size();
        Double_t _sum_x = 0;
        Double_t _sum_y = 0;
        Double_t _sum_w = 0;
        for (auto i=0; i<_data_length; i++){
            auto _x = Double_t(_data.x_vec[i]);
            auto _y = Double_t(_data.y_vec[i]);
            auto _w = Double_t(_data.value_vec[i]);
            _sum_x += _x * _w;
            _sum_y += _y * _w;
            _sum_w += _w;
        }
        _CoM.push_back(Double_t(_sum_x / _sum_w));
        _CoM.push_back(Double_t(_sum_y / _sum_w));
        return _CoM;
    };

    template <typename T>
    bool geo_event_is_cut(const DataErrorSet2D<T> &_data, Double_t _x_min, Double_t _x_max, Double_t _y_min, Double_t _y_max){
        auto _event_CoM = get_CoM(_data);
        if (_event_CoM[0] >= _x_min && _event_CoM[0] <= _x_max && _event_CoM[1] >= _y_min && _event_CoM[1] <= _y_max){
            return false;
        }
        return true;
    };
}