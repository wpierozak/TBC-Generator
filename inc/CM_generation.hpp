#pragma once

#include"CM_datatypes.hpp"

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
class Subdomain
{
    public:
    cm_state* domain;
    cm_state* statesBuffer;
    Neighbourhood neighbourhood;
    NeighbourhoodPlane baseNeighbourhood;
    cm_pos dimX, dimY, dimZ;
    cm_pos x0, x1;
    cm_pos y0, y1;
    cm_pos z0, z1;

    cm_size getIdx(cm_pos x, cm_pos y, cm_pos z)
    {
        return cm_size(y)*(dimX*dimZ) + cm_size(z)*dimX + cm_size(x);
    }

    cm_state& getCell(cm_pos x, cm_pos y, cm_pos z)
    {
        return domain[cm_size(y)*(dimX*dimZ) + cm_size(z)*dimX + cm_size(x)];
    }

    cm_state& accessStatesBuffer(cm_pos x, cm_pos y, cm_pos z)
    {
        return statesBuffer[cm_size(y)*(dimX*dimZ) + cm_size(z)*dimX + cm_size(x)];
    }
};

/* Function performs one iteration of a grain growth algorithm within a given subdomain */
void grainGrowth(Subdomain& subdomain);

/* Function tests if neighbour is within domain */
inline bool tryIfFit(const cm_pos cX, const cm_pos cY, const cm_pos cZ, cm_pos* dp, Subdomain& subdomain)
{
    return (
        (cX + dp[0] >= 0) && (cX + dp[0] < subdomain.dimX )&&
        (cY + dp[1] >= 0) && (cY + dp[1] < subdomain.dimY ) &&
        (cZ + dp[2] >= 0) && (cZ + dp[2] < subdomain.dimZ )
        );
}

inline bool tryIfFitBase(const cm_pos cX, const cm_pos cY, const cm_pos cZ, cm_smallsize n, Subdomain& subdomain)
{
    return (
        (cX + subdomain.baseNeighbourhood.neighbours[n][0] >= 0) && (cX + subdomain.baseNeighbourhood.neighbours[n][0] < subdomain.dimX )&&
        (cY + subdomain.baseNeighbourhood.neighbours[n][1] >= 0) && (cY + subdomain.baseNeighbourhood.neighbours[n][1] < subdomain.dimY ) &&
        (cZ + subdomain.baseNeighbourhood.neighbours[n][2] >= 0) && (cZ + subdomain.baseNeighbourhood.neighbours[n][2] < subdomain.dimZ )
        );
}
/* Function sets all cells to a default value and performs a nucleation */
void nucleation(GeneratorConfig& domain);

void fillBase(Subdomain& subdomain);