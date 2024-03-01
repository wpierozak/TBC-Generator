#include<iostream>
#include<ctime>
#include<random>
#include"CM_generation.hpp"

void grainGrowth(Subdomain& subdomain)
{
    std::minstd_rand gen(std::random_device{}());
    std::normal_distribution<double> dist(0.5, 0.1);
    cm_smallsize n;
    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.inputStates[subdomain.getIdx(x,y,z)] == EMPTY)
        {
            while(true)
            {
                n = subdomain.neighbourhood.size * dist(gen);
                if(n < 0) n = 0;
                else if(n >= subdomain.neighbourhood.size) n = subdomain.neighbourhood.size - 1;
                if(!tryIfFit(x, y, z, n, subdomain)) continue;
                subdomain.outputStates[subdomain.getIdx(x,y,z)] = subdomain.inputStates[subdomain.getIdx(x + subdomain.neighbourhood.neighbours[n][0], 
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
        neighbour = subdomain.inputStates[subdomain.getIdx(cX + subdomain.neighbourhood.neighbours[n][0], 
                cY + subdomain.neighbourhood.neighbours[n][1], cZ + subdomain.neighbourhood.neighbours[n][2])];
        if(std::rand()%100 > 20 && neighbour != EMPTY)
        {
            subdomain.outputStates[subdomain.getIdx(cX, cY, cZ)] = neighbour;
            break;
        }
    }
}

void nucleation(Domain& domain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);

    cm_pos nucleuses[3];
    for(cm_size n = 0; n < domain.getNucleusNum(); n++ )
    {
        nucleuses[0] = dist(gen)* domain.getDimX();
        nucleuses[1] = 0;
        nucleuses[2] = dist(gen)* domain.getDimZ();
        domain.getAbuffer()[domain.getIdx(nucleuses[0], nucleuses[1], nucleuses[2])] = n+1;
    }
}

