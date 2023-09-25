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

class cluster_MA;

class cell_MA {
    friend class cluster_MA;
    public:
        cell_MA();
        cell_MA(Double_t x, Double_t y, Double_t size, Double_t adc);
        ~cell_MA();
    public:
        void set_maxima(Int_t _tag_id);
        void add_tag(Int_t _tag_id);
        
        void update(void);

        Double_t get_distance(cell_MA _cell);
        Double_t get_distance_squared(cell_MA _cell);

        inline bool is_tagged(void){return tags.size() > 0;}
        // inline bool is_maxima(void){return is_maxima;}
    private:
        Double_t x;
        Double_t y;
        Double_t size;
        Double_t adc;
        bool is_maxima;
        bool has_propagated;
        std::vector<Int_t> tags;
        std::vector<Int_t> tags_buffer;
};

class cluster_MA {
    public:
        cluster_MA();
        ~cluster_MA();
    public:
        void assign_adc_to_cells(SJUtil::DataErrorSet2D<double> *adc_map);
        bool save_event_to_csv(SJUtil::DataErrorSet2D<double> *adc_map, std::string filename);
        inline void set_neighbor_distance(Double_t _distance){neighbor_distance = _distance;}
        inline void set_seed_threshold(Double_t _threshold){threshold_seed = _threshold;}
        inline void set_aggregation_threshold(Double_t _threshold){threshold_agg = _threshold;}
        inline void set_larger_neighbor_threshold(Double_t _threshold){threshold_larger_neighbor = _threshold;}

        void cells_update(void);

        bool find_maxima(void);

        bool iterate(void);

        inline Int_t get_n_tags(void){return tag_id_array.size();}
    private:
        std::vector<cell_MA*> *cells;
        std::vector<Int_t> tag_id_array;
        Double_t neighbor_distance;
        Double_t threshold_seed;
        Double_t threshold_agg;
        Double_t threshold_larger_neighbor;
};