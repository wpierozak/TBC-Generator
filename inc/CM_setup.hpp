#include"CM_datatypes.hpp"
#include"CM_task.hpp"

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ);

void defineTasks(const Configuration& config, tasks_array& taskData);

void nucleation(Configuration& config);

void setGrowthTensor(Grain& grain, const Microstructure_Properties& msp);

void setMaxWidenAngle(Grain& grain, const Microstructure_Properties& msp);

void setSmoothRegionLen(Grain& grain, const Microstructure_Properties& msp);

void setFeatheredRegionLen(Grain& grain, const Microstructure_Properties& msp);

void setTopRegionLen(Grain& grain, const Microstructure_Properties& msp);

void setReferenceRadius(Grain& grain, const Microstructure_Properties& msp);