#include<omp.h>
#include<cstring>
#include<iostream>
#include"CM_generationOMP.hpp"

#define LOGS

void generate(Domain& caDomain, const int threadsNumber)
{
    nucleation(caDomain);
    Subdomain* subdomains = createSubdomains(caDomain, threadsNumber);
    cm_size threadChunk = cm_size(subdomains->x1 - subdomains->x0)*cm_size(subdomains->y1 - subdomains->y0)*cm_size(subdomains->z1-subdomains->z0);
    #ifdef LOGS
        std::cout<< "Thread chunk: " << threadChunk << std::endl;
        std::cout<<"Generating begins - OMP\n";
    #endif
    #pragma omp parallel default(none) shared(subdomains) num_threads(threadsNumber)
    {
        int idx = omp_get_thread_num();
        cm_size threadChunk = subdomains->getIdx(subdomains[idx].x1, subdomains[idx].y1 -1, subdomains[idx].z1-1);
        bool done = false;
        static int alldone = 0; 
        while(true)
        {
            if(subdomains[idx].y0 != subdomains[idx].y1)
            {
                grainGrowth(subdomains[idx]);
                std::memcpy(subdomains[0].inputStates + cm_size(idx)*threadChunk, subdomains[0].outputStates + cm_size(idx)*threadChunk, threadChunk*sizeof(cm_state));
                checkBottomLayer(subdomains[idx]);
                checkUpperLayer(subdomains[idx]);
                subdomains[idx].y1 = (subdomains[idx].y1 > subdomains[idx].y0) ? subdomains[idx].y1: subdomains[idx].y0 + 1;
                subdomains[idx].y1 = (subdomains[idx].y1 < subdomains[idx].dimY) ? subdomains[idx].y1: subdomains[idx].dimY - 1;
                threadChunk = subdomains->getIdx(subdomains[idx].x1, subdomains[idx].y1 -1, subdomains[idx].z1-1);
            }
            else if(done == false)
            {
                done = true;
                #pragma omp atomic
                alldone += 1;

                #ifdef LOGS
                std::cout<< "Thread " << idx << " finished" << std::endl;
                #endif
            }
            #pragma omp barrier
            if(alldone == threadsNumber) break;
            #pragma omp barrier
            #pragma omp single
            {
                //std::cout << subdomains[idx].y0 << " " << subdomains[idx].y1 << std::endl;
            }
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

    #ifdef LOGS
        std::cout <<"Dim X: " << subdomains[0].dimX << std::endl;
        std::cout <<"Dim Y: " << subdomains[0].dimY << std::endl;
        std::cout <<"Dim Z: " << subdomains[0].dimZ << std::endl;
        std::cout << "x0: " << subdomains[0].x0 << std::endl;
        std::cout << "x1: " << subdomains[0].x1 << std::endl;
    #endif

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
    for(cm_pos z = subdomain.z0; z < subdomain.z1; z++)
    for(cm_pos x = subdomain.x0; x < subdomain.x1; x++)
    {
        if(subdomain.outputStates[subdomain.getIdx(x, subdomain.y1 - 1, z)] != EMPTY)
            {
                moveUpperLayer = true;
            }
    }
    subdomain.y1 = (moveUpperLayer) ? subdomain.y1+1: subdomain.y1;
}

void swapStateBuffers(Subdomain& subdomain)
{
    cm_state* tmp = subdomain.inputStates;
    subdomain.inputStates = subdomain.outputStates;
    subdomain.outputStates = tmp;
}