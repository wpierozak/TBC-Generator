#pragma once

#include"CM_config.hpp"

void createBitmap(Configuration& caDomain);
uint8_t* defineColors(uint16_t grainNum);
void saveMicrostructureFile(Domain& domain, Configuration& config);