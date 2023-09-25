#include "SJ_cluster.h"

cell_CA::cell_CA(){
    x = 0;
    y = 0;
    size = 0;
    adc = 0;
    is_local_maximum = false;
    tags.clear();
    tags_buffer.clear();
}

cell_CA::cell_CA(Double_t x, Double_t y, Double_t size, Double_t adc){
    this->x = x;
    this->y = y;
    this->size = size;
    this->adc = adc;
    is_local_maximum = false;
    tags.clear();
    tags_buffer.clear();
}

cell_CA::~cell_CA(){
    tags.clear();
    tags_buffer.clear();
}

void cell_CA::update(void){
    if (this->is_tagged())
        return;
    if (tags_buffer.size() == 0)
        return;
    tags.clear();
    tags = tags_buffer;
    // keep unique tags
    std::sort(tags.begin(), tags.end());
    tags.erase(std::unique(tags.begin(), tags.end()), tags.end());
    tags_buffer.clear();
    // LOG(DEBUG) << "new tags size: " << tags.size();
}

void cell_CA::set_local_maximum(Int_t _tag_id){
    is_local_maximum = true;
    tags_buffer.push_back(_tag_id);
}

void cell_CA::add_tag(Int_t _tag_id){
    // LOG_IF(is_local_maximum, WARNING) << "This cell is already a local maximum.";
    LOG_IF(tags.size() > 0, WARNING) << "This cell already has tags.";
    tags_buffer.push_back(_tag_id);
}

Double_t cell_CA::get_distance(cell_CA _cell){
    return sqrt(pow(x - _cell.x, 2) + pow(y - _cell.y, 2));
}

Double_t cell_CA::get_distance_squared(cell_CA _cell){
    return pow(x - _cell.x, 2) + pow(y - _cell.y, 2);
}


cluster_CA::cluster_CA():
    local_max_threshold(0){
    tag_id_array.clear();
    cells = new std::vector<cell_CA*>;
    for (auto x_index = 0; x_index < 15; x_index++){
        for (auto y_index = 0; y_index < 15; y_index++){
            if (x_index > 4 && x_index < 10 && y_index > 4 && y_index < 10)
                continue;
            cells->push_back(new cell_CA(
                Double_t(x_index * SIZE_OUTER + SIZE_OUTER / 2),
                Double_t(y_index * SIZE_OUTER + SIZE_OUTER / 2),
                Double_t(SIZE_OUTER),
                Double_t(0)
            ));
        }
    }
    for (auto x_index = 7; x_index < 14; x_index++){
        for (auto y_index = 7; y_index < 14; y_index++){
            cells->push_back(new cell_CA(
                Double_t(x_index * SIZE_CENTER + SIZE_CENTER / 2),
                Double_t(y_index * SIZE_CENTER + SIZE_CENTER / 2),
                Double_t(SIZE_CENTER),
                Double_t(0)
            ));
        }
    }

    // LOG(DEBUG) << "Total number of cells: " << cells->size();
}

cluster_CA::~cluster_CA(){
    for (auto i = 0; i < cells->size(); i++){
        delete cells->at(i);
    }
    cells->clear();
    delete cells;
    tag_id_array.clear();
}

void cluster_CA::assign_adc_to_cells(SJUtil::DataErrorSet2D<double> *adc_map){
    auto adc_map_size = adc_map->value_vec.size();
    for (auto i = 0; i < adc_map_size; i++){
        auto x_index = Double_t(adc_map->x_vec.at(i));
        auto y_index = Double_t(adc_map->y_vec.at(i));
        bool is_in_range = false;
        for (auto j = 0; j < cells->size(); j++){
            auto cell = cells->at(j);
            if (abs(cell->x - x_index) < 0.01 && abs(cell->y - y_index) < 0.01){
                
                cell->adc = adc_map->value_vec.at(i);
                is_in_range = true;
                break;
            }
        }
        if (!is_in_range){
            LOG(WARNING) << "ADC map is out of range.";
            LOG(WARNING) << "x: " << x_index << ", y: " << y_index;
        }
    }
}

bool cluster_CA::find_local_maximum(void){
    bool is_found = false;
    if (this->local_max_threshold == 0){
        LOG(ERROR) << "Local maximum threshold is not set.";
        return is_found;
    }
    for (auto i = 0; i < cells->size(); i++){
        auto cell = cells->at(i);
        if (cell->adc < local_max_threshold)
            continue;
        bool _is_local_maximum = true;
        Double_t _neighbor_sum_adc = 0;
        for (auto j = 0; j < cells->size(); j++){
            if (i == j)
                continue;
            auto cell2 = cells->at(j);
            if (cell->get_distance_squared(*cell2) < 99){
                if (cell->adc < cell2->adc){
                    _is_local_maximum = false;
                    break;
                }
                _neighbor_sum_adc += cell2->adc;
                // if (cell2->adc < local_max_threshold * 0.5){
                //     _is_local_maximum = false;
                //     break;
                // }
            }
        }
        if (_neighbor_sum_adc < local_max_threshold * 4)
            _is_local_maximum = false;
        if (_is_local_maximum){
            tag_id_array.push_back(i);
            cell->set_local_maximum(i);
            is_found = true;
            // LOG(DEBUG) << "Local maximum found at (" << cell->x << ", " << cell->y << ").";
        }
    }
    if (is_found)
        cells_update();
    return is_found;
}

