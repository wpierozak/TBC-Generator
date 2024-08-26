#pragma once

#include<list>
#include<unordered_map>
#include"CM_config.hpp"

class Generator
{
    public:
    struct Subspace
    {
        _long_int x0,x1;
        _long_int y0,y1;
        _long_int z0,z1;
    };

    Generator(Domain& domain, Subspace subspace);
    void update_grains(std::unordered_map<uint16_t, Grain>& grains);
    void set_g0(_int g0) { m_g0 = g0; }
    void set_prefered_orientation(f_vec& p){ m_prefered_orientation = p; }

    void run(Domain& input, Domain& output, double ct);
    Subspace& subspace() {return m_subspace;}

    private: 

    double dt(double d, f_vec pos, const Grain& grain);

    Domain m_domain;
    Subspace m_subspace;

    f_vec m_prefered_orientation;
    _int m_g0;

    std::unordered_map<uint16_t, Grain> m_grains;
};