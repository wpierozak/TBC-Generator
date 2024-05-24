#pragma once
#include<unordered_map>
#include<list>
#include"CM_shapefun.hpp"

class SFCreator
{
    public:
    SFCreator();
    sfptr get(std::string profile);
    void setParameters(std::list<std::string> coef, double* dest, int size);

    private:
    std::unordered_map<std::string, sfptr> _profiles;
};