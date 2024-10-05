#include<iostream>
#include<cmath>
#include<algorithm>
#include<memory>
#include<ctime>
#include<random>
#include<Eigen/Dense>
#include<Eigen/Geometry>
#include"setup.hpp"


void Nucleator::nucleate(Domain& domain, _long_int y0, _int g0, const Configuration::Layer& layer)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> pos_dist(0, 1);
    std::normal_distribution<> angle_dist(0.0, layer.tilt_stddev);

    Eigen::Vector3d prefered_orientation = {layer.prefered_orientation.x, layer.prefered_orientation.y, layer.prefered_orientation.z};
    Eigen::Vector3d axis_a = prefered_orientation.cross(Eigen::Vector3d{1,0,0}).normalized();
    Eigen::Vector3d axis_b = prefered_orientation.cross(axis_a).normalized();

    uint16_t grain_ID = g0;
    
    _long_int dimX = domain.dimX;
    _long_int dimZ = domain.dimZ;

    for(_long_int grain_ID = g0; grain_ID < g0 + layer.grainsNumber; grain_ID++)
    {
        m_grains[grain_ID].ID = grain_ID;
        double x = pos_dist(gen) * dimX;
        double z = pos_dist(gen) * dimZ;
        /* center */
        if( x >= dimX ) x = dimX - 1;
        if( z >= dimZ ) z = dimZ - 1;
        m_grains.insert({grain_ID, Grain()});
        for(_long_int y = y0; ;)
        {
            if( domain(x,y,z).state == Domain::VOID.state && domain(x,y-1,z).state != Domain::VOID.state)
            {
                m_grains[grain_ID].center = {round( x), double(y), round( z)};
                domain(x,y,z) = {m_grains[grain_ID].ID,0};
                break;
            }
            else if(domain(x,y,z).state == Domain::VOID.state)
            {
                y -= 1;
            }
            else
            {
                y += 1;
            }
        }
            
            /* Growth tensor */
    Eigen::AngleAxisd rotation_a(angle_dist(gen), axis_a);
    Eigen::Vector3d rotated_a = rotation_a * prefered_orientation;

    Eigen::AngleAxisd rotation_b(angle_dist(gen), axis_b);
    Eigen::Vector3d rotated_b = rotation_b * prefered_orientation;

    Eigen::Vector3d orientation = (rotated_a + rotated_b).normalized();
        
    m_grains[grain_ID].orientation = {orientation[0], orientation[1], orientation[2]};
            
    }
}



