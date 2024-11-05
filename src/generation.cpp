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
        if( input(x,y,z).state < m_g0 || input(x,y,z).state == Domain::BOND.state ) continue;
        f_vec pos = {static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)};

        double shadowing = 1.0;
        for(_long_int dy = 0; dy <= 1; dy++)
        for(_long_int dz = m_domain.neighbourhood.dz0; dz <= m_domain.neighbourhood.dz1; dz++)
        for(_long_int dx = m_domain.neighbourhood.dx0; dx <= m_domain.neighbourhood.dx1; dx++)
        {
            if(input.state(pos.x + dx, pos.y + dy, pos.z + dz).state != Domain::VOID.state)
            {
                double sh = 1.0 - m_prefered_orientation*f_vec{dx,dy,z};
                if(sh >=  1.0) sh = 1.0;
                if(sh < shadowing) shadowing = sh;
            }  
        }

        _int idx = -1;
        for(_long_int dy = m_domain.neighbourhood.dy0; dy <= m_domain.neighbourhood.dy1; dy++)
        for(_long_int dz = m_domain.neighbourhood.dz0; dz <= m_domain.neighbourhood.dz1; dz++)
        for(_long_int dx = m_domain.neighbourhood.dx0; dx <= m_domain.neighbourhood.dx1; dx++)
        {
            idx++;
            cell c = input.state(pos.x + dx, pos.y + dy, pos.z + dz);
            if((c.state == Domain::VOID.state) || (c.state < m_g0) || (m_domain(x,y,z).state == Domain::BOND.state) || (c.state == Domain::BOND.state)){
                continue;
            }

            Grain&  grain = m_grains[c.state];

            double dt = INFINITY;
            {

                //f_vec k = {-dx, -dy, -dz};
                //k.normalize();

                //double theta = acos(grain.orientation*k);
                // double v_k = cos(m_alpha_g * acos(k*m_prefered_orientation)) * (m_inv_dk + cos(m_alpha_t*grain.theta[idx]));
                //double v_k = m_layer->cosAlphaG[idx] * (m_inv_dk + cos(m_alpha_t*grain.theta[idx]));
               
                dt = (1.0+m_diff)*(sqrt(dx*dx + dy*dy + dz*dz)) /(shadowing*(*m_vkMatrix)[idx][c.state]+ m_diff) ;
               
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

