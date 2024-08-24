#include<iostream>
#include<omp.h>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include<unordered_map>
#include"CM_generation.hpp"

double noise()
{
    static std::minstd_rand generator(std::random_device{}());
    static std::uniform_real_distribution<> distribution(-1,1);
    return distribution(generator);
}

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
    for(cm_pos y = m_subspace.y0; y < m_subspace.y1; y++)
    for(cm_pos z = m_subspace.z0; z < m_subspace.z1; z++)
    for(cm_pos x = m_subspace.x0; x < m_subspace.x1; x++)
    {
        //if( m_domain(x,y,z).state != Domain::VOID.state ) continue;
        f_vec pos = {static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)};

        for(cm_pos dy = m_domain.neighbourhood.dy0; dy <= m_domain.neighbourhood.dy1; dy++)
        for(cm_pos dz = m_domain.neighbourhood.dz0; dz <= m_domain.neighbourhood.dz1; dz++)
        for(cm_pos dx = m_domain.neighbourhood.dx0; dx <= m_domain.neighbourhood.dx1; dx++)
        {
            cell c = input.state(pos.x + dx, pos.y + dy, pos.z + dz);
            if(c == Domain::VOID) continue;

            Grain&  grain = m_grains[c.state];

            double time = dt(sqrt(dx*dx + dy*dy + dz*dz), pos, grain) + c.time;

            if(time < input(x,y,z).time && time <= ct)
            {
               output(x,y,z) = {c.state, time};
            }
        }
    }
}

double Generator::dt(double d, f_vec pos, const Grain& grain)
{
    double f = dotProduct(grain.growth_tensor, m_prefered_orientation);

    f_vec rpv = { pos.x - grain.center.x,  pos.y - grain.center.y, pos.z - grain.center.z};
    //double r = crossProduct(rpv, grain.growth_tensor).norm()/rpv.norm();
    double r = abs(dotProduct(rpv, grain.growth_tensor))/rpv.norm();

    return d/(r*f);
}

