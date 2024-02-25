#include<omp.h>
#include<cstring>
#include<iostream>
#include"CM_generationOMP.hpp"


void generate(Domain& caDomain, const int threadsNumber)
{
    srand(time(NULL));
    nucleation(caDomain);
    Subdomain* subdomains = createSubdomains(caDomain, threadsNumber);

    #ifdef LOGS
        std::cout<<"Generating begins - OMP\n";
    #endif

    #pragma omp parallel default(none) shared(subdomains) num_threads(threadsNumber) firstprivate(threadsNumber) shared(std::cout)
    {
        int idx = omp_get_thread_num();
        bool done = false;
        static int alldone = 0;

        while(alldone != threadsNumber)
        {
            if(subdomains[idx].y0 != subdomains[idx].y1)
            {
                grainGrowth(subdomains[idx]);

                for(cm_pos y = subdomains[idx].y0; y < subdomains[idx].y1; y++)
                for(cm_pos z = subdomains[idx].z0; z < subdomains[idx].z1; z++)
                    std::memcpy(subdomains[idx].inputStates + subdomains[idx].getIdx(subdomains[idx].x0, y, z),
                    subdomains[idx].outputStates + subdomains[idx].getIdx(subdomains[idx].x0, y, z), 
                    (subdomains[idx].x1 - subdomains[idx].x0)*sizeof(cm_state));

                checkBottomLayer(subdomains[idx]);
                checkUpperLayer(subdomains[idx]);
                subdomains[idx].y1 = (subdomains[idx].y1 < subdomains[idx].dimY) ? subdomains[idx].y1 : subdomains[idx].dimY - 1;
            }
            else if(done == false)
            {
                done = true;
                #pragma omp critical
                {
                    alldone += 1;
                }
            }
            #pragma omp barrier
        }
    }

    #ifdef LOGS
        std::cout<<"Generating ends - OMP\n";
    #endif

    delete[] subdomains;
}

Subdomain* createSubdomains(Domain& caDomain, const int threadsNumber)
{
    Subdomain* subdomains = new Subdomain[threadsNumber];
    cm_pos dx = (caDomain.getDimX() + threadsNumber - 1)/threadsNumber;
    cm_pos dz = (caDomain.getDimZ() + threadsNumber - 1)/threadsNumber;
    cm_pos y0 = 0;
    cm_pos y1 = 5;

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
        counter++;              
    }
    return subdomains;
}

void checkBottomLayer(Subdomain& subdomain)
{
    bool moveBottomLayer = true;
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.outputStates[subdomain.getIdx(x, subdomain.y0, z)] == EMPTY)
            {
                moveBottomLayer = false;
                break;
            }
    }
    subdomain.y0 = (moveBottomLayer) ? subdomain.y0+1: subdomain.y0 ;
}

void checkUpperLayer(Subdomain& subdomain)
{
    bool moveUpperLayer = false;
    //if(subdomain.y1 != subdomain.dimY - 1)
    {
        for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
        for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
        {
            if(subdomain.outputStates[subdomain.getIdx(x, subdomain.y1 - 1, z)] != EMPTY)
            {
                moveUpperLayer = true;
            }
        }
    }
    /*else
    {
        moveUpperLayer = true;
        for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
        for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
        {
            if(subdomain.outputStates[subdomain.getIdx(x, subdomain.y1 - 1, z)] == EMPTY)
            {
                moveUpperLayer = false;
            }
        }
    }*/
    subdomain.y1 = (moveUpperLayer) ? subdomain.y1+1: subdomain.y1;
}

void swapStateBuffers(Subdomain& subdomain)
{
    cm_state* tmp = subdomain.inputStates;
    subdomain.inputStates = subdomain.outputStates;
    subdomain.outputStates = tmp;
}