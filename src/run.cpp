#include<omp.h>
#include"run.hpp"

#include <iostream>
#include <cmath>

void printProgressBar(double dt, double time) {
    int barWidth = 50; // Width of the progress bar
    double progress = dt / time;

    // Ensure progress does not exceed 100%
    if (progress > 1.0) progress = 1.0;

    int pos = static_cast<int>(barWidth * progress);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0) << " %\r";
    std::cout.flush();
}


GenerationManager::GenerationManager(Configuration& config):
    m_domain(config.space.dimX, config.space.dimY, config.space.dimZ, config.neighbourhood),
    m_config(config)
{
   
} 

void GenerationManager::generate_layer(_int layer, double y0_p, double y1_p)
{
    std::cout<< "Layer: " << layer << std::endl;
    Domain copy(m_domain);

    double y0 = y0_p;
    double y1 = y1_p;

    bool flip = true;

    double t_stop = m_config.layers[layer].layer_height/(1.0 + 1.0/m_config.layers[layer].dk + m_config.layers[layer].diff);

    #pragma omp parallel num_threads(m_config.parallel.cpu_threads) shared(y0,y1)
    {
        _int idx = omp_get_thread_num();
        for(double i = 0; i < t_stop; i+=m_config.time.dt)
        {
            #pragma omp master
            {
                printProgressBar(i, t_stop);
                y0 += m_config.front.vb*m_config.time.dt;
                if(y1 < m_domain.dimY - 1){
                    y1 += 1.0;
                }
            }

            #pragma omp barrier
            m_generators[idx].subspace().y0 = round(y0);
            m_generators[idx].subspace().y1 = round(y1);
            #pragma omp barrier

            if(flip) m_generators[idx].run(m_domain, copy, i);
            else m_generators[idx].run(copy, m_domain, i);
            !flip;
            #pragma omp barrier
        
        }
    }

    std::cout << std::endl;
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

void GenerationManager::update_generators(_int layer, _int g0)
{
    for(Generator& g: m_generators)
    {
        g.update_grains(m_nucleator.grains());
        g.set_g0(g0);

        g.set_prefered_orientation(m_config.layers[layer].prefered_orientation);
        g.set_alpha_g(m_config.layers[layer].alpha_g);
        g.set_alpha_t(m_config.layers[layer].alpha_t);
        g.set_diffusion(m_config.layers[layer].diff);
        g.set_inv_dk(m_config.layers[layer].dk);
    }
}

void GenerationManager::generate()
{
    BondCoat bond(m_config.bond.parameters);
    bond.fill(m_domain);
    
    create_generators();
    _int g0 = 0;
    double y0 = 0;

    for(_int layer = 0; layer < m_config.layers.size(); layer++)
    {
        g0 = (layer == 0) ? 0 : g0 + m_config.layers[layer-1].grainsNumber;
        y0 = calc_y0(layer, y0, g0);
        
        auto ys = m_nucleator.nucleate(m_domain, y0, g0, m_config.layers[layer]);
        double y_max = ys.first;
        y0 = ys.second;

        update_generators(layer, g0);
        generate_layer(layer, y0, y_max);
    }
}

_long_int GenerationManager::calc_y0(_int layer, _int y0, _int g0)
{
    return (layer == 0) ? 0 : y0 + m_config.layers[layer-1].layer_height;
}