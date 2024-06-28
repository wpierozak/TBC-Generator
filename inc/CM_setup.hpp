#pragma once
#include"CM_config.hpp"
#include<unordered_map>

class Nucleator
{
    public:
    Nucleator() = default;

    void nucleate(Domain& domain, cm_pos y0, cm_int g0, Microstructure_Properties& msp);
    std::unordered_map<cm_state, Grain>& grains() {return m_grains;}

    private:

    void setGrowthTensor(Grain& grain, Microstructure_Properties& msp);
    void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp);
    void setTopRegionLen(Grain& grain, Microstructure_Properties& msp);
    void setReferenceRadius(Grain& grain, Microstructure_Properties& msp);
    void setReferenceLength(Grain& grain, Microstructure_Properties& msp);
    void setRVector(Grain& grain);

    std::unordered_map<cm_state, Grain> m_grains;
};