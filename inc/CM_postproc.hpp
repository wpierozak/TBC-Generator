#pragma once

#include"CM_datatypes.hpp"
void fillColorBuffer(Domain& caDomain, const int threadNum);
void createBitmap(Domain& caDomain);
cm_colorampl* defineColors(cm_size grainNum);