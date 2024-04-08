#include"CM_SFCreator.hpp"

void SFCreator::initialize()
{
    creator._profiles.emplace("radius_bound", bsf::smooth::radius_bound);
    creator._profiles.emplace("radius_bound_w", bsf::smooth::radius_bound_w);
    creator._profiles.emplace("parabolic_profile", bsf::top::parabolic_profile);
    creator._profiles.emplace("parabolic_profile_w", bsf::top::parabolic_profile_w);
}

void SFCreator::setParameters(std::list<std::string> coef, double* dest, int size)
{
    if(coef.size() > size) std::runtime_error("Too many parameters!");
    auto cc = coef.begin();
    for(int pos = 0; pos < coef.size(); pos++)
    {
        dest[pos] = std::stod(*cc);
        cc++;
    }
}

sfptr SFCreator::get(std::string profile)
{
    return (*creator._profiles.find(profile)).second;
}