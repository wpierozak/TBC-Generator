#include<iostream>
#include<cmath>
#include<algorithm>
#include<memory>
#include<ctime>
#include<random>
#include"CM_setup.hpp"
#include"CM_logs.hpp"

/* Global variables for random number generation in the grain set up process */
std::minstd_rand gl_rand_gen(std::random_device{}());
std::uniform_real_distribution<double> lu_dist(-1, 1);
std::uniform_real_distribution<double> p_dist(0, 1);
std::normal_distribution<double> n_dist(0.7,0.2);


void defineTasks(Configuration& config, tasks_array& tasks)
{
    int threadsNumber = config.threadsNum;
    tasks.clear();
    tasks.resize(threadsNumber);

    cm_pos dx = (config.domain->dimX + threadsNumber - 1)/threadsNumber;
    cm_pos y0 = 1;
    cm_pos y1 = 2;

    cm_int counter = 0;

    for(cm_pos x = 0; x < config.domain->dimX; x+=dx)
    {
        tasks[counter].domain = std::make_unique<Domain>(*config.domain);

        tasks[counter].x0 = x;
        tasks[counter].x1 = ((x + dx) < config.domain->dimX)? x + dx: config.domain->dimX;
        tasks[counter].y0 = y0;
        tasks[counter].y1 = y1;
        tasks[counter].z0 = 0;
        tasks[counter].z1 = config.domain->dimZ; 


        std::copy(config.grains.begin(), config.grains.end(), std::back_insert_iterator(tasks[counter].grains));   
        counter++;              
    }
}

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ)
{
    std::pair<cm_pos, cm_pos> res;
    cm_pos field = dimX*dimZ;
    cm_pos min_diff = field+1;
    for(cm_pos n = 1; n < field; n++)
    {
        if(field % n != 0) continue;
        cm_pos r = field/n;
        if(abs(r-n) < min_diff)
        {
            res.first = r;
            res.second = n;
            min_diff = abs(r-n);
        }
    }
    return res;
}

void nucleation(Configuration& config)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);

    cm_pos dimX = config.domain->dimX;
    cm_pos dimZ = config.domain->dimZ;

    config.grains.resize(config.grainsNumber);
    cm_pos grain_ID = 0;

    auto div = findDiv(dimX, dimZ);
    cm_pos dX = (config.domain->dimX + sqrt(config.grainsNumber) - 1)/ceil(sqrt(config.grainsNumber));
    cm_pos dZ = (config.domain->dimZ + sqrt(config.grainsNumber) - 1)/floor(sqrt(config.grainsNumber));

    for(cm_pos z = 0; z < dimZ; z+=dZ)
        for(cm_pos x = 0; x < dimX; x+=dX)
        {
            /* center */
            config.grains[grain_ID].center = {round( x), 0.0, round( z)};
            if(config.grains[grain_ID].center.x >= dimX) config.grains[grain_ID].center.x = dimX - 1.0;
            if(config.grains[grain_ID].center.z >= dimZ) config.grains[grain_ID].center.z = dimZ - 1.0;
            /* ID */
            config.grains[grain_ID].ID = grain_ID;
            /* Growth tensor */
            setGrowthTensor(config.grains[grain_ID], config.msp);
            setRVector(config.grains[grain_ID]);
             /* ref_length */
            setReferenceLength(config.grains[grain_ID], config.msp);
            /* cos_phi_ub */
            setMaxWidenAngle(config.grains[grain_ID], config.msp);
            /* h0_norm_top_region */
            setTopRegionLen(config.grains[grain_ID], config.msp);
            /* ref_column_rad */
            setReferenceRadius(config.grains[grain_ID], config.msp);
           

            config.grains[grain_ID].resolution = config.msp.resolution;

            config.grains[grain_ID].tan_angle_of_widen = tan(config.grains[grain_ID].angle_of_widen);
            config.grains[grain_ID].y_to_norm2 = pow(config.grains[grain_ID].growth_tensor.y/config.grains[grain_ID].growth_tensor.norm(),2);
            
            grain_ID++;
            #ifdef DEBUG
            if(LogManager::Manager().logmode()) LogManager::Manager().printGrainData(config.grains[grain_ID-1]);
            #endif 
            if(grain_ID == config.grainsNumber) break;
        }

    std::vector<int> idxs;
    for(int i = 0; i < config.grainsNumber; i++) idxs.push_back(i);
    std::minstd_rand minstd(std::random_device{}());
    std::shuffle(idxs.begin(), idxs.end(), minstd);
    for(int grain_ID: idxs)
    {
        for(cm_pos dz = -config.grains[grain_ID].ref_column_rad; config.grains[grain_ID].center.z + dz < dimZ && dz < config.grains[grain_ID].ref_column_rad; dz++)
        for(cm_pos dx = -config.grains[grain_ID].ref_column_rad; config.grains[grain_ID].center.x + dx < dimX && dx < config.grains[grain_ID].ref_column_rad; dx++)
            {
                if(dx*dx + dz*dz > pow(config.grains[grain_ID].ref_column_rad, 2)) continue;
                if(config.grains[grain_ID].center.x + dx < 0 || config.grains[grain_ID].center.x + dx > dimX ||
                    config.grains[grain_ID].center.z + dz < 0 || config.grains[grain_ID].center.z + dz > dimZ) continue; 
                if((*config.domain)(config.grains[grain_ID].center.x + dx, 0, config.grains[grain_ID].center.z + dz) == Domain::VOID)
                (*config.domain)(config.grains[grain_ID].center.x + dx, 0, config.grains[grain_ID].center.z + dz) = grain_ID;
            }
    }
}

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, Microstructure_Properties& msp)
{
    double xy_angle = radians(msp.tilt->draw());
    double zy_angle = radians(msp.tilt->draw());

    grain.growth_tensor.x = sin(xy_angle);
    grain.growth_tensor.z = sin(zy_angle);
    grain.growth_tensor.y = 1.0;

    normalize(grain.growth_tensor);
}

/* Defines bound for column width by bounding the angle between growth tensor and relative position vector*/
void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp)
{
    grain.angle_of_widen = radians(msp.widen->draw());
}


void setTopRegionLen(Grain& grain, Microstructure_Properties& msp)
{
    grain.top_fraction = msp.top_frac->draw();
    grain.top_parabola_coeff = -0.5;
}

void setReferenceRadius(Grain& grain, Microstructure_Properties& msp)
{
    grain.ref_column_rad = msp.radius->draw();
}

void setReferenceLength(Grain& grain, Microstructure_Properties& msp)
{
    grain.ref_length = msp.length->draw();
}

void setRVector(Grain& grain)
{
    f_vec i = {1,0,0};
    grain.r_vector = crossProduct(i, grain.growth_tensor);
    normalize(grain.r_vector);
}