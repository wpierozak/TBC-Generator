#include"CM_basictypes.hpp"

void normalize(f_vec &vec)
{
    double v_norm = vec.norm();
    vec.x = vec.x / v_norm;
    vec.y = vec.y / v_norm;
    vec.z = vec.z / v_norm;
}
