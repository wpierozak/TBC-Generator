#pragma once
#include"CM_config.hpp"
#include<unordered_map>

class Nucleator
{
    public:
    Nucleator() = default;

    void nucleate(Domain& domain, _long_int y0, _int g0, Layer& msp);
    std::unordered_map<uint16_t, Grain>& grains() {return m_grains;}

    private:
    std::unordered_map<uint16_t, Grain> m_grains;
};