#pragma once
#include"CM_config.hpp"
#include<unordered_map>

class Nucleator
{
    public:
    Nucleator() = default;

    void nucleate(Domain& domain, cm_pos y0, cm_int g0, Layer& msp);
    std::unordered_map<uint16_t, Grain>& grains() {return m_grains;}

    private:

    void setGrowthTensor(Grain& grain, Layer& msp);

    std::unordered_map<uint16_t, Grain> m_grains;
};