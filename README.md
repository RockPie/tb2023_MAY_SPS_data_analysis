# SJ's CAEN data analysis project

Shihai J.
Niels Bohr Institute, University of Copenhagen

- [SJ's CAEN data analysis project](#sjs-caen-data-analysis-project)
  - [Typical workflow](#typical-workflow)
    - [a. Read data from original text file and write to root frames file](#a-read-data-from-original-text-file-and-write-to-root-frames-file)
    - [b. Open a root frames file and use event builder to reconstruct events](#b-open-a-root-frames-file-and-use-event-builder-to-reconstruct-events)
    - [c. Read a mapping file](#c-read-a-mapping-file)
    - [d. Read a mapping file and generate x,y coordinates](#d-read-a-mapping-file-and-generate-xy-coordinates)
    - [e. Read reconstructed events and get channel locations based on the mapping file](#e-read-reconstructed-events-and-get-channel-locations-based-on-the-mapping-file)


## Typical workflow

### a. Read data from original text file and write to root frames file

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
auto reader  = new CAEN_data_reader();
auto builder = new CAEN_event_builder();
reader ->read_root_file2frame_array(file_root_frames_path);
builder->reconstruct_event(reader->get_frame_info_array_ptr(), INFINITE_FRAMES);
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