bool cluster_CA::iterate(void){
    bool _all_tagged = true;

    for (auto i = 0; i < cells->size(); i++){
        auto cell = cells->at(i);
        if (cell->is_tagged())
            continue;
        else {
            _all_tagged = false;
            // LOG(DEBUG) << "Cell (" << cell->x << ", " << cell->y << ") is not tagged.";
        }
        for (auto j = 0; j < cells->size(); j++){
            if (i == j)
                continue;
            auto cell2 = cells->at(j);
            if (cell->get_distance_squared(*cell2) < 99){
                if (cell2->is_tagged()){
                    // LOG(DEBUG) << "Tagging cell (" << cell->x << ", " << cell->y << ") with tag " << cell2->tags.at(0) << ".";
                    for (auto k = 0; k < cell2->tags.size(); k++){
                        cell->add_tag(cell2->tags.at(k));
                    }
                }
            }   
        }
    }
    if (!_all_tagged)
        cells_update();
    return _all_tagged;
}

void cluster_CA::cells_update(void){
    for (auto i = 0; i < cells->size(); i++)
        cells->at(i)->update();
}

bool cluster_CA::save_event_to_csv(SJUtil::DataErrorSet2D<double> *adc_map, std::string filename) {
    if (adc_map->value_vec.size() == 0){
        LOG(ERROR) << "ADC map is empty.";
        return false;
    }
    if (tag_id_array.size() == 0){
        LOG(WARNING) << "No local maximum found.";
    }
    std::ofstream csv_file;
    csv_file.open(filename);
    csv_file << "x,y,adc,tag_id" << std::endl;
    for (auto i = 0; i < adc_map->value_vec.size(); i++){
        auto x_index = Double_t(adc_map->x_vec.at(i));
        auto y_index = Double_t(adc_map->y_vec.at(i));
        for (auto j = 0; j < cells->size(); j++){
            auto cell = cells->at(j);
            if (abs(cell->x - x_index) < 0.01 && abs(cell->y - y_index) < 0.01){
                csv_file << cell->x << "," << cell->y << "," << cell->adc << ",";
                if (cell->is_tagged()){
                    csv_file << cell->tags.at(0);
                }
                else{
                    csv_file << "-1";
                }
                csv_file << std::endl;
                break;
            }
        }
    }
    csv_file.close();
    return true;
}

cell_MA::cell_MA(){
    x = 0;
    y = 0;
    size = 0;
    adc = 0;
    is_maxima = false;
    has_propagated = false;
    tags.clear();
}

cell_MA::cell_MA(Double_t x, Double_t y, Double_t size, Double_t adc){
    this->x = x;
    this->y = y;
    this->size = size;
    this->adc = adc;
    is_maxima = false;
    has_propagated = false;
    tags.clear();
}

cell_MA::~cell_MA(){
    tags.clear();
}

void cell_MA::set_maxima(Int_t _tag_id){
    is_maxima = true;
    tags.push_back(_tag_id);
}

void cell_MA::add_tag(Int_t _tag_id){
    tags_buffer.push_back(_tag_id);
}

Double_t cell_MA::get_distance(cell_MA _cell){
    return sqrt(pow(x - _cell.x, 2) + pow(y - _cell.y, 2));
}

Double_t cell_MA::get_distance_squared(cell_MA _cell){
    return pow(x - _cell.x, 2) + pow(y - _cell.y, 2);
}

void cell_MA::update(){
    if (this->is_tagged())
        return;
    if (tags_buffer.size() == 0)
        return;
    tags.clear();
    tags = tags_buffer;
    // keep unique tags
    std::sort(tags.begin(), tags.end());
    tags.erase(std::unique(tags.begin(), tags.end()), tags.end());
    tags_buffer.clear();
    // LOG(DEBUG) << "new tags size: " << tags.size();
}

cluster_MA::cluster_MA():
    neighbor_distance(99),
    threshold_seed(0),
    threshold_agg(0),
    threshold_larger_neighbor(0){
    cells = new std::vector<cell_MA*>;
    tag_id_array.clear();
    for (auto x_index = 0; x_index < 15; x_index++){
        for (auto y_index = 0; y_index < 15; y_index++){
            if (x_index > 4 && x_index < 10 && y_index > 4 && y_index < 10)
                continue;
            cells->push_back(new cell_MA(
                Double_t(x_index * SIZE_OUTER + SIZE_OUTER / 2),
                Double_t(y_index * SIZE_OUTER + SIZE_OUTER / 2),
                Double_t(SIZE_OUTER),
                Double_t(0)
            ));
        }
    }
    for (auto x_index = 7; x_index < 14; x_index++){
        for (auto y_index = 7; y_index < 14; y_index++){
            cells->push_back(new cell_MA(
                Double_t(x_index * SIZE_CENTER + SIZE_CENTER / 2),
                Double_t(y_index * SIZE_CENTER + SIZE_CENTER / 2),
                Double_t(SIZE_CENTER),
                Double_t(0)
            ));
        }
    }
}

