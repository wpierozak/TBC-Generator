#include <iostream>
#include <cstring>
#include "CM_datatypes.hpp"

#define LOGS

Domain::Domain(cm_pos X, cm_pos Y, cm_pos Z, Neighbourhood n):
    dimX(X), dimY(Y), dimZ(Z), neighbourhood(n)
{
    buffer = std::shared_ptr<cm_state[]>(new cm_state[static_cast<cm_size>(dimX)*static_cast<cm_size>(dimZ)*static_cast<cm_size>(dimY)]);
    for(cm_size n = 0; n < static_cast<cm_size>(dimX)*static_cast<cm_size>(dimZ)*static_cast<cm_size>(dimY); n++)
    buffer[n] = Domain::VOID;   
}

cm_state Domain::state(cm_pos x, cm_pos y, cm_pos z) const
{
    if(dimZ > 1)
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY) && (z >= 0 && z < dimZ);
        if(inside) return buffer[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    }

    else
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY);
        if(inside) return buffer[cm_size(y)*(dimX * dimZ) + cm_size(x)];
    }
    
    switch (bc)
    {
    case BC::absorption :
        return EMPTY;
    break;

    case BC::bouncy:
        if(x >= dimX) x = dimX - 1 - (x-dimX);
        else if(x < 0) x = -x - 1;
        
        if(y >= dimY) y = dimY - 1 - (y-dimY);
        else if(y < 0) y = -y - 1;

        if(z >= dimZ) z = dimZ - 1 - (z-dimZ);
        else if(z < 0) z = -z - 1;

        return buffer[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    break;

    case BC::periodic:
        if(x >= dimX) x = x - dimX;
        else if(x < 0) x = dimX + x;

        if(y >= dimY) y = y - dimY;
        else if(y < 0) y = dimY + y;

        if(z >= dimZ) z = z - dimZ;
        else if(z < 0) z = dimZ + z;
        
        return buffer[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    break;

    default:
        throw std::runtime_error("Invalid bc");
        break;
    }
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

void printGrain(const Grain& grain) {
  std::cout << "Grain Properties:" << std::endl;
  std::cout << "  ID: " << grain.ID << std::endl;
  std::cout << "  Center: (" << grain.center.x << ", " << grain.center.y << ", " << grain.center.z << ")" << std::endl;
  std::cout << "  Growth Tensor: (" << grain.growth_tensor.x << ", " << grain.growth_tensor.y << ", " << grain.growth_tensor.z << ")" << std::endl;
  std::cout << "  Cos Phi Upper Bound: " << grain.cos_phi_ub << std::endl;
  std::cout << "  RPV Norm Upper Bound: " << grain.ref_length << std::endl;
  std::cout << "  h0 Norm Bounds:" << std::endl;
  std::cout << "    Smooth Region: " << grain.h0_norm_smooth_region << std::endl;
  std::cout << "    Feathered Region: " << grain.h0_norm_feathered_region << std::endl;
  std::cout << "    Top Region: " << grain.h0_norm_top_region << std::endl;
  // Function pointers and coefficients are omitted for brevity
  std::cout << "  Reference Column Radius: " << grain.ref_column_rad << std::endl;
  std::cout << "  Maximum Column Radius: " << grain.max_column_rad << std::endl;
}

#include <iostream>

void printMicrostructureProperties(const Microstructure_Properties& properties) {
  std::cout << "Microstructure Properties:" << std::endl;
  std::cout << "  Maximum Angle of Widening: " << properties.max_angle_of_widen << std::endl;
  std::cout << "  Maximum Tilt (YZ, XY Plane): " << properties.max_tilt << std::endl;
  std::cout << "  Minimum Tilt (YZ, XY Plane): " << properties.min_tilt << std::endl;
  std::cout << "  Smooth Region Length (%): " << properties.smooth_region_length << std::endl;
  std::cout << "  Smooth Region Length Variance (%): " << properties.smooth_region_length_var << std::endl;
  std::cout << "  Feathered Region Length (%): " << properties.feathered_region_length << std::endl;
  std::cout << "  Feathered Region Length Variance (%): " << properties.feathered_region_length_var << std::endl;
  std::cout << "  Top Region Length (%): " << properties.top_region_length << std::endl;
  std::cout << "  Top Region Length Variance (%): " << properties.top_region_length_var << std::endl;
  std::cout << "  Minimum Column Length: " << properties.min_length << std::endl;
  std::cout << "  Maximum Column Length: " << properties.max_length << std::endl;
  std::cout << "  Maximum Reference Radius: " << properties.max_reference_radius << std::endl;
}
