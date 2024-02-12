#include<omp.h>
#include"CM_generationOMP.hpp"

void generate(Domain& caDomain, const int threadsNumber)
{
    nucleation(caDomain);
    Subdomain* subdomains = createSubdomains(caDomain, threadsNumber);
    #pragma omp parallel default(none) shared(subdomains)
    {
        int idx = omp_get_thread_num();
        while(subdomains[idx].y1 < subdomains[idx].dimY)
        {
            grainGrowth(subdomains[idx]);
            checkBottomLayer(subdomains[idx]);
            checkUpperLayer(subdomains[idx]);
            subdomains[idx].y1 = (subdomains[idx].y1 > subdomains[idx].y0) ? subdomains[idx].y1: subdomains[idx].y0 + 1;
            #pragma omp barrier
        }
    }
}

void checkBottomLayer(Subdomain& subdomain)
{
    bool moveBottomLayer = true;
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.outputStates[subdomain.y0*(subdomain.dimX*subdomain.dimZ) + z*subdomain.dimX + x] == EMPTY)
            {
                moveBottomLayer = false;
                break;
            }
    }
    subdomain.y0 = (moveBottomLayer) ? subdomain.y0: subdomain.y0 + 1;
}

void checkUpperLayer(Subdomain& subdomain)
{
    bool moveUpperLayer = true;
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.outputStates[(subdomain.y1 - 1)*(subdomain.dimX*subdomain.dimZ) + z*subdomain.dimX + x] == EMPTY)
            {
                moveUpperLayer = false;
            }
    }
    subdomain.y1 = (moveUpperLayer) ? subdomain.y1: subdomain.y1 + 1;
}