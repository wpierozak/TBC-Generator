#pragma once

#include"config.hpp"
#include"setup.hpp"

class GenerationManager
{
    public:
    GenerationManager(Configuration& config);
    GenerationManager(_long_int dimX, _long_int dimY, _long_int dimZ);

    void generate_layer(_int layer, double y0_p, double y1_p);
    void generate();

    Domain& domain() { return m_domain; }

    private:

    void create_generators();
    void update_generators(_int layer, _int g0);
    void precalculateLayer(_int layer);
    void calculateVkMatrix(_int layer);

    void oneIteration(double ct, Domain& input, Domain& output, _long_int layer, _long_int y0, _long_int y1,_long_int z0, _long_int z1);

    double* m_vkMatrix{nullptr};
    double m_kLen[27];

    _long_int calc_y0(_int layer, _int y0, _int g0);

    Configuration& m_config;

    Domain m_domain;
    Nucleator m_nucleator;
    _long_int m_g0;
};
