#pragma once

#include"CM_datatypes.hpp"

void createBitmap(Configuration& caDomain, const int threadsNum);
cm_colorampl* defineColors(cm_size grainNum);
void saveMicrostructureFile(Configuration& caDomain);