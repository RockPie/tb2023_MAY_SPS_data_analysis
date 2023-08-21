# SJ's CAEN analysis code

Shihai J.
Niels Bohr Institute, University of Copenhagen

- [SJ's CAEN data analysis project](#sjs-caen-data-analysis-project)
  - [1. Typical workflow](#1-typical-workflow)
    - [a. Read data from original text file and write to root frames file](#a-read-data-from-original-text-file-and-write-to-root-frames-file)
    - [b. Open a root frames file and use event builder to reconstruct events](#b-open-a-root-frames-file-and-use-event-builder-to-reconstruct-events)
    - [c. Read a mapping file](#c-read-a-mapping-file)
    - [d. Read a mapping file and generate x,y coordinates](#d-read-a-mapping-file-and-generate-xy-coordinates)
    - [e. Read reconstructed events and get channel locations based on the mapping file](#e-read-reconstructed-events-and-get-channel-locations-based-on-the-mapping-file)
    - [f. Generate accumulated charge map](#f-generate-accumulated-charge-map)
  - [2. 2-D reconstruction](#2-2-d-reconstruction)
    - [a. Processing of the mapped data](#a-processing-of-the-mapped-data)
  - [3. Parallel bash script](#3-parallel-bash-script)


## 1. Typical workflow

![workflow](./Architecture.drawio.png)

### a. Read data from the original text file and write to the root frames file

```mermaid
graph LR
A["original text file \n (.txt)"] --"CAEN_data_reader"--> B("std::vector \n < FrameInfo >")
B --> C["root frames file \n (.root)"]
```

```cpp
CAEN_data_reader *reader = new CAEN_data_reader(file_CAEN_path);
reader->extract_frame_info_array(INFINITE_FRAMES, true);
reader->write_frame_array2root_file(file_root_frames_path);
delete reader;
```

### b. Open a root frames file and use event builder to reconstruct events

```mermaid
graph LR
A["frames file \n (.root)"] --CAEN_data_reader--> B("std::vector \n < FrameInfo >")
B --"CAEN_event_builder"--> D("std::vector \n < EventInfo >")
D --> E["events file \n (.root)"]
```

```cpp
// * Other info generation
auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
auto pedestalInfo   = SJUtil::read_pedestal_csv_file(file_pedestal_path);
// * Main program

auto reader  = new CAEN_data_reader();
auto builder = new CAEN_event_builder();
reader ->read_root_file2frame_array(file_root_frames_path);
builder->set_pedestal_info(pedestalInfo);
builder->reconstruct_event(reader->get_frame_info_array_ptr (),INFINITE_FRAMES, true);
builder->write_event_array2root_file(file_root_events_path);
delete builder;
delete reader;
```

### c. Read a mapping file 

```cpp
auto mapping = SJUtil::read_mapping_csv_file("path_to_mapping.csv");

auto Mapping_Board_Num_Array    = mapping[0];
auto Mapping_Channel_Num_Array  = mapping[1];
auto Mapping_Module_Num_Array   = mapping[2];
auto Mapping_Col_Array          = mapping[3];
auto Mapping_Row_Array          = mapping[4];
```

### d. Read a mapping file and generate x,y coordinates

```cpp
auto mapping = SJUtil::read_mapping_csv_file("path_to_mapping.csv");
auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);

auto uni_chn = mapping_coords[0];
auto x_coord = mapping_coords[1];
auto y_coord = mapping_coords[2];
```

### e. Read reconstructed events and get channel locations based on the mapping file

```mermaid
graph LR
A["events file \n (.root)"] --> B("std::vector \n < EventInfo >")
C["mapping file \n (.csv)"] --SJUtil--> D("uni_chn \n x_coord \n y_coord")
B --"SJUtil::map1d_to_2d"--> E("x_coord \n y_coord \n HG/LG_charges")
E --> F["root results file \n (.root)"]
D --"SJUtil::map1d_to_2d"--> E
```

```cpp
auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);
auto builder = new CAEN_event_builder();
builder->read_root_file2event_array(file_root_events_path);

auto eventArrayPtr  = builder->get_event_array_ptr();
auto eventValidPtr  = builder->get_event_valid_array_ptr();
auto eventNum       = int(eventValidPtr->size() / 100);
TFile *f = new TFile(file_root_results_path, "RECREATE");

for (auto i = 0; i < eventNum; i++){
    if (!eventValidPtr->at(i)) continue;
    auto HG_charges     = eventArrayPtr->at(i).LG_charges;
    auto _currentName   = Form("event_%d", i);
    auto _currentTitle  = Form("Event %d", i);
    auto Canvas_Ptr     = new TCanvas(_currentName, _currentTitle, 200,10, 700, 500);
    auto Graph_Ptr      = SJPlot::scatter_3d(HG_charges, mapping_coords,_currentName, _currentTitle);
    Canvas_Ptr->Update();
    Canvas_Ptr->WaitPrimitive();
    Canvas_Ptr->Write();
    delete Graph_Ptr;
    delete Canvas_Ptr;
}
f->Close();
delete builder;
```

### f. Generate accumulated charge map

```cpp
// * Main program
auto mapping        = SJUtil::read_mapping_csv_file(file_mapping_path);
auto mapping_coords = SJUtil::generate_mapping_croodinate(mapping);

auto builder = new CAEN_event_builder();
builder->read_root_file2event_array(file_root_events_path);

auto eventArrayPtr  = builder->get_event_array_ptr();
auto eventValidPtr  = builder->get_event_valid_array_ptr();
auto eventNum       = int(eventValidPtr->size() / 100);

TFile *f = new TFile(file_root_results_path, "RECREATE");

// * Get HGain data
std::vector<std::vector<Short_t>> HGain_data;
for (int i = 0; i < eventNum; i++)
    if (eventValidPtr->at(i))
        HGain_data.push_back(eventArrayPtr->at(i).LG_charges);

// * Generate 2-D distribution
auto Canvas_Ptr = new TCanvas("Canvas", "Canvas", 800, 600);
auto hist2D = SJPlot::distribution_2d(HGain_data, "hgain", "HGain_2D");
hist2D->Draw("colz");
gStyle->SetPalette(103);

std::vector<TLine*> LinePtrArray;

LinePtrArray.push_back(
    SJPlot::add_horizontal_line(35, 0, 105, kWhite, 2, 1));
LinePtrArray.push_back(
    SJPlot::add_horizontal_line(70, 0, 105, kWhite, 2, 1));
LinePtrArray.push_back(
    SJPlot::add_vertical_line(35, 0, 105, kWhite, 2, 1));
LinePtrArray.push_back(
    SJPlot::add_vertical_line(70, 0, 105, kWhite, 2, 1));

Canvas_Ptr->Write();

f->Close();

for (auto i=0; i<4; i++)
    delete LinePtrArray.at(i);

delete Canvas_Ptr;
delete builder;
```

## 2. 2-D reconstruction

### a. Processing of the mapped data

``` mermaid
graph LR
A0("HG_\ncharges") --"to\ndouble"--> A("HG_charges\n_double")   --"map1d\n_to_2d"--> A1("twoD_hg\n_values") --"noise_\nsubtracted\n_data"--> A2("twoD_hg\n_values_N")
B0("LG_\ncharges") --> B("LG_charges\n_Multipled") 
--"map1d\n_to_2d"--> B1("twoD_lg\n_values") --"noise_\nsubtracted\n_data"--> B2("twoD_lg\n_values_N")
S1("slope\nInfo") --> B
A2 --"area_\nnormalized\n_data"--> A3("twoD_hg\n_values_NA")
B2 --"area_\nnormalized\n_data"--> B3("twoD_lg\n_values_NA")
A3 --> C("target\n_event")
B3 --> C
```

```cpp
std::vector<Double_t> HG_charges_double;
for (auto i = 0; i < HG_charges.size(); i++)
    HG_charges_double.push_back(Double_t(HG_charges[i]));
auto LG_charges_Multipled =SJUtil::gain_multiplication(slopeInfo, offsetInfo, LG_charges);
auto _twoD_hg_values      =SJUtil::map1d_to_2d(HG_charges_double, mapping_coords);
auto _twoD_lg_values      =SJUtil::map1d_to_2d(LG_charges_Multipled, mapping_coords);
auto _twoD_hg_values_N    =SJUtil::noise_subtracted_data(_twoD_hg_values, 1);
auto _twoD_lg_values_N    =SJUtil::noise_subtracted_data(_twoD_lg_values, 1);
auto _twoD_hg_values_NA   =SJUtil::area_normalized_data(_twoD_hg_values_N);
auto _twoD_lg_values_NA   =SJUtil::area_normalized_data(_twoD_lg_values_N);
auto _target_event        =SJUtil::substitued_data(
    _twoD_hg_values_NA, 
    _twoD_lg_values_NA,
    Double_t(1500),
    Double_t(1)
);
```

Here the `_twoD_hg_values` and `_twoD_lg_values` are the 2-D data point sets of the HG and LG charges. The noise-subtracted data are the `_twoD_hg_values_N` and `_twoD_lg_values_N`. 

<div style="border: 1px solid #faebcc; background:#fcf8e3; color:#8a6d3b; padding: .75rem 1.25rem; border-radius: .25rem;"><b>Important:</b> The SJUtil::noise_subtracted_data will also be used to subtract data points under 0 caused by pedestal-subtraction </div>

The `_twoD_hg_values_NA` and `_twoD_lg_values_NA` are the area-normalized data. The `_target_event` is the data set to be processed.

<div style="border: 1px solid #faebcc; background:#fcf8e3; color:#8a6d3b; padding: .75rem 1.25rem; border-radius: .25rem;"><b>Important:</b> The normalization will make data points in the outer module raised by 49/25</div>

Because the LG values are already multiplied by the gain with the `SJUtil::gain_multiplication` function, the gain settings in `SJUtil::substitued_data` is set to `Double_t(1)`.


## 3. Parallel bash script

```bash
# ./build/SJS_fitting_general_main -n <value> -t <value> -e <value>

# Define the number of parallel processes
max_processes=8
event_total=1000

# Function to execute the command
execute_command() {
    ../build/SJS_fitting_general_main -n $1 -t $2 -e $3
}

# Export the function so it can be called by xargs
export -f execute_command

# Run the command in parallel using xargs
seq 1 $max_processes | xargs -n 1 -P $max_processes -I {} bash -c "execute_command {} $max_processes $event_total"
```

The script is used for parallel processing. The user needs to set the `max_processes` and `event_total` parameters. The `max_processes` is the number of parallel processes, and the `event_total` is the total number of events to be processed.

In the above example, the script will run **8 parallel processes**, and each process will process **125 events**. The total number of events is 1000.

<div style="border: 1px solid #BCE8F1; background:#D9EDF7; color:#31708F; padding: .75rem 1.25rem; border-radius: .25rem;"><b>Info:</b> It is recommended to use ((Your-CPU-Cores-Num) - 2) as the max_processed parameter</div>
