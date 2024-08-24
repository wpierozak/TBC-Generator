#include"CM_bondcoat.hpp"
#include<cmath>


void BondCoat::fill(Domain& domain)
{
    double max_y = std::max_element(m_parameters.begin(), m_parameters.end());

    for(double y = 0; y < max_y; y += 1)
    {
    for(double z = 0; z < static_cast<double>(domain.dimZ); z += 1)
    {
    for(double x = 0; x < static_cast<double>(domain.dimX); x += 1)
    {
        if(y <= shape_function(x,z))
        {
            domain(x,y,z) = Domain::BOND_COAT;
        }
    }// x    
    }// y
    }// z

}

