#pragma once

#include <vector>

#include "SJ_utilities.h"

#include "csv.h"
#include "easylogging++.h"

#define SIZE_CENTER 5
#define SIZE_OUTER  7

class cluster_CA;

class cell_CA {
    friend class cluster_CA;
    public:
        cell_CA();
        cell_CA(Double_t x, Double_t y, Double_t size, Double_t adc);
        ~cell_CA();
    public:
        void update(void);
        void set_local_maximum(Int_t _tag_id);
        void add_tag(Int_t _tag_id);

        Double_t get_distance(cell_CA _cell);
        Double_t get_distance_squared(cell_CA _cell);

        inline bool is_tagged(void){return tags.size() > 0;}
        inline bool is_localmaximum(void){return is_local_maximum;}
    private:
        Double_t x;
        Double_t y;
        Double_t size;
        Double_t adc;
        bool is_local_maximum;
        std::vector<Int_t> tags;
        std::vector<Int_t> tags_buffer;
};

class cluster_CA {
    public:
        cluster_CA();
        ~cluster_CA();
    public:
        void assign_adc_to_cells(SJUtil::DataErrorSet2D<double> *adc_map);
        bool save_event_to_csv(SJUtil::DataErrorSet2D<double> *adc_map, std::string filename);
        inline void set_local_max_threshold(Double_t _threshold){local_max_threshold = _threshold;}
        bool find_local_maximum(void);

        bool iterate(void);

        inline Int_t get_n_tags(void){return tag_id_array.size();}

    private:
        void cells_update(void);
    private:
        std::vector<cell_CA*> *cells;
        std::vector<Int_t> tag_id_array;
        Double_t local_max_threshold;
};
