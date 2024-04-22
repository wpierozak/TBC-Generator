#include<iostream>
#include"CM_mstypes.hpp"

Grain::Grain(const Grain& other) :
    ID(other.ID),
    center(other.center),
    growth_tensor(other.growth_tensor),
    r_vector(other.r_vector),
    angle_of_widen(other.angle_of_widen),
    ref_length(other.ref_length),
    h0_norm_smooth_region(other.h0_norm_smooth_region),
    h0_norm_feathered_region(other.h0_norm_feathered_region),
    h0_norm_top_region(other.h0_norm_top_region),
    s_profile(other.s_profile),  // Can be a shallow copy if function pointer doesn't manage resources
    s_param(other.s_param),
    f_profile(other.f_profile),  // Can be a shallow copy if function pointer doesn't manage resources
    f_param(other.f_param),
    t_profile(other.t_profile),  // Can be a shallow copy if function pointer doesn't manage resources
    t_param(other.t_param),
    ref_column_rad(other.ref_column_rad),
    max_column_rad(other.max_column_rad)
{
    // Deep copy of vector fields
    s_param = other.s_param;
    f_param = other.f_param;
    t_param = other.t_param;
}
