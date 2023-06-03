#include "SJ_eventbuilder.h"

CAEN_event_builder::CAEN_event_builder(){
    event_array_ptr = new std::vector<EventInfo>;
    event_valid_array_ptr = new std::vector<bool>;
}

CAEN_event_builder::~CAEN_event_builder(){
    event_array_ptr->clear();
    event_array_ptr->resize(0);
    delete event_array_ptr;
    event_valid_array_ptr->clear();
    event_valid_array_ptr->resize(0);
    delete event_valid_array_ptr;
}

bool CAEN_event_builder::reconstruct_event(std::vector<CAEN_data_reader::FrameInfo> *_frame_array_ptr, int _frame_num){
    if(_frame_array_ptr->size() < _frame_num && _frame_num != INFINITE_FRAMES){
        LOG(ERROR) << "Frame number is less than " << _frame_num;
        return false;
    }
    if (_frame_num > _frame_array_ptr->size())
        _frame_num = _frame_array_ptr->size();
    if (_frame_num == INFINITE_FRAMES)
        _frame_num = _frame_array_ptr->size();

    EventInfo _tmp_event;
    _tmp_event.timestamps.resize(EVENT_FRAME_NUM);
    _tmp_event.HG_charges.resize(EVENT_FRAME_NUM*DEFAULT_CHANNEL_NUMBER);
    _tmp_event.LG_charges.resize(EVENT_FRAME_NUM*DEFAULT_CHANNEL_NUMBER);

    std::list<Long_t> _trigID_array;
    std::vector<Long_t> _unique_trigID_array;

    // * Create a list of trigger ID
    // * like: [1, 1, 1, 2, 2, 2, 3, 3, 3]
    for (auto i = 0; i<_frame_num; i++){
        _trigID_array.push_back(_frame_array_ptr->at(i).trigID);
    }
    LOG(DEBUG) << "Trigger ID list created";

    // * Create a map of trigger ID and its count
    // * like: {1:3, 2:3, 3:3}
    std::unordered_map<Long_t, int> _trigID_count_map;
    for (auto i = _trigID_array.begin(); i != _trigID_array.end(); i++){
        _trigID_count_map[*i]++;
    }
    LOG(DEBUG) << "Trigger ID count map created";

    // * Create event array based on unique trigger IDs
    // * like: [1, 2, 3]
    // * and if it is a valid event
    // * like: [true, true, true]
    for (auto i = _trigID_count_map.begin(); i != _trigID_count_map.end(); i++){
        if (i->second == EVENT_FRAME_NUM)
            event_valid_array_ptr->push_back(true);
        else
            event_valid_array_ptr->push_back(false);
        _tmp_event.trigID = i->first;
        event_array_ptr->push_back(_tmp_event);
        _unique_trigID_array.push_back(i->first);
    }
    LOG(DEBUG) << "Event array created";
    LOG(INFO) << "Filling event array...";
    // * Go through the frame array and fill the event array
    for (auto i = 0; i<_frame_num; i++){
        auto _current_trigID = _frame_array_ptr->at(i).trigID;
        auto _current_boardNum = _frame_array_ptr->at(i).nboards;
        auto _event_index = std::distance(_unique_trigID_array.begin(), std::find(_unique_trigID_array.begin(), _unique_trigID_array.end(), _current_trigID));
        // * Fill the corresponding event
        event_array_ptr->at(_event_index).timestamps[_current_boardNum] = _frame_array_ptr->at(i).timestamp;
        for (auto j = 0; j<DEFAULT_CHANNEL_NUMBER; j++){
            event_array_ptr->at(_event_index).HG_charges[_current_boardNum*DEFAULT_CHANNEL_NUMBER+j] = 
                _frame_array_ptr->at(i).HG_charge[j];
            event_array_ptr->at(_event_index).LG_charges[_current_boardNum*DEFAULT_CHANNEL_NUMBER+j] = 
                _frame_array_ptr->at(i).LG_charge[j];
        }
    }
    LOG(DEBUG) << "Event array filled";

    LOG(INFO) << event_array_ptr->size() << " events reconstructed";

    auto _valid_event_num = 0;
    for (auto i = 0; i<event_valid_array_ptr->size(); i++){
        if (event_valid_array_ptr->at(i))
            _valid_event_num++;
    }

    LOG(INFO) << _valid_event_num << " valid events";

    return true;
}

std::vector<Int_t> CAEN_event_builder::get_event_sum_array() {
    std::vector<Int_t> _event_sum_array;
    for (auto i = 0; i<event_array_ptr->size(); i++){
        if (!event_valid_array_ptr->at(i))
            continue;
        auto _sum = 0;
        for (auto j = 0; j<event_array_ptr->at(i).HG_charges.size(); j++){
            _sum += event_array_ptr->at(i).HG_charges[j];
        }
        _event_sum_array.push_back(_sum);
    }
    return _event_sum_array;
}