#pragma once

#include"CM_datatypes.hpp"

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
class Subdomain
{
    public:
    cm_state* inputStates;
    cm_state* outputStates;
    Neighborhood neighborhood;
    cm_pos dimX, dimY, dimZ;
    cm_pos x0, x1;
    cm_pos y0, y1;
    cm_pos z0, z1;

    cm_size getIdx(cm_pos x, cm_pos y, cm_pos z)
    {
        return cm_size(y)*(dimX*dimZ) + cm_size(z)*dimX + cm_size(x);
    }
};

/* Function performs one iteration of a grain growth algorithm within a given subdomain */
void grainGrowth(Subdomain& subdomain);
/* Function tests neighbourhood and sets output state of a given cell */
void tryNeighborhood(const cm_pos cX, const cm_pos cY, const cm_pos cZ, Subdomain& subdomain);
/* Function tests if neighbour is within domain */
inline bool tryIfFit(const cm_pos cX, const cm_pos cY, const cm_pos cZ, cm_smallsize n, Subdomain& subdomain)
{
    return (
        (cX + subdomain.neighborhood.neighbours[n][0] >= 0) && (cX + subdomain.neighborhood.neighbours[n][0] < subdomain.dimX )&&
        (cY + subdomain.neighborhood.neighbours[n][1] >= 0) && (cY + subdomain.neighborhood.neighbours[n][1] < subdomain.dimY ) &&
        (cZ + subdomain.neighborhood.neighbours[n][2] >= 0) && (cZ + subdomain.neighborhood.neighbours[n][2] < subdomain.dimZ )
        );
}
/* Function sets all cells to a default value and performs a nucleation */
void nucleation(Domain& domain);
