#include"CM_basictypes.hpp"

void normalize(f_vec &vec)
{
    double v_norm = vec.norm();
    vec.x = vec.x / v_norm;
    vec.y = vec.y / v_norm;
    vec.z = vec.z / v_norm;
}

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