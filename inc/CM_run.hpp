#pragma once

#include"CM_config.hpp"
#include"CM_generation.hpp"
#include"CM_setup.hpp"

class GenerationManager
{
    public:
    GenerationManager(Configuration& config);
    GenerationManager(cm_pos dimX, cm_pos dimY, cm_pos dimZ, Neighbourhood neighbourhood);
    void generate_single_layer(cm_int layer);
    void generate();

    void add_layer(Layer& msp) { m_layers_properties.emplace_back(msp);}
    Domain& domain() { return m_domain; }
    void set_threads_number(cm_int threads) {m_threads_number=threads;}

    private:
    void create_generators();
    void update_generators();
    void find_y0();
    void propagate_downward();

    std::vector<Layer> m_layers_properties;

    Domain m_domain;
    
    cm_int m_threads_number;
    cm_int m_current_layer;
    cm_int m_current_y0;
    cm_int m_current_grain_0;

    Nucleator m_nucleator;
    std::vector<Generator> m_generators;
};
