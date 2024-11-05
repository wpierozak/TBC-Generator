#pragma once
#include"config.hpp"
#include"grain.hpp"
#include<unordered_map>

class Nucleator
{
    public:
    Nucleator() = default;

    std::pair<double,double> nucleate(Domain& domain, _long_int y0, _int g0, const Configuration::Layer& layer);
    std::unordered_map<uint16_t, Grain>& grains() {return m_grains;}

    private:
    std::unordered_map<uint16_t, Grain> m_grains;
};