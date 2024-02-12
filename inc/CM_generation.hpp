#include"CM_datatypes.hpp"

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
struct Subdomain
{
    cm_state* inputStates;
    cm_state* outputStates;
    Neighborhood neighborhood;
    cm_pos dimX, dimY, dimZ;
    cm_pos x0, x1;
    cm_pos y0, y1;
    cm_pos z0, z1;
};

/* Function performs one iteration of a grain growth algorithm within a given subdomain */
void grainGrowth(Subdomain& subdomain);
/* Function tests neighbourhood and sets output state of a given cell */
void tryNeighborhood(const cm_pos cX, const cm_pos cY, const cm_pos cZ, Subdomain& subdomain);
/* Function tests if neighbour is within domain */
inline bool tryIfFit(const cm_pos cX, const cm_pos cY, const cm_pos cZ, cm_smallsize n, Subdomain& subdomain)
{
    return (cX + subdomain.neighborhood.neighbours[n][0] >= subdomain.x0 && cX + subdomain.neighborhood.neighbours[n][0] < subdomain.x1 &&
        cX + subdomain.neighborhood.neighbours[n][0] >= subdomain.x0 && cX + subdomain.neighborhood.neighbours[n][0] < subdomain.x1 &&
        cY + subdomain.neighborhood.neighbours[n][1] >= subdomain.y0 && cY + subdomain.neighborhood.neighbours[n][1] < subdomain.y1 &&
        cZ + subdomain.neighborhood.neighbours[n][2] >= subdomain.z0 && cZ + subdomain.neighborhood.neighbours[n][2] < subdomain.z1);
}
/* Function sets all cells to a default value and performs a nucleation */
void nucleation(Domain& domain);
