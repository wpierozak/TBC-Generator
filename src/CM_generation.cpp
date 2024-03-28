#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include"CM_generation.hpp"
#include"CM_parameters.hpp"

/* Global variables for random number generation in the grain set up process */
std::minstd_rand gl_rand_gen(std::random_device{}());
std::uniform_real_distribution<double> lu_dist(-1, 1);
std::uniform_real_distribution<double> p_dist(0, 1);
std::normal_distribution<double> n_dist(0.5,0.25);


void createSubdomains(GeneratorConfig& config, subdomains_array& subdomains)
{
    int threadsNumber = config.getThreadsNumber();
    subdomains.resize(threadsNumber);

    int counter = 0;

    for(int counter = 0; counter < threadsNumber; counter++)
    {
        subdomains[counter].dimX = config.getDimX();
        subdomains[counter].dimY = config.getDimY();
        subdomains[counter].dimZ = config.getDimZ();

        std::copy(config.getGrains().begin(), config.getGrains().end(), std::back_insert_iterator(subdomains[counter].grains));   
        counter++;              
    }
}

void copySubdomainsArray(subdomains_array& dest, subdomains_array& src)
{
    std::copy(src.begin(), src.end(), std::back_insert_iterator(dest));
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

void nucleation(GeneratorConfig& domain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);
    
    cm_pos dimX = domain.getDimX();
    cm_pos dimZ = domain.getDimZ();

    grains_array grains;
    grains.resize(domain.getGrainsNum());
    cm_size n = 0;

    auto div = findDiv(dimX, dimZ);
    double dX = static_cast<double>(dimX/div.first);
    double dZ = static_cast<double>(dimZ/div.second);

    for(double z = 0; z < dimZ; z+=dZ)
        for(double x = 0; x < dimX; x+=dX)
        {
            /* center */
            grains[n].center = {round(dist(gen)*dX + x), 0.0, round(dist(gen)*dZ + z)};
            if(grains[n].center.x >= dimX) grains[n].center.x = dimX - 1.0;
            if(grains[n].center.z >= dimZ) grains[n].center.z = dimZ - 1.0;
            /* ID */
            grains[n].ID = n+1;
            /* Growth tensor */
            setGrowthTensor(grains[n], domain.getMSP());
            /* cos_phi_ub */
            setMaxWidenAngle(grains[n], domain.getMSP());
            /* h0_norm_smooth_region */
            setSmoothRegionLen(grains[n], domain.getMSP());
            /* h0_norm_feathered_region */
            setFeatheredRegionLen(grains[n], domain.getMSP());
            /* h0_norm_top_region */
            setTopRegionLen(grains[n], domain.getMSP());
            /* ref_column_rad */
            setReferenceRadius(grains[n], domain.getMSP());
        }

    domain.setGrainsConfiguration(grains);
}

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, Microstructure_Properties& msp)
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
void setMaxWidenAngle(Grain& grain, Microstructure_Properties& msp)
{
    grain.cos_phi_ub = cos((180.0 - msp.max_angle_of_widen * 0.5) * M_PI / 180.0);
}

void setSmoothRegionLen(Grain& grain, Microstructure_Properties& msp)
{
    grain.h0_norm_smooth_region = msp.smooth_region_length + lu_dist(gl_rand_gen)*msp.smooth_region_length_var;
}

void setFeatheredRegionLen(Grain& grain, Microstructure_Properties& msp)
{
    grain.h0_norm_feathered_region = msp.feathered_region_length + lu_dist(gl_rand_gen)*msp.feathered_region_length_var;
}

void setTopRegionLen(Grain& grain, Microstructure_Properties& msp)
{
    grain.h0_norm_top_region = msp.top_region_length + lu_dist(gl_rand_gen)*msp.top_region_length_var;
}

void setReferenceRadius(Grain& grain, Microstructure_Properties& msp)
{
    grain.ref_column_rad = msp.max_reference_radius*n_dist(gl_rand_gen);
}

/* Calculates a cosine of an angle between GT and RPV */
double cosGTRPV(cm_pos_vec rpv, const Grain& grain)
{
    return (double(rpv.x)*grain.growth_tensor.x + double(rpv.y)*grain.growth_tensor.y + double(rpv.z)*grain.growth_tensor.z) / 
        ( rpv.norm() * grain.growth_tensor.norm());
}


void growGrain(const Subdomain& subdomain, const Grain& grain)
{
    f_vec cy = grain.center;
    const f_vec& growth_tensor = grain.growth_tensor;

    while(cy.y < subdomain.dimY)
    {
        for(cm_pos z = -grain.max_column_rad/2; z < grain.max_column_rad/2; z++)
        for(cm_pos x = -grain.max_column_rad/2; x < grain.max_column_rad/2; x++)
        {
            f_vec rpv = { static_cast<double>(x) - grain.center.x,  cy.y - grain.center.y, static_cast<double>(z) - grain.center.z};

            double h = (growth_tensor.x * rpv.x + growth_tensor.y * rpv.y + growth_tensor.z * rpv.z);
            double r = crossProduct(rpv, growth_tensor).norm();
            
            if( h <= grain.h0_norm_smooth_region)
            {
                if(grain.smooth_region_function(h,r, grain.smooth_region_function_coeff))
                {

                }
            }
            else if( h <= grain.h0_norm_feathered_region)
            {
                if(grain.feathered_region_function(h, r, grain.feathered_region_function_coeff))
                {

                }
            }
            else if( h <= grain.h0_norm_top_region)
            {
                if(grain.top_region_function(h, r, grain.top_region_function_param))
                {

                }
            }
        }

        cy = add(cy, grain.growth_tensor * (1.0/grain.growth_tensor.y));
    }
}