#include<omp.h>
#include"run.hpp"


GenerationManager::GenerationManager(Configuration& config):
    m_domain(config.space.dimX, config.space.dimY, config.space.dimZ, config.neighbourhood),
    m_config(config)
{
   
} 

void GenerationManager::generate_layer(_int layer)
{
    Domain copy(m_domain);

    for(int i = 0; i < m_config.layers[layer].layer_height; i++)
    {
        #pragma omp parallel num_threads(m_config.parallel.cpu_threads) 
        {
            _int idx = omp_get_thread_num();
            #pragma omp barrier
            if(i % 2 == 0) m_generators[idx].run(m_domain, copy, i);
            else m_generators[idx].run(copy, m_domain, i);
            #pragma omp barrier
        }
    }
}

void GenerationManager::create_generators()
{
    int threadsNumber = m_config.parallel.cpu_threads;

    _long_int dz = (m_domain.dimZ + threadsNumber - 1)/threadsNumber;

    for(_long_int z = 0; z < m_domain.dimZ; z+=dz)
    {
        Generator::Subspace subspace;

        subspace.x0 = 0;
        subspace.x1 = m_domain.dimX;
        subspace.y0 = 0;
        subspace.y1 = m_domain.dimY;
        subspace.z0 = z;
        subspace.z1 = ((z+dz) < m_domain.dimZ) ? z + dz : m_domain.dimZ;

        m_generators.emplace_back(m_domain, subspace);
    }
}

void GenerationManager::update_generators(_int layer, _int g0, _int y0)
{
    for(Generator& g: m_generators)
    {
        g.update_grains(m_nucleator.grains());
        g.set_g0(g0);
        g.set_prefered_orientation(m_config.layers[layer].prefered_orientation);
        g.subspace().y0 = y0;
        g.set_alpha_g(m_config.layers[layer].alpha_g);
        g.set_alpha_t(m_config.layers[layer].alpha_t);
    }
}

void GenerationManager::generate()
{
    BondCoat bond(m_config.bond.parameters);
    bond.fill(m_domain);
    
    create_generators();
    _int g0 = 0;
    _int y0 = 0;

    for(_int layer = 0; layer < m_config.layers.size(); layer++)
    {
        g0 = (layer == 0) ? 0 : g0 + m_config.layers[layer-1].grainsNumber;
        y0 = calc_y0(layer, y0, g0);
        
        m_nucleator.nucleate(m_domain, y0, g0, m_config.layers[layer]);

        update_generators(layer, g0, y0);
        generate_layer(layer);
    }
}

_long_int GenerationManager::calc_y0(_int layer, _int y0, _int g0)
{
    return (layer == 0) ? 0 : y0 + m_config.layers[layer-1].layer_height*0.9;
}