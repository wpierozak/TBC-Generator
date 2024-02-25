#include<iostream>
#include<ctime>
#include"CM_generation.hpp"

void grainGrowth(Subdomain& subdomain)
{
    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.inputStates[subdomain.getIdx(x,y,z)] == EMPTY)
            tryNeighborhood(x,y,z, subdomain);
    }
}

void tryNeighborhood(const cm_pos cX, const cm_pos cY, const cm_pos cZ, Subdomain& subdomain)
{
    for(cm_smallsize n = 0; n < subdomain.neighborhood.size; n++)
    {
        if(!tryIfFit(cX, cY, cZ, n, subdomain)) continue;
        cm_state neighbour = subdomain.inputStates[subdomain.getIdx(cX + subdomain.neighborhood.neighbours[n][0], 
                cY + subdomain.neighborhood.neighbours[n][1], cZ + subdomain.neighborhood.neighbours[n][2])];
        if(rand()%100 > 20 && neighbour != EMPTY)
        {
            subdomain.outputStates[subdomain.getIdx(cX, cY, cZ)] = neighbour;
            break;
        }
    }
}

void nucleation(Domain& domain)
{
    srand(time(NULL));
    cm_pos nucleuses[3];
    cm_state grainID = 1;
    for(cm_size n = 0; n < domain.getNucleusNum(); n++ )
    {
        nucleuses[0] = std::rand() % domain.getDimX();
        nucleuses[1] = 0;
        nucleuses[2] = std::rand() % domain.getDimZ();
        domain.getAbuffer()[domain.getIdx(nucleuses[0], nucleuses[1], nucleuses[2])] = grainID;
        grainID++;
    }
}

