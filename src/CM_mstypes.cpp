#include<iostream>
#include"CM_mstypes.hpp"

void Microstructure_Properties::print(std::ostream& os) const
{
    os << "--> Length" << std::endl;
    os << *length;
    os << "--> Radius" << std::endl;
    os << *radius; 
    os << "--> Tilt" << std::endl;
    os << *tilt;
    os << "--> Widen" << std::endl;
    os << *widen;
    os << "--> Top fraction" << std::endl;
    os << *top_frac;
    os << "--> Resolution: " << static_cast<int>(resolution) << std::endl;
}