cluster_MA::~cluster_MA(){
    for (auto i = 0; i < cells->size(); i++){
        delete cells->at(i);
    }
    cells->clear();
    delete cells;
    tag_id_array.clear();
}

bool cluster_MA::save_event_to_csv(SJUtil::DataErrorSet2D<double> *adc_map, std::string filename){
    if (adc_map->value_vec.size() == 0){
        LOG(ERROR) << "ADC map is empty.";
        return false;
    }
    if (tag_id_array.size() == 0){
        LOG(WARNING) << "No maxima found.";
    }
    std::ofstream csv_file;
    csv_file.open(filename);
    csv_file << "x,y,adc,tag_id" << std::endl;
    for (auto i = 0; i < adc_map->value_vec.size(); i++){
        auto x_index = Double_t(adc_map->x_vec.at(i));
        auto y_index = Double_t(adc_map->y_vec.at(i));
        for (auto j = 0; j < cells->size(); j++){
            auto cell = cells->at(j);
            if (abs(cell->x - x_index) < 0.01 && abs(cell->y - y_index) < 0.01){
                csv_file << cell->x << "," << cell->y << "," << cell->adc << ",";
                if (cell->is_tagged()){
                    csv_file << cell->tags.at(0);
                }
                else{
                    csv_file << "-1";
                }
                csv_file << std::endl;
                break;
            }
        }
    }
    csv_file.close();
    return true;
}

void cluster_MA::assign_adc_to_cells(SJUtil::DataErrorSet2D<double> *adc_map){
    auto adc_map_size = adc_map->value_vec.size();
    for (auto i = 0; i < adc_map_size; i++){
        auto x_index = Double_t(adc_map->x_vec.at(i));
        auto y_index = Double_t(adc_map->y_vec.at(i));
        bool is_in_range = false;
        for (auto j = 0; j < cells->size(); j++){
            auto cell = cells->at(j);
            if (abs(cell->x - x_index) < 0.01 && abs(cell->y - y_index) < 0.01){
                
                cell->adc = adc_map->value_vec.at(i);
                is_in_range = true;
                break;
            }
        }
        if (!is_in_range){
            LOG(WARNING) << "ADC map is out of range.";
            LOG(WARNING) << "x: " << x_index << ", y: " << y_index;
        }
    }
}

bool cluster_MA::find_maxima(){
    auto cells_size = cells->size();
    Double_t _current_max_adc = 0;
    auto _current_max_index = -1;
    auto _current_max_cell = (cell_MA*)nullptr;
    for (auto i=0; i<cells_size; i++){
        auto cell = cells->at(i);
        if (cell->is_tagged())
            continue;
        if (cell->adc < threshold_seed)
            continue;
        if (cell->adc > _current_max_adc){
            _current_max_adc = cell->adc;
            _current_max_cell = cell;
            _current_max_index = i;
        }
    }
    if (_current_max_cell == nullptr){
        // LOG(ERROR) << "No maxima found.";
        return false;
    } else {
        tag_id_array.push_back(_current_max_index);
        _current_max_cell->set_maxima(_current_max_index);
        // LOG(DEBUG) << "Maxima found at (" << _current_max_cell->x << ", " << _current_max_cell->y << ").";
    }
    return true;
}

bool cluster_MA::iterate(){
    int _propagated_cells = 0;

    for (auto i = 0; i < cells->size(); i++){
        auto cell = cells->at(i);
        if (!cell->is_tagged())
            continue;
        if (cell->has_propagated)
            continue;

        bool _is_larger_neighbor = false;
        std::vector<cell_MA*> _neighbor_cells;
        for (auto j = 0; j < cells->size(); j++){
            if (i == j)
                continue;
            auto cell2 = cells->at(j);
            if (cell2->is_tagged())
                continue;
            if (cell->get_distance_squared(*cell2) < neighbor_distance){
                // * now it's a valid neighbor
                if (cell2->adc > cell->adc + threshold_larger_neighbor){
                    _is_larger_neighbor = true;
                    break;
                }
                _neighbor_cells.push_back(cell2);
            }   
        }
        cell->has_propagated = true;
        if (_is_larger_neighbor)
            continue;
        else {
            _propagated_cells++;
            // * now it's a valid cell to propagate
            for(auto cell2: _neighbor_cells){
                cell2->add_tag(cell->tags.at(0));
            }
        }
    }
    // LOG(DEBUG) << "Propagated cells: " << _propagated_cells;
    return _propagated_cells > 0;
}

void cluster_MA::cells_update(){
    for (auto i = 0; i < cells->size(); i++)
        cells->at(i)->update();
}

