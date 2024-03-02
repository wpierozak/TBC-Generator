#include<iostream>
#include<ctime>
#include<random>
#include"CM_generation.hpp"

void grainGrowth(Subdomain& subdomain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> dist(0, 1);
    cm_smallsize n;
    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.domain[subdomain.getIdx(x,y,z)] == EMPTY)
        {
            while(true)
            {
                n = subdomain.neighbourhood.size * dist(gen);
                if(n < 0) n = 0;
                else if(n >= subdomain.neighbourhood.size) n = subdomain.neighbourhood.size - 1;
                if(!tryIfFit(x, y, z, n, subdomain)) continue;
                subdomain.statesBuffer[subdomain.getIdx(x,y,z)] = subdomain.domain[subdomain.getIdx(x + subdomain.neighbourhood.neighbours[n][0], 
                y + subdomain.neighbourhood.neighbours[n][1], z + subdomain.neighbourhood.neighbours[n][2])];
                break;
            }
        }
    }
}

void tryNeighbourhood(const cm_pos cX, const cm_pos cY, const cm_pos cZ, Subdomain& subdomain)
{
    cm_state neighbour;
    for(cm_smallsize n = 0; n < subdomain.neighbourhood.size; n++)
    {
        if(!tryIfFit(cX, cY, cZ, n, subdomain)) continue;
        neighbour = subdomain.domain[subdomain.getIdx(cX + subdomain.neighbourhood.neighbours[n][0], 
                cY + subdomain.neighbourhood.neighbours[n][1], cZ + subdomain.neighbourhood.neighbours[n][2])];
        if(std::rand()%100 > 20 && neighbour != EMPTY)
        {
            subdomain.statesBuffer[subdomain.getIdx(cX, cY, cZ)] = neighbour;
            break;
        }
    }
}

void nucleation(GeneratorConfig& domain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);
    cm_state* buffer = domain.getDomain();
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
            if(buffer[domain.getIdx(nucleuses[0], nucleuses[1], nucleuses[2])] == EMPTY)
                buffer[domain.getIdx(nucleuses[0], nucleuses[1], nucleuses[2])] = n+1;
        }
    }
}

void fillBase(Subdomain& subdomain)
{
    cm_smallsize n;
    cm_pos** neighbours = subdomain.baseNeighbourhood.neighbours;
    cm_size size = subdomain.neighbourhood.size;

    std::minstd_rand gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, size - 1);

    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.domain[subdomain.getIdx(x,0,z)] == EMPTY)
        {
            while(true)
            {
                n = dist(gen);
                if(n < 0) n = 0;
                else if(n >= size) n = size - 1;
                if(!tryIfFit(x, 0, z, n, subdomain)) continue;
                subdomain.statesBuffer[subdomain.getIdx(x,0,z)] = subdomain.domain[subdomain.getIdx(x + neighbours[n][0], 
                0 + neighbours[n][1], z + neighbours[n][2])];
                break;
            }
        }
    }
}
