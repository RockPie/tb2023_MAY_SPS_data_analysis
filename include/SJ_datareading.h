// *****************************************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 01-06-2023
// *****************************************************************************


#ifndef SJ_DATAREADING_H
#define SJ_DATAREADING_H

#include <iostream>
#include <TFile.h>
#include <TTree.h>

class CAEN_data_reader {
public: 
    CAEN_data_reader();
    ~CAEN_data_reader();
private:
    TFile *file_ptr;
    TTree *tree_ptr;
    Int_t x;
};

#endif // SJ_DATAREADING_H