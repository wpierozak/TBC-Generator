#include"CM_datatypes.hpp"

void fillColorBuffer_GPU(Domain& caDomain);
cm_colorampl* defineColors(cm_size grainNum);

__global__ void assignColors(cm_state* caDomain, cm_colorampl* rgb, const cm_colorampl* __restrict__ colorArray);