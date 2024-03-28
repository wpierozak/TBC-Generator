#pragma once

#include<list>
#include<vector>
#include"CM_datatypes.hpp"



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

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ);

/* Function sets all cells to a default value and performs a nucleation */
void nucleation(GeneratorConfig& domain);

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, Microstructure_Properties& msp);

/* Defines reference bound for column width by bounding the angle between growth tensor and relative position vector*/
void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp);

void setSmoothRegionLen(Grain& grain, Microstructure_Properties& msp);

void setFeatheredRegionLen(Grain& grain, Microstructure_Properties& msp);

void setTopRegionLen(Grain& grain, Microstructure_Properties& msp);

void setReferenceRadius(Grain& grain, Microstructure_Properties& msp);

/* Calculates the P parameter */
double calculateP(double phi, const Grain& grain);

/* Calculates the R parameter */
double calculateR(f_vec RPV, const Grain& grain);

f_vec calculateH0(f_vec& position, const Grain& grain);

/*
    Grain generation process:
    -> move along g direction up to the next layer (y + 1)
    -> for each cell within square with side equals to max_column_radius
        -> calculate correspondig H0
            ->calculate H0 = c - cos(rpv, gt) * gt 
        -> calculate distance to H0
        -> if dist(cell, H0) < ref_radius + angular_widen(H0)
            -> accept cell
            -> continue ->
        -> calculate H0 norm
        -> indentify region: smooth, feathered, top
        -> for smooth region
            -> reject
            -> continue
        -> for feathered region:
            -> if shape_function(cell, H0) == true
                -> accept cell
                -> continue ->
            -> else
                -> reject
                -> continue ->
        -> for top region:
            -> if shape_function(cell, H0) == true
                -> accept cell
                -> continue ->
            -> else
                -> reject
                -> continue ->
*/

void growGrain(const Grain& grain);