#include<iostream>
#include"CM_mstypes.hpp"

Grain::Grain(const Grain& other) :
    ID(other.ID),
    center(other.center),
    growth_tensor(other.growth_tensor),
    angle_of_widen(other.angle_of_widen),
    ref_length(other.ref_length),
    h0_norm_smooth_region(other.h0_norm_smooth_region),
    h0_norm_feathered_region(other.h0_norm_feathered_region),
    h0_norm_top_region(other.h0_norm_top_region),
    smooth_section_function(other.smooth_section_function),  // Can be a shallow copy if function pointer doesn't manage resources
    smooth_section_function_coeff(other.smooth_section_function_coeff),
    feathered_section_function(other.feathered_section_function),  // Can be a shallow copy if function pointer doesn't manage resources
    feathered_section_function_coeff(other.feathered_section_function_coeff),
    top_section_function(other.top_section_function),  // Can be a shallow copy if function pointer doesn't manage resources
    top_section_function_param(other.top_section_function_param),
    ref_column_rad(other.ref_column_rad),
    max_column_rad(other.max_column_rad)
{
    // Deep copy of vector fields
    smooth_section_function_coeff = other.smooth_section_function_coeff;
    feathered_section_function_coeff = other.feathered_section_function_coeff;
    top_section_function_param = other.top_section_function_param;
}
