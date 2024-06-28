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
    std::copy(config.layers.begin(), config.layers.end(), std::back_inserter(m_layers_properties));
} 

void GenerationManager::generate_single_layer(cm_int layer)
{
    cm_pos dimY = m_domain.dimY;
    #pragma omp parallel num_threads(m_threads_number) 
    {
        cm_int idx = omp_get_thread_num();

        while (m_generators[idx].subspace().y0 != dimY)
        {
            #pragma omp barrier
            m_generators[idx].run();
            #pragma omp barrier

            m_generators[idx].subspace().y0 += 1;
            m_generators[idx].subspace().y1 += 1;
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
        subspace.y0 = m_current_y0;
        subspace.y1 = m_current_y0 + 1;
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
        g.subspace().y0 = m_current_y0;
        g.subspace().y1 = m_current_y0 + 1;
    }
}

void GenerationManager::generate()
{
    create_generators();
    for(cm_int layer = 0; layer < m_layers_properties.size(); layer++)
    {
        m_current_layer = layer;
        find_y0();
        m_nucleator.nucleate(m_domain, m_current_y0, m_current_grain_0, m_layers_properties[layer]);
        update_generators();
        propagate_downward();
        generate_single_layer(layer);
        m_current_grain_0 = m_layers_properties[layer].grainsNumber + 1;
    }
}

void GenerationManager::find_y0()
{
    if(m_current_layer == 0) return;

    m_current_y0 =  m_layers_properties[m_current_layer-1].layer_height;
    bool flag = true;
    
    for(cm_pos y = m_layers_properties[m_current_layer-1].layer_height-1; y >= 0; y--)
    {
        #pragma omp parallel for schedule(static) num_threads(m_threads_number) firstprivate(y)
        for(cm_pos z = 0; z < m_domain.dimZ; z++)
        {
            for(cm_pos x = 0; x < m_domain.dimX; x++)
            {
                if(m_domain(x,y,z) != Domain::VOID) flag = false;
            }
        }
        if(flag) m_current_y0 = y;
        else break;
    }
}

void GenerationManager::propagate_downward()
{
    if(m_current_layer == 0) return;
    bool flag = true;

    for(cm_pos y = m_current_y0-1; y >=0; y--)
    {
        flag = true;

        #pragma omp parallel for schedule(static) num_threads(m_threads_number) shared(flag) firstprivate(y)
        for(cm_pos z = 0; z  < m_domain.dimZ; z++ )
        {
            for(cm_pos x = 0; x < m_domain.dimX; x++)
            {
                if(m_domain(x,y,z) == Domain::VOID)
                {
                    if(flag)
                    {
                        #pragma omp critical
                        {
                            flag = false;
                        }
                    }
                    m_domain(x,y,z) = m_domain(x,y+1,z);
                }
            }
        }

        if(flag) break;
    }
}