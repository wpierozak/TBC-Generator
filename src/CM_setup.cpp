#include<iostream>
#include<cmath>
#include<algorithm>
#include<memory>
#include<ctime>
#include<random>
#include"CM_setup.hpp"
#include"CM_shapefun.hpp"
#include"CM_logs.hpp"

/* Global variables for random number generation in the grain set up process */
std::minstd_rand gl_rand_gen(std::random_device{}());
std::uniform_real_distribution<double> lu_dist(-1, 1);
std::uniform_real_distribution<double> p_dist(0, 1);
std::normal_distribution<double> n_dist(0.7,0.2);


void defineTasks(const Configuration& config, tasks_array& tasks)
{
    int threadsNumber = config.threadsNum;
    tasks.clear();
    tasks.resize(threadsNumber);

    cm_pos dx = (config.domain->dimX + threadsNumber - 1)/ceil(sqrt(threadsNumber));
    cm_pos dz = (config.domain->dimZ + threadsNumber - 1)/floor(sqrt(threadsNumber));
    cm_pos y0 = 1;
    cm_pos y1 = 2;

    cm_int counter = 0;

    for(cm_pos z = 0; z < config.domain->dimZ; z+=dz)
    for(cm_pos x = 0; x < config.domain->dimX; x+=dx)
    {
        tasks[counter].input.domain = std::make_unique<Domain>(*config.domain);

        tasks[counter].input.x0 = x;
        tasks[counter].input.x1 = ((x + dx) < config.domain->dimX)? x + dx: config.domain->dimX;
        tasks[counter].input.y0 = y0;
        tasks[counter].input.y1 = y1;
        tasks[counter].input.z0 = z;
        tasks[counter].input.z1 = ((z + dz) < config.domain->dimZ)? z + dz: config.domain->dimZ; 


        std::copy(config.grains.begin(), config.grains.end(), std::back_insert_iterator(tasks[counter].input.grains));   
        counter++;              
    }
}

std::pair<cm_pos, cm_pos> findDiv(cm_pos dimX, cm_pos dimZ)
{
    std::pair<cm_size, cm_size> res;
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
    cm_size grain_ID = 0;

    auto div = findDiv(dimX, dimZ);
    cm_pos dX = (config.domain->dimX + sqrt(config.grainsNumber) - 1)/ceil(sqrt(config.grainsNumber));
    cm_pos dZ = (config.domain->dimZ + sqrt(config.grainsNumber) - 1)/floor(sqrt(config.grainsNumber));

    for(cm_pos z = 0; z < dimZ; z+=dZ)
        for(cm_pos x = 0; x < dimX; x+=dX)
        {
            if(LogManager::Manager().logmode()) 
            LogManager::Manager().header(std::string("Grain ") + std::to_string(grain_ID));
            /* center */
            config.grains[grain_ID].center = {round(dist(gen)*dX + x), 0.0, round(dist(gen)*dZ + z)};
            if(config.grains[grain_ID].center.x >= dimX) config.grains[grain_ID].center.x = dimX - 1.0;
            if(config.grains[grain_ID].center.z >= dimZ) config.grains[grain_ID].center.z = dimZ - 1.0;
            /* ID */
            config.grains[grain_ID].ID = grain_ID;
            /* Growth tensor */
            setGrowthTensor(config.grains[grain_ID], config.msp);
            setRVector(config.grains[grain_ID]);
            /* cos_phi_ub */
            setMaxWidenAngle(config.grains[grain_ID], config.msp);
            /* h0_norm_smooth_region */
            setSmoothRegionLen(config.grains[grain_ID], config.msp);
            /* h0_norm_feathered_region */
            setFeatheredRegionLen(config.grains[grain_ID], config.msp);
            /* h0_norm_top_region */
            setTopRegionLen(config.grains[grain_ID], config.msp);
            config.grains[grain_ID].h0_norm_top_region = 1.0 - config.grains[grain_ID].h0_norm_smooth_region - config.grains[grain_ID].h0_norm_feathered_region;
            /* ref_column_rad */
            setReferenceRadius(config.grains[grain_ID], config.msp);
            /* ref_length */
            setReferenceLength(config.grains[grain_ID], config.msp);

            for(cm_pos dz = -config.grains[grain_ID].ref_column_rad; config.grains[grain_ID].center.z + dz < dimZ && dz < config.grains[grain_ID].ref_column_rad; dz++)
            for(cm_pos dx = -config.grains[grain_ID].ref_column_rad; config.grains[grain_ID].center.x + dx < dimX && dx < config.grains[grain_ID].ref_column_rad; dx++)
            {
                if(dx*dx + dz*dz > pow(config.grains[grain_ID].ref_column_rad, 2)) continue;
                if(config.grains[grain_ID].center.x + dx < 0 || config.grains[grain_ID].center.x + dx > dimX ||
                    config.grains[grain_ID].center.z + dz < 0 || config.grains[grain_ID].center.z + dz > dimZ) continue; 
                if((*config.domain)(config.grains[grain_ID].center.x + dx, 0, config.grains[grain_ID].center.z + dz) == Domain::VOID)
                (*config.domain)(config.grains[grain_ID].center.x + dx, 0, config.grains[grain_ID].center.z + dz) = grain_ID;
            }

            assignSmoothProfile(config.grains[grain_ID], config.profilesSmooth);
            assignFeatheredProfile(config.grains[grain_ID], config.profilesFeathered);
            assignTopProfile(config.grains[grain_ID], config.profilesTop);

            grain_ID++;
            if(LogManager::Manager().logmode()) LogManager::Manager().printGrainData(config.grains[grain_ID-1]);
            if(grain_ID == config.grainsNumber) break;
        }
    
}

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, const Microstructure_Properties& msp)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> rand_angle(msp.min_tilt * M_PI / 180.0, msp.max_tilt * M_PI / 180.0);

    double xy_angle = rand_angle(gen);
    double zy_angle = rand_angle(gen);

    grain.growth_tensor.x = sin(xy_angle);
    grain.growth_tensor.z = sin(zy_angle);
    grain.growth_tensor.y = 1.0;

    normalize(grain.growth_tensor);
}

