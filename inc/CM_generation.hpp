#pragma once

#include<list>
#include<vector>
#include"CM_config.hpp"
#include"CM_task.hpp"

void scanNeighbourhood(f_vec pos, const Domain& domain, std::list<cm_state>& grains);
cm_int singleFieldCalculation(f_vec pos, const Grain& grain);
void runTask(Task& task);