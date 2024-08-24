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
    void update_grains(std::unordered_map<uint16_t, Grain>& grains);
    void set_g0(cm_int g0) { m_g0 = g0; }
    void set_prefered_orientation(f_vec& p){ m_prefered_orientation = p; }

    void run(Domain& input, Domain& output, double ct);
    Subspace& subspace() {return m_subspace;}

    private: 

    double dt(double d, f_vec pos, const Grain& grain);

    Domain m_domain;
    Subspace m_subspace;

    f_vec m_prefered_orientation;
    cm_int m_g0;

    std::unordered_map<uint16_t, Grain> m_grains;
};