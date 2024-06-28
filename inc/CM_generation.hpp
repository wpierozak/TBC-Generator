#pragma once

#include<list>
#include<unordered_map>
#include"CM_config.hpp"

class Generator
{
    public:
    struct Subspace
    {
        cm_pos x0,x1;
        cm_pos y0,y1;
        cm_pos z0,z1;
    };

    Generator(Domain& domain, Subspace subspace);
    void update_grains(std::unordered_map<cm_state, Grain>& grains);
    void set_g0(cm_int g0) { m_g0 = g0; }

    void run();
    Subspace& subspace() {return m_subspace;}

    private: 

    void check_neighbourhood(f_vec pos, std::list<cm_state>& grains);
    void cell(f_vec pos);
    double fitness_value(f_vec pos, const Grain& grain);

    Domain m_domain;
    Subspace m_subspace;

    cm_int m_g0;

    std::unordered_map<cm_state, Grain> m_grains;
};