#include<iostream>
#include<omp.h>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include<unordered_map>
#include"generation.hpp"



Generator::Generator(Domain& domain, Subspace subspace):
    m_domain(domain), m_subspace(subspace)
{
    
}

void Generator::update_grains(std::unordered_map<uint16_t, Grain> &grains)
{
    m_grains = grains;
}

void Generator::run(Domain& input, Domain& output, double ct)
{
    f_vec space = {m_domain.dimX, m_domain.dimY, m_domain.dimZ};

    for(_long_int y = m_subspace.y0; y < m_subspace.y1; y++)
    for(_long_int z = m_subspace.z0; z < m_subspace.z1; z++)
    for(_long_int x = m_subspace.x0; x < m_subspace.x1; x++)
    {
        if( m_domain(x,y,z).state < m_g0 || m_domain(x,y,z).state == Domain::BOND.state) continue;
        f_vec pos = {static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)};

        for(_long_int dy = m_domain.neighbourhood.dy0; dy <= m_domain.neighbourhood.dy1; dy++)
        for(_long_int dz = m_domain.neighbourhood.dz0; dz <= m_domain.neighbourhood.dz1; dz++)
        for(_long_int dx = m_domain.neighbourhood.dx0; dx <= m_domain.neighbourhood.dx1; dx++)
        {
            cell c = input.state(pos.x + dx, pos.y + dy, pos.z + dz);
            if(c == Domain::VOID || c.state < m_g0 || m_domain(x,y,z).state == Domain::BOND.state) continue;

            Grain&  grain = m_grains[c.state];

            double dt = INFINITY;
            {
                f_vec vp = virtual_pos(pos, {dx,dy,dz}, space);

                double theta = acos((m_prefered_orientation*f_vec{-dx,-dy,-dz})/sqrt(dx*dx + dy*dy + dz*dz));
                double beta = atan_jw((vp - grain.center).cross(grain.orientation).norm());

                dt =  sqrt(dx*dx + dy*dy + dz*dz) /( cos(beta*m_alpha_g)* ( 0.1 + cos(theta*m_alpha_t)) );

                if(dt <= 0) dt = INFINITY;
            }

            //double time = dt(sqrt(dx*dx + dy*dy + dz*dz), virtual_pos(pos, {dx,dy,dz}, space), grain) + c.time;
            double time = dt + c.time;

            if(time < input(x,y,z).time && time <= ct)
            {
               output(x,y,z) = {c.state, time};
            }
        }
    }
}

