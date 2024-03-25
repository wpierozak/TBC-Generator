#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<random>
#include"CM_generation.hpp"
#include"CM_parameters.hpp"

void createSubdomains(GeneratorConfig& config, subdomains_array& subdomains)
{
    int threadsNumber = config.getThreadsNumber();
    subdomains.resize(threadsNumber);
    cm_pos dx = (config.getDimX() + threadsNumber - 1)/ceil(sqrt(threadsNumber));
    cm_pos dz = (config.getDimZ() + threadsNumber - 1)/floor(sqrt(threadsNumber));
    cm_pos y0 = 0;
    cm_pos y1 = config.getDimY();

    int counter = 0;

    for(cm_pos z = 0; z < config.getDimZ(); z+=dz)
    for(cm_pos x = 0; x < config.getDimX(); x+=dx)
    {
        subdomains[counter].dimX = config.getDimX();
        subdomains[counter].dimY = config.getDimY();
        subdomains[counter].dimZ = config.getDimZ();

        subdomains[counter].boundryCondition = config.getBC();

        subdomains[counter].domain = config.getDomain();

        subdomains[counter].x0 = x;
        subdomains[counter].x1 = ((x + dx) < config.getDimX())? x + dx: config.getDimX();
        subdomains[counter].y0 = y0;
        subdomains[counter].y1 = y1;
        subdomains[counter].z0 = z;
        subdomains[counter].z1 = ((z + dz) < config.getDimZ())? z + dz: config.getDimZ();   

        std::copy(config.getGrains().begin(), config.getGrains().end(), std::back_insert_iterator(subdomains[counter].grains));   
        subdomains[counter].maxColumnTilt = config.getMaxTilt();
        subdomains[counter].minColumnTilt = config.getMinTilt(); 
        subdomains[counter].referenceRadius = config.getRefRadius();
        subdomains[counter].maxAngularWidth = config.getMaxAngularWidth();

        counter++;              
    }
}

void copySubdomainsArray(subdomains_array& dest, subdomains_array& src)
{
    std::copy(src.begin(), src.end(), std::back_insert_iterator(dest));
}

cm_state Subdomain::getState(cm_pos x, cm_pos y, cm_pos z)
{
    if(dimZ > 1)
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY) && (z >= 0 && z < dimZ);
        if(inside) return domain[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    }

    else
    {
        bool inside = (x >= 0 && x < dimX) && (y >= 0 && y < dimY);
        if(inside) return domain[cm_size(y)*(dimX * dimZ) + cm_size(x)];
    }
    
    switch (boundryCondition)
    {
    case BC::absorption :
        return EMPTY;
    break;

    case BC::bouncy:
        if(x >= dimX) x = dimX - 1 - (x-dimX);
        else if(x < 0) x = -x - 1;
        
        if(y >= dimY) y = dimY - 1 - (y-dimY);
        else if(y < 0) y = -y - 1;

        if(z >= dimZ) z = dimZ - 1 - (z-dimZ);
        else if(z < 0) z = -z - 1;

        return domain[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    break;

    case BC::periodic:
        if(x >= dimX) x = x - dimX;
        else if(x < 0) x = dimX + x;

        if(y >= dimY) y = y - dimY;
        else if(y < 0) y = dimY + y;

        if(z >= dimZ) z = z - dimZ;
        else if(z < 0) z = dimZ + z;
        
        return domain[cm_size(y)*(dimX * dimZ) + cm_size(z)*dimX + cm_size(x)];
    break;

    default:
        throw std::runtime_error("Invalid bc");
        break;
    }
}

