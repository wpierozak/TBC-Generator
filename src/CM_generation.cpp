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
        copy(subdomains[counter].neighbourhood, config.getNeighbourhood());
        subdomains[counter].baseNeighbourhood = config.getBaseNeighbourhood();
        subdomains[counter].boundryCondition = config.getBC();
        subdomains[counter].domain = config.getDomain();
        subdomains[counter].statesBuffer = config.getStatesBuffer();
        subdomains[counter].x0 = x;
        subdomains[counter].x1 = ((x + dx) < config.getDimX())? x + dx: config.getDimX();
        subdomains[counter].y0 = y0;
        subdomains[counter].y1 = y1;
        subdomains[counter].z0 = z;
        subdomains[counter].z1 = ((z + dz) < config.getDimZ())? z + dz: config.getDimZ();   
        subdomains[counter].grains = new Grain[config.getNucleusNum()];
        for(int i = 0; i < config.getNucleusNum(); i++)
            subdomains[counter].grains[i] = config.getGrains()[i];
        subdomains[counter].grainNum = config.getNucleusNum();
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

void grainGrowth(Subdomain& subdomain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> eta_dist(ETA_LOW, ETA_UP);
    std::normal_distribution<double> phi_dist(PHI_MEAN, PHI_STD_VAR);
    std::uniform_real_distribution<> radius_dist(RADIUS_LOW, RADIUS_UP);

    double dr[3];
    cm_pos dp[3];

    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.getCell(x,y,z) == EMPTY)
        {
            while(true)
            {
                dr[PHI] = subdomain.neighbourhood.alpha + phi_dist(gen)*(M_PI - 2*subdomain.neighbourhood.alpha);
                dr[ETA] = 2.0 * M_PIf * eta_dist(gen);
                dr[RADIUS] = subdomain.neighbourhood.r * radius_dist(gen);

                dp[1] = round(-sin(dr[PHI])*dr[RADIUS]);
                dp[0] = round(cos(dr[PHI])*cos(dr[ETA])*dr[RADIUS]);
                dp[2] = round(cos(dr[PHI])*sin(dr[ETA])*dr[RADIUS]);
            
                subdomain.accessStatesBuffer(x,y,z) = subdomain.getState(x + dp[0], y + dp[1], z + dp[2]);
                break;
            }
        }
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

    Grain* grains = new Grain[domain.getNucleusNum()];

    for(cm_size n = 0; n < domain.getNucleusNum(); n++ )
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

        std::cout << nucleuses[0] << ' ' << nucleuses[1] << ' ' << nucleuses[2] << '\n';
        grains[n].center = {nucleuses[0], nucleuses[1], nucleuses[2]};
        grains[n].ID = n+1;
        setGrowthTensor(&grains[n]);
        std::cout << grains[n].growth_tensor.x << ' ' << grains[n].growth_tensor.y << ' ' << grains[n].growth_tensor.z << '\n';
        setRPVNormBound(&grains[n]);
        setColumnWidthBound(&grains[n]);
    }

    domain.setGrainsConfiguration(grains);
}

void fillBase(Subdomain& subdomain)
{
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> eta_dist(0, 2.0 * M_PIf);
    std::uniform_real_distribution<> radius_dist(0, subdomain.baseNeighbourhood.r);

    double dr[2];
    cm_pos dp[3];
    dp[1] = 0;

    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.getCell(x,0,z) == EMPTY)
        {
            while(true)
            {
                dr[0] = 2.0 * M_PIf * eta_dist(gen);
                dr[1] = radius_dist(gen);
  
                dp[0] = round(cos(dr[0])*dr[1]);
                dp[2] = round(sin(dr[0])*dr[1]);

                if(!tryIfFit(x + dp[0], 0, z + dp[2], dp, subdomain)) continue;
                subdomain.accessStatesBuffer(x,0,z) = subdomain.getCell(x + dp[0], 
                0, z + dp[2]);
                break;
            }
        }
    }
}

/* Defines grow tensor with regarding in-code parameters */
void setGrowthTensor(Grain* grain)
{
    const double BOUNDING_ANGLE = 5.0 * M_PI / 180.0;
    std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<> rand_angle(0, BOUNDING_ANGLE);

    double xy_angle = rand_angle(gen);
    double zy_angle = rand_angle(gen);

    grain->growth_tensor.x = sin(xy_angle);
    grain->growth_tensor.z = sin(zy_angle);
    grain->growth_tensor.y = 1;

    normalize(grain->growth_tensor);
}

