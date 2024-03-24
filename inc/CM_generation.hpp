#pragma once

#include<list>
#include<vector>
#include"CM_datatypes.hpp"

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
class Subdomain
{
    public:
    cm_state* domain;
    cm_state* statesBuffer;
    BC boundryCondition;
    Neighbourhood neighbourhood;
    NeighbourhoodPlane baseNeighbourhood;
    Grain* grains;
    cm_size grainNum;
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

    cm_state getState(cm_pos x, cm_pos y, cm_pos z);

    cm_state& accessStatesBuffer(cm_pos x, cm_pos y, cm_pos z)
    {
        return statesBuffer[cm_size(y)*(dimX*dimZ) + cm_size(z)*dimX + cm_size(x)];
    }
};

typedef std::vector<Subdomain> subdomains_array;
void copySubdomainsArray(subdomains_array& dest, subdomains_array& src);

void createSubdomains(GeneratorConfig& config, subdomains_array& subdomains);

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

/* Function sets all cells to a default value and performs a nucleation */
void nucleation(GeneratorConfig& domain);

/* Expands grains bases to fill entire bottom layer of cells */
void fillBase(Subdomain& subdomain);

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain* grain);

/* Defines reference bound for column width by bounding the angle between growth tensor and relative position vector*/
void setColumnWidthBound(Grain* grain);

/* Defines reference bound for a RPV norm */
void setRPVNormBound(Grain* grain);

/* Calculates a relative position vector (RPV) */
cm_pos_vec calculateRPV(const Grain* grain, cm_pos x0, cm_pos y0, cm_pos z0);

/* Calculates a cosine of an angle between GT and RPV */
double cosGTRPV(cm_pos_vec rpv, const Grain* grain);

/* Calculates the P parameter */
double calculateP(double phi, const Grain* grain);

/* Calculates the R parameter */
double calculateR(cm_pos_vec RPV, const Grain* grain);

bool isWithinRadius(cm_pos x, cm_pos y, cm_pos z, f_vec h0, const Grain* grain);

f_vec calculateH0(cm_pos y, const Grain* grain);

double cosCircleRPV(cm_pos x, cm_pos y, cm_pos z, cm_pos_vec rpv, f_vec h0, const Grain* grain);

/* Search for grains in neighbourhood */
void scanNeighbourhood(cm_pos x0, cm_pos y0, cm_pos z0, Subdomain& subdomain, std::list<Grain*>& grain_id);

/* Grain growth */
void growColumns(Subdomain& subdomain);