#pragma once
#include<unordered_map>
#include<list>
#include"CM_shapefun.hpp"

class SFCreator
{
    public:
    static void initialize();
    static sfptr get(std::string profile);
    static void setParameters(std::list<std::string> coef, double* dest, int size);

    private:
    static SFCreator creator;
    SFCreator() = default;

    std::unordered_map<std::string, sfptr> _profiles;
};