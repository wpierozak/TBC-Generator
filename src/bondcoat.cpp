#include"bondcoat.hpp"
#include"domain.hpp"
#include<cmath>


void BondCoat::fill(Domain& domain)
{
    float max_y = 0;
    for(float& parameter: m_parameters){
        max_y += parameter;
    }

    for(float y = 0; y < max_y; y += 1)
    {
    for(float z = 0; z < static_cast<float>(domain.dimZ); z += 1)
    {
    for(float x = 0; x < static_cast<float>(domain.dimX); x += 1)
    {
        if(y <= shape_function(x,z))
        {
            domain(x,y,z) = Domain::BOND;
        }
    }// x    
    }// y
    }// z

}

