#pragma once

#include"CM_config.hpp"
#include"CM_generation.hpp"
#include"CM_setup.hpp"

class GenerationManager
{
    public:
    GenerationManager() = default;
    void generate_single_layer(Configuration& config);

    //Domain& domain();

    private:
    void create_generators(Configuration& config);

    cm_pos dimX, dimY, dimZ;

    Nucleator m_nucleator;
    std::vector<Generator> m_generators;
};
void run(Configuration& config);