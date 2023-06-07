// ***********************************************************
// * Author: Shihai J.
// * Description:
// *   To be added.
// * Created: 07-06-2023
// ***********************************************************

#pragma once

#define HG_RELATIVE_GAIN    9.0
#define SUBSTITUE_THRESHOLD 4096-100
#include "easylogging++.h"

namespace SJFunc{
    double gaussian2D (double *x, double *par);
    double gaussian2D_c (double *x, double *par);
}; // namespace SJFunc