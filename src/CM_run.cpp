#include<omp.h>
#include"CM_run.hpp"
#include"CM_logs.hpp"

void GenerationManager::generate_single_layer(Configuration& config)
{
    m_nucleator.nucleate(config);
    create_generators(config);
    dimX = config.domain->dimX;
    dimY = config.domain->dimY;
    dimZ = config.domain->dimZ;
    #pragma omp parallel default(none) num_threads(config.threadsNum) firstprivate(dimX,dimY,dimZ)
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

void GenerationManager::create_generators(Configuration& config)
{
    int threadsNumber = config.threadsNum;

    cm_pos dx = (config.domain->dimX + threadsNumber - 1)/threadsNumber;

    for(cm_pos x = 0; x < config.domain->dimX; x+=dx)
    {
        Generator::Subspace subspace;

        subspace.x0 = x;
        subspace.x1 = ((x + dx) < config.domain->dimX)? x + dx: config.domain->dimX;
        subspace.y0 = 1;
        subspace.y1 = 2;
        subspace.z0 = 0;
        subspace.z1 = config.domain->dimZ; 

        m_generators.emplace_back(*config.domain, subspace, m_nucleator.grains());
    }
}