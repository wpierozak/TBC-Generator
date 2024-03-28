#include"CM_datatypes.hpp"

void copySubdomainsArray(subdomains_array& dest, subdomains_array& src);

void createSubdomains(GeneratorConfig& config, subdomains_array& subdomains);

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ);

/* Function sets all cells to a default value and performs a nucleation */
void nucleation(GeneratorConfig& domain);

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, Microstructure_Properties& msp);

/* Defines reference bound for column width by bounding the angle between growth tensor and relative position vector*/
void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp);

void setSmoothRegionLen(Grain& grain, Microstructure_Properties& msp);

void setFeatheredRegionLen(Grain& grain, Microstructure_Properties& msp);

void setTopRegionLen(Grain& grain, Microstructure_Properties& msp);

void setReferenceRadius(Grain& grain, Microstructure_Properties& msp);