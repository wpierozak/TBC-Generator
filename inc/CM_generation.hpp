#pragma once

#include<list>
#include<vector>
#include"CM_config.hpp"
#include"CM_task.hpp"

void scanNeighbourhood(f_vec pos, const Domain& domain, std::list<cm_state>& grains);
double singleFieldCalculation(f_vec pos, const Grain& grain);
void runTask(Task* task);

class Generator
{
    public:
    struct Subspace
    {
        cm_int x0,x1;
        cm_int y0,y1;
        cm_int z0,z1;
    };

    Generator(Domain&& domain, Subspace subspace, const grains_array& grains);

    void run();

    private: 

    void cell(f_vec pos);
    void fitness_value(f_vec pos, const Grain& grain);

    Domain m_domain;
    Subspace m_subspace;

    grains_array m_grains;
};