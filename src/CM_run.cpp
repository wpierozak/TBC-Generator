#include<omp.h>
#include"CM_run.hpp"
#include"CM_logs.hpp"


GenerationManager::GenerationManager(Configuration& config):
    m_domain(config.dimX, config.dimY, config.dimZ, config.neighbourhood)
{
    m_threads_number = config.threadsNum;
    m_current_layer = 0;
    m_current_y0 =0;
    m_current_grain_0 = 0;
    for(auto& l : config.layers)
    {
        m_layers_properties.push_back(l);
    }
    //std::copy(config.layers.begin(), config.layers.end(), std::back_inserter(m_layers_properties));
} 

void GenerationManager::generate_single_layer(cm_int layer)
{
    cm_pos dimY = m_domain.dimY;
    Domain copy(m_domain);

    for(int i = 0; i < m_layers_properties[layer].layer_height; i++)
    {
        #pragma omp parallel num_threads(m_threads_number) 
        {
            cm_int idx = omp_get_thread_num();
            #pragma omp barrier
            if(i % 2 == 0) m_generators[idx].run(m_domain, copy, i);
            else m_generators[idx].run(copy, m_domain, i);
            #pragma omp barrier
        }
    }
}

void GenerationManager::create_generators()
{
    int threadsNumber = m_threads_number;

    cm_pos dz = (m_domain.dimZ + threadsNumber - 1)/threadsNumber;

    for(cm_pos z = 0; z < m_domain.dimZ; z+=dz)
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

void GenerationManager::update_generators()
{
    for(Generator& g: m_generators)
    {
        g.update_grains(m_nucleator.grains());
        g.set_prefered_orientation(m_layers_properties[m_current_layer].prefered_orientation);
        g.subspace().y0 = m_current_y0;
    }
}

void GenerationManager::generate()
{
    create_generators();
    for(cm_int layer = 0; layer < m_layers_properties.size(); layer++)
    {
        m_current_layer = layer;
        m_current_y0 = 0;
        m_nucleator.nucleate(m_domain, m_current_y0, m_current_grain_0, m_layers_properties[layer]);

        update_generators();
        generate_single_layer(layer);

        m_current_grain_0 = m_layers_properties[layer].grainsNumber + 1;
    }
}

