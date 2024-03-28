#include"CM_datatypes.hpp"
#include"CM_task.hpp"

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ);

/* Function sets all cells to a default value and performs a nucleation */
void nucleation(Configuration& config);

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, const Microstructure_Properties& msp);

/* Defines reference bound for column width by bounding the angle between growth tensor and relative position vector*/
void setMaxWidenAngle(Grain& grain, const Microstructure_Properties& msp);

void setSmoothRegionLen(Grain& grain, const Microstructure_Properties& msp);

void setFeatheredRegionLen(Grain& grain, const Microstructure_Properties& msp);

void setTopRegionLen(Grain& grain, const Microstructure_Properties& msp);

void setReferenceRadius(Grain& grain, const Microstructure_Properties& msp);