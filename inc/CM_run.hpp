#pragma once

#include"CM_config.hpp"
#include"CM_generation.hpp"
#include"CM_setup.hpp"

class GenerationManager
{
    public:
    GenerationManager(Configuration& config);
    GenerationManager(_long_int dimX, _long_int dimY, _long_int dimZ, Neighbourhood neighbourhood);
    void generate_layer(_int layer);
    void generate();

    void add_layer(Layer& msp) { m_layers.emplace_back(msp);}
    Domain& domain() { return m_domain; }
    void set_threads_number(_int threads) {m_threads_number=threads;}

    private:
    void create_generators();
    void update_generators(_int layer, _int g0, _int y0);
    _long_int calc_y0(_int layer, _int y0, _int g0);

    std::vector<Layer> m_layers;

    Domain m_domain;
    
    _int m_threads_number;

    Nucleator m_nucleator;
    std::vector<Generator> m_generators;
};
