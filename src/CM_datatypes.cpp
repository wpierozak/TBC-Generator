#include <iostream>
#include <cstring>
#include "CM_datatypes.hpp"

#define LOGS

Domain::Domain(cm_pos X, cm_pos Y, cm_pos Z, Neighbourhood n):
    dimX(X), dimY(Y), dimZ(Z), neighbourhood(n)
{
    buffer = std::shared_ptr<cm_state[]>(new cm_state(static_cast<cm_size>(dimX)*static_cast<cm_size>(dimZ)*static_cast<cm_size>(dimY)));   
}

void normalize(f_vec &vec)
{
    double v_norm = vec.norm();
    vec.x = vec.x / v_norm;
    vec.y = vec.y / v_norm;
    vec.z = vec.z / v_norm;
}


void copy(Grain &dest, const Grain &src)
{
    dest = src;
}