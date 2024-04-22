#include<stdexcept>
#include"CM_SFCreator.hpp"

SFCreator::SFCreator()
{
    _profiles.emplace("radius_bound", bsf::smooth::radius_bound);
    _profiles.emplace("radius_bound_w", bsf::smooth::radius_bound_w);
    _profiles.emplace("parabolic_profile", bsf::top::parabolic_profile);
    _profiles.emplace("parabolic_profile_w", bsf::top::parabolic_profile_w);
    _profiles.emplace("feather", bsf::feathered::feather);
    _profiles.emplace("feather_w", bsf::feathered::feather_w);
    _profiles.emplace("multi_feather_w", bsf::feathered::multi_feather_w);
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

sfptr SFCreator::get(std::string profileName)
{
    auto profile = _profiles.find(profileName);
    if(profile == _profiles.end())
        throw std::runtime_error("Profile function not found");
    return profile->second;
}