void nucleation(GeneratorConfig& domain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(0, 1);
    
    cm_pos dimX = domain.getDimX();
    cm_pos dimZ = domain.getDimZ();
    double radius = domain.getBaseRadius();
    cm_pos nucleuses[3];

    grains_array grains;
    grains.resize(domain.getGrainsNum());

    for(cm_size n = 0; n < domain.getGrainsNum(); n++ )
    {
        nucleuses[0] = dist(gen)* dimX;
        nucleuses[1] = 0;
        nucleuses[2] = dist(gen)* dimZ;

        for(cm_pos dz = 0; nucleuses[2] + dz < dimZ && dz < radius; dz++)
        for(cm_pos dx = 0; nucleuses[0] + dx < dimX && dx < radius; dx++)
        {
            if(dx*dx + dz*dz > radius*radius) continue;
            if(domain.getCell(nucleuses[0], nucleuses[1], nucleuses[2]) == EMPTY)
                domain.getCell(nucleuses[0], nucleuses[1], nucleuses[2]) = n+1;
        }

        grains[n].center = {static_cast<double>(nucleuses[0]), static_cast<double>(nucleuses[1]), static_cast<double>(nucleuses[2])};
        grains[n].ID = n+1;

        setGrowthTensor(grains[n], domain);
        setRPVNormBound(grains[n], domain);
        setColumnWidthBound(grains[n], domain);
    }

    domain.setGrainsConfiguration(grains);
}

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain& grain, GeneratorConfig& config)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> rand_angle(config.getMinTilt() * M_PI / 180.0, config.getMaxTilt() * M_PI / 180.0);

    double xy_angle = rand_angle(gen);
    double zy_angle = rand_angle(gen);

    grain.growth_tensor.x = sin(xy_angle);
    grain.growth_tensor.z = sin(zy_angle);
    grain.growth_tensor.y = 1.0;

    normalize(grain.growth_tensor);
}

/* Defines bound for column width by bounding the angle between growth tensor and relative position vector*/
void setColumnWidthBound(Grain& grain, GeneratorConfig& config)
{
    grain.cos_phi_ub = cos((180.0 - config.getMaxAngularWidth() * 0.5) * M_PI / 180.0);
}

/* Defines reference bound for a RPV norm */
void setRPVNormBound(Grain& grain, GeneratorConfig& config)
{
    const double BOUNDING_NORM = std::rand()%config.getDimY()*0.2 + config.getDimY()*0.8;
    grain.rpv_norm_ub = BOUNDING_NORM;
}

/* Calculates a cosine of an angle between GT and RPV */
double cosGTRPV(cm_pos_vec rpv, const Grain& grain)
{
    return (double(rpv.x)*grain.growth_tensor.x + double(rpv.y)*grain.growth_tensor.y + double(rpv.z)*grain.growth_tensor.z) / 
        ( rpv.norm() * grain.growth_tensor.norm());
}

/* Calculates the P parameter */
double calculateP(double cos_phi, const Grain& grain)
{
    if(cos_phi > grain.cos_phi_ub) return 0.0;
    else return 1.0;
}

/* Calculates the R parameter */
double calculateR(f_vec RPV, const Grain& grain)
{
    if(RPV.norm() > grain.rpv_norm_ub) return 0.0;
    else return 1.0;
}


f_vec calculateH0(f_vec& pos, const Grain& grain)
{
    return {grain.growth_tensor.x * pos.y/grain.growth_tensor.y + grain.center.x, pos.y, grain.growth_tensor.z * pos.y/grain.growth_tensor.y + grain.center.z};
}

void assignCell(f_vec position, Subdomain& subdomain)
{
    for(Grain grain: subdomain.grains)
    {
        f_vec rpv = subtractVectors(position, grain.center);
        f_vec h0 = calculateH0(position, grain);
            
        double param_p = 1.0;

        f_vec mv = subtractVectors(position, h0);
        if(mv.norm() > subdomain.referenceRadius)
        {
            normalize(mv);
            f_vec mrpv = rpv;
            mrpv.x -= mv.x * subdomain.referenceRadius;
            mrpv.z -= mv.z * subdomain.referenceRadius;
            param_p = calculateP(cosVectors(mrpv, grain.growth_tensor), grain);
        }
            
        double param_r = calculateR(rpv, grain);

        if((param_p * param_r) != 0)
        {
            subdomain.getCell(position.x, position.y, position.z) = grain.ID;
            break;
        }

    }
}

void growColumns(Subdomain& subdomain)
{
    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        assignCell({static_cast<double>(x),static_cast<double>(y), static_cast<double>(z)}, subdomain);
    }
}