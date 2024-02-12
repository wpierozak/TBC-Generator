#include"CM_generation.hpp"

void grainGrowth(Subdomain& subdomain)
{
    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.inputStates[y*(subdomain.dimX*subdomain.dimZ) + z*subdomain.dimX + x] == EMPTY)
            tryNeighborhood(x,y,z, subdomain);
    }
}

void tryNeighborhood(const cm_pos cX, const cm_pos cY, const cm_pos cZ, Subdomain& subdomain)
{
    for(cm_smallsize n = 0; n < subdomain.neighborhood.size; n++)
    {
        if(!tryIfFit(cX, cY, cZ, n, subdomain)) continue;
        if(rand()%100 > 50)
        {
            subdomain.outputStates[cY*(subdomain.dimX*subdomain.dimZ) + cZ*subdomain.dimX + cX] =
                subdomain.inputStates[(cY + subdomain.neighborhood.neighbours[n][1])*(subdomain.dimX*subdomain.dimZ) + 
                (cZ + subdomain.neighborhood.neighbours[n][2])*subdomain.dimX + cX + subdomain.neighborhood.neighbours[n][0]];
            break;
        }
    }
}

void nucleation(Domain& domain)
{

}

