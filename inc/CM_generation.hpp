#pragma once

#include<list>
#include<vector>
#include"CM_datatypes.hpp"

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
class Subdomain
{
    public:
    cm_state* domain;
    BC boundryCondition;

    grains_array grains;
    double maxColumnTilt;
    double minColumnTilt;
    double referenceRadius;
    double maxAngularWidth;
    
    Microstructure_Properties msp;
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
};

typedef std::vector<Subdomain> subdomains_array;

inline f_vec subtractVectors(f_vec v0, f_vec v1)
{
    return {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
}

inline f_vec addVectors(f_vec v0, f_vec v1)
{
    return {v1.x + v0.x, v1.y + v0.y, v1.z + v0.z};
}

inline double cosVectors(f_vec v0, f_vec v1)
{
    return (v0.x*v1.x + v0.y*v1.y + v0.z*v1.z)/(v0.norm()*v1.norm());
}

void copySubdomainsArray(subdomains_array& dest, subdomains_array& src);

void createSubdomains(GeneratorConfig& config, subdomains_array& subdomains);

/* Function sets all cells to a default value and performs a nucleation */
void nucleation(GeneratorConfig& domain);

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, GeneratorConfig& config);

/* Defines reference bound for column width by bounding the angle between growth tensor and relative position vector*/
void setColumnWidthBound(Grain& grain, GeneratorConfig& config);

/* Defines reference bound for a RPV norm */
void setRPVNormBound(Grain& grain, GeneratorConfig& config);

/* Calculates the P parameter */
double calculateP(double phi, const Grain& grain);

/* Calculates the R parameter */
double calculateR(f_vec RPV, const Grain& grain);

f_vec calculateH0(f_vec& position, const Grain& grain);

void assignCell(f_vec position, Subdomain& subdomain);

/* Grain growth */
void growColumns(Subdomain& subdomain);