#include"CM_basictypes.hpp"


std::ostream& operator<<(std::ostream& os, const GaussianDistr gd)
{
    os << "Mean: " << gd.mean() << std::endl;
    os << "Standatd deviation: " << gd.std() << std::endl;
    if(gd.max() == gd.min())
    {
        os << "Min. value and Max. value are not specified" << std::endl;
    }
    else
    {
        os <<"Max. value: " << gd.max() << std::endl;
        os <<"Min. value: " << gd.min() << std::endl;
    }
    return os;
}