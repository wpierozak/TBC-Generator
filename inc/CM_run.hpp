#pragma once

#include"CM_config.hpp"
#include"CM_generation.hpp"
#include"CM_setup.hpp"

class GenerationManager
{
    public:
    GenerationManager(cm_pos dimX, cm_pos dimY, cm_pos dimZ, Neighbourhood neighbourhood);
    void generate_single_layer(cm_int layer);

    void add_layer(Microstructure_Properties& msp) { m_layers_properties.emplace_back(msp);}
    Domain& domain() { return m_domain; }
    void set_threads_number(cm_int threads) {m_threads_number=threads;}

    private:
    void create_generators();

    std::vector<Microstructure_Properties> m_layers_properties;

    Domain m_domain;
    
    cm_int m_threads_number;

    Nucleator m_nucleator;
    std::vector<Generator> m_generators;
};
