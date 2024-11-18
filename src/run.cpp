#include<omp.h>
#include"run.hpp"

#include <iostream>
#include <cmath>

void printProgressBar(float dt, float time) {
    int barWidth = 50; // Width of the progress bar
    float progress = dt / time;

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
    m_domain(config.space.dimX, config.space.dimY, config.space.dimZ),
    m_config(config)
{
    _int idx = 0;
    for(_long_int dy = -1; dy <= 1; dy++)
    for(_long_int dz = -1; dz <= 1; dz++)
    for(_long_int dx = -1; dx <= 1; dx++)
    {
        m_kLen[idx] = sqrt(dx*dx + dy*dy + dz*dz);
        idx++;
    }
} 

void GenerationManager::generate_layer(_int layer, float y0_p, float y1_p)
{
    std::cout<< "Layer: " << layer << std::endl;
    Domain copy(m_domain);

    float y0 = y0_p;
    float y1 = y1_p;

    bool flip = true;

    float t_stop = m_config.layers[layer].layer_height/(1.0 + 1.0/m_config.layers[layer].dk + m_config.layers[layer].diff);

    #pragma omp parallel num_threads(m_config.parallel.cpu_threads) default(shared) firstprivate(flip, t_stop)
    {
         
        _int idx = omp_get_thread_num();
        _long_int dz = (m_domain.dimZ + omp_get_num_threads() - 1)/omp_get_num_threads();
        _long_int z0 = dz*idx;
        _long_int z1 = ((z0+dz*(idx+1)) < m_domain.dimZ) ? z0 + dz*(idx+1) : m_domain.dimZ;

        for(float i = 0; i < t_stop; i+=m_config.time.dt)
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
            
            if(flip){
                oneIteration(i, m_domain, copy, layer, round(y0), round(y1), z0, z1);
            }
            else{
                oneIteration(i, copy, m_domain, layer, round(y0), round(y1), z0, z1);
            }
            
            flip != flip;
        
        }
    }

    std::cout << std::endl;
}

void GenerationManager::oneIteration(float ct, Domain& input, Domain& output, _long_int layerIdx, _long_int y0, _long_int y1, _long_int z0, _long_int z1)
{
    const Configuration::Layer& layer = m_config.layers[layerIdx];
    f_vec space = {m_domain.dimX, m_domain.dimY, m_domain.dimZ};

    for(_long_int y = y0; y < y1; y++)
    for(_long_int z = z0; z < z1; z++)
    for(_long_int x = 0; x < input.dimX; x++)
    {
        if( input(x,y,z).state < m_g0 || input(x,y,z).state == Domain::BOND.state ) continue;
        f_vec pos = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};

        float shadowing = 1.0;
        for(_long_int dy = 0; dy <= 1; dy++)
        for(_long_int dz = -1; dz <= 1; dz++)
        for(_long_int dx = -1; dx <= 1; dx++)
        {
            if(input.state(pos.x + dx, pos.y + dy, pos.z + dz).state != Domain::VOID.state)
            {
                float sh = 1.0 - layer.prefered_orientation*f_vec{dx,dy,z};
                if(sh >=  1.0) sh = 1.0;
                if(sh < shadowing) shadowing = sh;
            }  
        }

        _int idx = -1;
        for(_long_int dy = -1; dy <= 1; dy++)
        for(_long_int dz = -1; dz <= 1; dz++)
        for(_long_int dx = -1; dx <= 1; dx++)
        {
            idx++;
            cell c = input.state(pos.x + dx, pos.y + dy, pos.z + dz);
            if((c.state == Domain::VOID.state) || (c.state < m_g0) || (m_domain(x,y,z).state == Domain::BOND.state) || (c.state == Domain::BOND.state)){
                continue;
            }

            Grain&  grain = m_nucleator.grains()[c.state];

            float dt = INFINITY;
            {
                dt = (1.0+layer.diff)*(m_kLen[idx]) /(shadowing*m_vkMatrix[(c.state-m_g0)*27 + idx]+ layer.diff) ;
               
                if(dt <= 0) dt = INFINITY;
            }

            float time = dt + c.time;

            if(time < input(x,y,z).time && time <= ct)
            {
               output(x,y,z) = {c.state, time};
            }
        }
    }
}


void GenerationManager::generate()
{
    BondCoat bond(m_config.bond.parameters);
    bond.fill(m_domain);
    
    float y0 = 0;
    m_g0 = 0;

    for(_int layer = 0; layer < m_config.layers.size(); layer++)
    {
        m_g0 = (layer == 0) ? 0 : m_g0 + m_config.layers[layer-1].grainsNumber;
        auto ys = m_nucleator.nucleate(m_domain, m_domain.dimY-2, m_g0, m_config.layers[layer]);
        float y_max = ys.first;
        y0 = ys.second;


        precalculateLayer(layer);
        calculateVkMatrix(layer);
        std::cout << "\nGeneration\n";
        generate_layer(layer, y0*0.9, y_max+5);
    }
}

void GenerationManager::precalculateLayer(_int layerIdx)
{
    Configuration::Layer& layer = m_config.layers[layerIdx];

    int32_t idx = 0;
    for(int32_t dy = -1; dy <= 1; dy++)
    for(int32_t dz = -1; dz <= 1; dz++)
    for(int32_t dx = -1; dx <= 1; dx++)
    {
        f_vec k{-dx,-dy,-dz};
        k.normalize();
        layer.cosAlphaG[idx] = cos(layer.alpha_g * acos(k*layer.prefered_orientation));
        idx++;
    }
}

void GenerationManager::calculateVkMatrix(_int layerIdx)
{
    Configuration::Layer& layer = m_config.layers[layerIdx];
    if(m_vkMatrix != nullptr){
        delete [] m_vkMatrix;
    }

    m_vkMatrix = new float[layer.grainsNumber*27];
    for(_int grainId = 0; grainId < layer.grainsNumber; grainId++){
        for(_int idx = 0; idx < 27; idx++){
            m_vkMatrix[27*grainId + idx] = layer.cosAlphaG[idx] * ((1.0/layer.dk) + cos(layer.alpha_t*m_nucleator.grains()[grainId].theta[idx]));
        }
    }
}
