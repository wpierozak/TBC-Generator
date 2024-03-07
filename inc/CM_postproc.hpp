#pragma once

#include"CM_datatypes.hpp"
void fillColorBuffer(GeneratorConfig& caDomain, const int threadNum);
void createBitmap(GeneratorConfig& caDomain, const int threadsNum);
cm_colorampl* defineColors(cm_size grainNum);
void saveMicrostructureFile(GeneratorConfig& caDomain);