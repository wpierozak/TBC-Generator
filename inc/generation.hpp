#pragma once

#include<list>
#include<unordered_map>
#include"config.hpp"
#include"grain.hpp"

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
    void set_prefered_orientation(f_vec p){ m_prefered_orientation = p; }
    void set_alpha_g(double alpha){m_alpha_g = alpha;}
    void set_alpha_t(double alpha){m_alpha_t = alpha;}
    void set_inv_dk(double dk){m_inv_dk = 1.0/dk;}
    void set_diffusion(double diff){m_diff = diff;}

    void run(Domain& input, Domain& output, double ct);
    Subspace& subspace() {return m_subspace;}

    private: 

    f_vec virtual_pos(const f_vec& pos, f_vec&& relative_pos, const f_vec& space)
    {
        f_vec virtual_pos = pos;

        if(pos.x + relative_pos.x < 0)
        {
            virtual_pos.x = space.x + relative_pos.x;
        }
        else if(pos.x + relative_pos.x >= space.x)
        {
            virtual_pos.x = -relative_pos.x;
        }

        if(pos.y + relative_pos.y < 0)
        {
            virtual_pos.y = space.y + relative_pos.y;
        }
        else if(pos.y + relative_pos.y >= space.y)
        {
            virtual_pos.y = -relative_pos.y;
        }

        if(pos.z + relative_pos.z < 0)
        {
            virtual_pos.z = space.z + relative_pos.z;
        }
        else if(pos.z + relative_pos.z >= space.z)
        {
            virtual_pos.z = -relative_pos.z;
        }

        return virtual_pos;
    }

    Domain m_domain;
    Subspace m_subspace;

    f_vec m_prefered_orientation;
    _int m_g0;
    
    double m_alpha_g;
    double m_alpha_t;
    double m_diff;
    double m_inv_dk;

    std::unordered_map<uint16_t, Grain> m_grains;
};