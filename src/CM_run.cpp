#include<omp.h>
#include"CM_run.hpp"
#include"CM_logs.hpp"

GenerationManager::GenerationManager(cm_pos dimX, cm_pos dimY, cm_pos dimZ, Neighbourhood neighbourhood):
    m_domain(dimX, dimY, dimZ, neighbourhood)
{
    m_threads_number = omp_get_num_threads();
}

void GenerationManager::generate_single_layer(cm_int layer)
{
    m_nucleator.nucleate(m_domain, m_layers_properties[layer]);
    create_generators();
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

    cm_pos dx = (m_domain.dimX + threadsNumber - 1)/threadsNumber;

    for(cm_pos x = 0; x < m_domain.dimX; x+=dx)
    {
        Generator::Subspace subspace;

        subspace.x0 = x;
        subspace.x1 = ((x + dx) < m_domain.dimX)? x + dx: m_domain.dimX;
        subspace.y0 = 1;
        subspace.y1 = 2;
        subspace.z0 = 0;
        subspace.z1 = m_domain.dimZ; 

        m_generators.emplace_back(m_domain, subspace, m_nucleator.grains());
    }
}