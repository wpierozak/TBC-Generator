#pragma once
#include"CM_config.hpp"
#include"CM_task.hpp"

class Nucleator
{
    public:
    Nucleator() = default;

    void nucleate(Configuration& config);
    std::vector<Grain>& grains() {return m_grains;}

    private:

    void setGrowthTensor(Grain& grain, Microstructure_Properties& msp);
    void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp);
    void setTopRegionLen(Grain& grain, Microstructure_Properties& msp);
    void setReferenceRadius(Grain& grain, Microstructure_Properties& msp);
    void setReferenceLength(Grain& grain, Microstructure_Properties& msp);
    void setRVector(Grain& grain);

    std::vector<Grain> m_grains;
};