/* Defines bound for column width by bounding the angle between growth tensor and relative position vector*/
void setColumnWidthBound(Grain* grain)
{
    const double BOUNDING_ANGLE = 178.0 * M_PI / 180.0;
    grain->cos_phi_ub = cos(BOUNDING_ANGLE);
}

/* Defines reference bound for a RPV norm */
void setRPVNormBound(Grain* grain)
{
    const double BOUNDING_NORM = 100;
    grain->rpv_norm_ub = BOUNDING_NORM;
}

/* Calculates a relative position vector (RPV) */
cm_pos_vec calculateRPV(const Grain* grain, cm_pos x0, cm_pos y0, cm_pos z0)
{
    cm_pos_vec rpv = {grain->center.x - x0,  grain->center.y - y0, grain->center.z - z0 };
    return rpv;
}

/* Calculates a cosine of an angle between GT and RPV */
double cosGTRPV(cm_pos_vec rpv, const Grain* grain)
{
    return (double(rpv.x)*grain->growth_tensor.x + double(rpv.y)*grain->growth_tensor.y + double(rpv.z)*grain->growth_tensor.z) / 
        ( rpv.norm() * grain->growth_tensor.norm());
}

/* Calculates the P parameter */
double calculateP(double cos_phi, const Grain* grain)
{
    if(cos_phi > grain->cos_phi_ub) return 0.0f;
    else return 1.0f;
}

/* Calculates the R parameter */
double calculateR(cm_pos_vec RPV, const Grain* grain)
{
    if(RPV.norm() > grain->rpv_norm_ub) return 0.0f;
    else return 1.0f;
}

/* Search for grains in neighbourhood */
void scanNeighbourhood(cm_pos x0, cm_pos y0, cm_pos z0, Subdomain& subdomain, std::list<Grain*>& grain_id)
{
    for(int n = 0; n < subdomain.neighbourhood.size; n++)
    {
        /*cm_state neighbour = subdomain.getState(x0 + subdomain.neighbourhood.relative_pos[n*3 + 0],
                                                y0 + subdomain.neighbourhood.relative_pos[n*3 + 1],
                                                z0 + subdomain.neighbourhood.relative_pos[n*3 + 2]);
        if(neighbour != EMPTY) */
        grain_id.push_back(&subdomain.grains[n]);
    }
}

bool isWithinRadius(cm_pos x, cm_pos y, cm_pos z, f_vec h0, const Grain* grain)
{
    const double REF_RADIUS = 10.0;
    f_vec dist = {double(x) - h0.x, 0, double(z) - h0.z };
    if(dist.norm() <= REF_RADIUS) return true;
    else return false;
}

f_vec calculateH0(cm_pos y, const Grain* grain)
{
    return {grain->growth_tensor.x * double(y)/grain->growth_tensor.y + grain->center.x, y, grain->growth_tensor.z * double(y)/grain->growth_tensor.y + grain->center.z};
}

double cosCircleRPV(cm_pos x, cm_pos y, cm_pos z, cm_pos_vec rpv, f_vec h0, const Grain* grain)
{
    const double REF_RADIUS = 10.0;
    f_vec mv = {h0.x - double(x), 0, h0.z - double(z) };
    normalize(mv);
    mv.x = mv.x * REF_RADIUS;
    mv.z = mv.z * REF_RADIUS;

    rpv.x -= mv.x;
    rpv.z -= mv.z;

    return cosGTRPV(rpv, grain);
}

void growColumns(Subdomain& subdomain)
{
    //const double LB_PROBABILITY = 0.7;

    for(cm_pos y = subdomain.y0; y < subdomain.y1; y++)
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        for(int g = 0; g < subdomain.grainNum; g++)
        {
            cm_pos_vec rpv = calculateRPV(&subdomain.grains[g], x, y, z);
            double cos_rpvgt = cosGTRPV(rpv, &subdomain.grains[g]);
            f_vec h0 = calculateH0(y, &subdomain.grains[g]);
            
            double param_p = 1.0;

            if(isWithinRadius(x,y,z, h0, &subdomain.grains[g]) == false)
            {
                double cosC = cosCircleRPV(x,y,z,rpv, h0, &subdomain.grains[g]);
                param_p = calculateP(cosC, &subdomain.grains[g]);
            }
            
            double param_r = calculateR(rpv, &subdomain.grains[g]);

            bool classification = param_p * param_r;
            if(classification)
            {
                subdomain.getCell(x,y,z) = subdomain.grains[g].ID;
                break;
            }

        }
    }
}