#include"CM_config.hpp"
#include"CM_task.hpp"

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ);

void defineTasks(Configuration& config, tasks_array& taskData);

void nucleation(Configuration& config);

void setGrowthTensor(Grain& grain, Microstructure_Properties& msp);

void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp);

void setTopRegionLen(Grain& grain, Microstructure_Properties& msp);

void setReferenceRadius(Grain& grain, Microstructure_Properties& msp);

void setReferenceLength(Grain& grain, Microstructure_Properties& msp);

void setRVector(Grain& grain);