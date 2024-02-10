#include"CM_datatypes.hpp"

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
struct Subdomain
{
    Field* domain;
    Neighborhood neighborhood;
    cm_pos x0, x1;
    cm_pos y0, y1;
    cm_pos z0, z1;

};

/* Function performs one iteration of a grain growth algorithm within a given subdomain */
void grainGrowth(Subdomain subdomain);
/* Function sets all cells to a default value and performs a nucleation */
void nucleation(Domain& domain);