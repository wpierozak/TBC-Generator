#include<iostream>
#include<cmath>
#include<ctime>
#include<random>
#include"CM_generation.hpp"
#include"CM_parameters.hpp"

void f()
{
    
}

cm_state Subdomain::getState(cm_pos x, cm_pos y, cm_pos z)
{
    if(dimZ > 1)
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY) && (z >= 0 && z < dimZ);
        if(inside) return domain[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    }

    else
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY);
        if(inside) return domain[cm_size(y)*(dimX * dimZ) + cm_size(x)];
    }
    
    switch (boundryCondition)
    {
    case BC::absorption :
        return EMPTY;
    break;

    case BC::bouncy:
        if(x >= dimX) x = dimX - 1 - (x-dimX);
        else if(x < 0) x = -x - 1;
        
        if(y >= dimY) y = dimY - 1 - (y-dimY);
        else if(y < 0) y = -y - 1;

        if(z >= dimZ) z = dimZ - 1 - (z-dimZ);
        else if(z < 0) z = -z - 1;

        return domain[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    break;

    case BC::periodic:
        if(x >= dimX) x = x - dimX;
        else if(x < 0) x = dimX + x;

        if(y >= dimY) y = y - dimY;
        else if(y < 0) y = dimY + y;

        if(z >= dimZ) z = z - dimZ;
        else if(z < 0) z = dimZ + z;
        
        return domain[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    break;

    default:
        throw std::runtime_error("Invalid bc");
        break;
    }
}

void grainGrowth(Subdomain& subdomain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> eta_dist(ETA_LOW, ETA_UP);
    std::normal_distribution<float> phi_dist(PHI_MEAN, PHI_STD_VAR);
    std::uniform_real_distribution<> radius_dist(RADIUS_LOW, RADIUS_UP);

    float dr[3];
    cm_pos dp[3];

    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.getCell(x,y,z) == EMPTY)
        {
            while(true)
            {
                dr[PHI] = subdomain.neighbourhood.alpha + phi_dist(gen)*subdomain.neighbourhood.beta;
                dr[ETA] = 2.0 * M_PIf * eta_dist(gen);
                dr[RADIUS] = subdomain.neighbourhood.r * radius_dist(gen);

                dp[1] = round(-sin(dr[PHI])*dr[RADIUS]);
                dp[0] = round(cos(dr[PHI])*cos(dr[ETA])*dr[RADIUS]);
                dp[2] = round(cos(dr[PHI])*sin(dr[ETA])*dr[RADIUS]);

                subdomain.accessStatesBuffer(x,y,z) = subdomain.getState(x + dp[0], y + dp[1], z + dp[2]);
                break;
            }
        }
    }
}

void nucleation(GeneratorConfig& domain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);
    
    cm_pos dimX = domain.getDimX();
    cm_pos dimZ = domain.getDimZ();
    float radius = domain.getBaseRadius();
    cm_pos nucleuses[3];

    for(cm_size n = 0; n < domain.getNucleusNum(); n++ )
    {
        nucleuses[0] = dist(gen)* dimX;
        nucleuses[1] = 0;
        nucleuses[2] = dist(gen)* dimZ;
        for(cm_pos dz = 0; nucleuses[2] + dz < dimZ && dz < radius; dz++)
        for(cm_pos dx = 0; nucleuses[0] + dx < dimX && dx < radius; dx++)
        {
            if(dx*dx + dz*dz > radius*radius) continue;
            if(domain.getCell(nucleuses[0], nucleuses[1], nucleuses[2]) == EMPTY)
                domain.getCell(nucleuses[0], nucleuses[1], nucleuses[2]) = n+1;
        }
    }
}

void fillBase(Subdomain& subdomain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> eta_dist(0, 2.0 * M_PIf);
    std::uniform_real_distribution<> radius_dist(0, subdomain.baseNeighbourhood.r);

    float dr[2];
    cm_pos dp[3];
    dp[1] = 0;

    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.getCell(x,0,z) == EMPTY)
        {
            while(true)
            {
                dr[0] = 2.0 * M_PIf * eta_dist(gen);
                dr[1] = subdomain.neighbourhood.r * radius_dist(gen);
  
                dp[0] = round(cos(dr[0])*dr[1]);
                dp[2] = round(sin(dr[0])*dr[1]);

                if(!tryIfFit(x, 0, z, dp, subdomain)) continue;
                subdomain.accessStatesBuffer(x,0,z) = subdomain.getCell(x + dp[0], 
                0, z + dp[2]);
                break;
            }
        }
    }
}