/* Defines bound for column width by bounding the angle between growth tensor and relative position vector*/
void setMaxWidenAngle(Grain& grain, const Microstructure_Properties& msp)
{
    grain.angle_of_widen =  msp.max_angle_of_widen * 0.5 * M_PI / 180.0;
}

void setSmoothRegionLen(Grain& grain, const Microstructure_Properties& msp)
{
    grain.h0_norm_smooth_region = msp.smooth_region_length + lu_dist(gl_rand_gen)*msp.smooth_region_length_var;
}

void setFeatheredRegionLen(Grain& grain, const Microstructure_Properties& msp)
{
    grain.h0_norm_feathered_region = msp.feathered_region_length + lu_dist(gl_rand_gen)*msp.feathered_region_length_var;
}

void setTopRegionLen(Grain& grain, const Microstructure_Properties& msp)
{
    grain.h0_norm_top_region = msp.top_region_length + lu_dist(gl_rand_gen)*msp.top_region_length_var;
}

void setReferenceRadius(Grain& grain, const Microstructure_Properties& msp)
{
    grain.ref_column_rad = msp.max_reference_radius*n_dist(gl_rand_gen);
}

void setReferenceLength(Grain& grain, const Microstructure_Properties& msp)
{
    grain.ref_length = (msp.max_length - msp.min_length)*p_dist(gl_rand_gen) + msp.min_length;
}

void correctRefLen(Grain& grain)
{
    grain.ref_length = (grain.h0_norm_feathered_region + grain.h0_norm_smooth_region + grain.h0_norm_top_region);
}

void assignSmoothProfile(Grain& grain, const std::vector<SectionProfile> profiles)
{
    if(profiles.size() == 1)
    {
        grain.s_profile = profiles[0].profile;
        std::copy(profiles[0].coeff.begin(), profiles[0].coeff.end(), std::back_inserter(grain.s_param));
    }
    else
    {
        int idx = p_dist(gl_rand_gen) * profiles.size();
         grain.s_profile = profiles[idx].profile;
        std::copy(profiles[idx].coeff.begin(), profiles[idx].coeff.end(), std::back_inserter(grain.s_param));
    }
}

void assignFeatheredProfile(Grain& grain, const std::vector<SectionProfile> profiles)
{
     if(profiles.size() == 1)
    {
        grain.f_profile = profiles[0].profile;
        std::copy(profiles[0].coeff.begin(), profiles[0].coeff.end(), std::back_inserter(grain.f_param));
    }
    else
    {
        int idx = p_dist(gl_rand_gen) * profiles.size();
         grain.f_profile = profiles[idx].profile;
        std::copy(profiles[idx].coeff.begin(), profiles[idx].coeff.end(), std::back_inserter(grain.f_param));
    }
}

void assignTopProfile(Grain& grain, const std::vector<SectionProfile> profiles)
{
     if(profiles.size() == 1)
    {
        grain.t_profile = profiles[0].profile;
        std::copy(profiles[0].coeff.begin(), profiles[0].coeff.end(), std::back_inserter(grain.t_param));
    }
    else
    {
        int idx = p_dist(gl_rand_gen) * profiles.size();
         grain.t_profile = profiles[idx].profile;
        std::copy(profiles[idx].coeff.begin(), profiles[idx].coeff.end(), std::back_inserter(grain.t_param));
    }
}

void setRVector(Grain& grain)
{
    f_vec i = {1,0,0};
    grain.r_vector = crossProduct(i, grain.growth_tensor);
    normalize(grain.r_vector);
}