#include<iostream>
#include"CM_mstypes.hpp"

void Grain::print() const
{
 std::cout << "Grain Properties:" << std::endl;
  std::cout << "\tID: " << ID << std::endl;
  std::cout << "\tCenter: (" << center.x << ", " << center.y << ", " << center.z << ")" << std::endl;
  std::cout << "\tGrowth Tensor: (" << growth_tensor.x << ", " << growth_tensor.y << ", " << growth_tensor.z << ")" << std::endl;
  std::cout << "\tCos Phi Upper Bound: " << angle_of_widen << std::endl;
  std::cout << "\tRPV Norm Upper Bound: " << ref_length << std::endl;
  std::cout << "\th0 Norm Bounds:" << std::endl;
  std::cout << "\t\tSmooth Region: " << h0_norm_smooth_region << std::endl;
  std::cout << "\t\tFeathered Region: " << h0_norm_feathered_region << std::endl;
  std::cout << "\t\tTop Region: " << h0_norm_top_region << std::endl;
  std::cout << "\tReference Column Radius: " << ref_column_rad << std::endl;
  std::cout << "\tMaximum Column Radius: " << max_column_rad << std::endl;
}

void Microstructure_Properties::print() const
{
    std::cout << "Microstructure Properties:" << std::endl;
    std::cout << "\tMaximum Angle of Widening: " << max_angle_of_widen << std::endl;
    std::cout << "\tMaximum Tilt (YZ, XY Plane): " << max_tilt << std::endl;
    std::cout << "\tMinimum Tilt (YZ, XY Plane): " << min_tilt << std::endl;
    std::cout << "\tSmooth Region Length (%): " << smooth_region_length << std::endl;
    std::cout << "\tSmooth Region Length Variance (%): " << smooth_region_length_var << std::endl;
    std::cout << "\tFeathered Region Length (%): " << feathered_region_length << std::endl;
    std::cout << "\tFeathered Region Length Variance (%): " << feathered_region_length_var << std::endl;
    std::cout << "\tTop Region Length (%): " << top_region_length << std::endl;
    std::cout << "\tTop Region Length Variance (%): " << top_region_length_var << std::endl;
    std::cout << "\tMinimum Column Length: " << min_length << std::endl;
    std::cout << "\tMaximum Column Length: " << max_length << std::endl;
    std::cout << "\tMaximum Reference Radius: " << max_reference_radius << std::endl;
}