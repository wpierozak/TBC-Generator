#pragma once

#include<list>
#include<vector>
#include"CM_config.hpp"
#include"CM_task.hpp"

class Generator
{
    public:
    struct Subspace
    {
        cm_pos x0,x1;
        cm_pos y0,y1;
        cm_pos z0,z1;
    };

    Generator(Domain& domain, Subspace subspace, const grains_array& grains);

    void run();
    Subspace& subspace() {return m_subspace;}

    private: 

    void check_neighbourhood(f_vec pos, std::list<cm_state>& grains);
    void cell(f_vec pos);
    double fitness_value(f_vec pos, const Grain& grain);

    Domain m_domain;
    Subspace m_subspace;

    grains_array m_grains;
};