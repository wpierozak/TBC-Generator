#include<omp.h>
#include"CM_generationOMP.hpp"

void generate(Domain& caDomain, const int threadsNumber)
{
    nucleation(caDomain);
    Subdomain* subdomains = createSubdomains(caDomain, threadsNumber);
    #pragma omp parallel default(none) shared(subdomains) num_threads(threadsNumber)
    {
        int idx = omp_get_thread_num();
        while(subdomains[idx].y1 < subdomains[idx].dimY)
        {
            grainGrowth(subdomains[idx]);
            checkBottomLayer(subdomains[idx]);
            checkUpperLayer(subdomains[idx]);
            subdomains[idx].y1 = (subdomains[idx].y1 > subdomains[idx].y0) ? subdomains[idx].y1: subdomains[idx].y0 + 1;
            swapStateBuffers(subdomains[idx]);
            #pragma omp barrier
        }
    }

    delete[] subdomains;
}

Subdomain* createSubdomains(Domain& caDomain, const int threadsNumber)
{
    Subdomain* subdomains = new Subdomain[threadsNumber];
    cm_pos dx = (caDomain.getDimX() + threadsNumber - 1)/threadsNumber;
    cm_pos dz = (caDomain.getDimZ() + threadsNumber - 1)/threadsNumber;
    cm_pos y0 = 0;
    cm_pos y1 = 1;

    int counter = 0;

    for(cm_pos z = 0; z < caDomain.getDimZ(); z+=dz)
    for(cm_pos x = 0; x < caDomain.getDimX(); x+=dx)
    {
        subdomains[counter].dimX = caDomain.getDimX();
        subdomains[counter].dimY = caDomain.getDimY();
        subdomains[counter].dimZ = caDomain.getDimZ();
        subdomains[counter].neighborhood = caDomain.getNeighborhood();
        subdomains[counter].inputStates = caDomain.getAbuffer();
        subdomains[counter].outputStates = caDomain.getBbuffer();
        subdomains[counter].x0 = x;
        subdomains[counter].x1 = ((x + dx) < caDomain.getDimX())? x + dx: caDomain.getDimX();
        subdomains[counter].y0 = y0;
        subdomains[counter].y1 = y1;
        subdomains[counter].z0 = z;
        subdomains[counter].z1 = ((z + dz) < caDomain.getDimZ())? z + dz: caDomain.getDimZ();                 
    }

    return subdomains;
}

void checkBottomLayer(Subdomain& subdomain)
{
    bool moveBottomLayer = false;
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.outputStates[subdomain.y0*(subdomain.dimX*subdomain.dimZ) + z*subdomain.dimX + x] != EMPTY)
            {
                moveBottomLayer = true;
                break;
            }
    }
    subdomain.y0 = (moveBottomLayer) ? subdomain.y0: subdomain.y0 + 1;
}

void checkUpperLayer(Subdomain& subdomain)
{
    bool moveUpperLayer = false;
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.outputStates[(subdomain.y1 - 1)*(subdomain.dimX*subdomain.dimZ) + z*subdomain.dimX + x] != EMPTY)
            {
                moveUpperLayer = true;
            }
    }
    subdomain.y1 = (moveUpperLayer) ? subdomain.y1: subdomain.y1 + 1;
}

void swapStateBuffers(Subdomain& subdomain)
{
    cm_state* tmp = subdomain.inputStates;
    subdomain.inputStates = subdomain.outputStates;
    subdomain.outputStates = tmp;
}