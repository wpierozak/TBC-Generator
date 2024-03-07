#include<omp.h>
#include<cstring>
#include<iostream>
#include<math.h>
#include"CM_generationOMP.hpp"

void generate(GeneratorConfig& caDomain, const int threadsNumber)
{
    srand(time(NULL));
    #ifdef LOGS
    std::cout<<"\t===\tNucleation - start\t===\t"<<std::endl;
    #endif

    nucleation(caDomain);

    #ifdef LOGS
    std::cout<<"\t===\tNucleation - end\t===\t"<<std::endl;
    #endif

    #ifdef LOGS
    std::cout<<"\t===\tSubdomain creation - start\t===\t"<<std::endl;
    #endif

    Subdomain* subdomains = createSubdomains(caDomain, threadsNumber);
    #ifdef LOGS

    std::cout<<"\t===\tSubdomain creation - end\t===\t"<<std::endl;
    #endif

    if(caDomain.fillBase())
    {

    #ifdef LOGS
    std::cout<<"\t===\tFilling base - start\t===\t"<<std::endl;
    #endif

    #pragma omp parallel default(none) shared(subdomains) num_threads(threadsNumber) firstprivate(threadsNumber) 
    {
        int idx = omp_get_thread_num();
        bool done = false;
        static int alldone = 0;
        while(alldone != threadsNumber)
        {
            if(done == false)
            {
                fillBase(subdomains[idx]);

                for(cm_pos z = subdomains[idx].z0; z < subdomains[idx].z1; z++)
                    std::memcpy(&subdomains[idx].getCell(subdomains[idx].x0, 0, z), &subdomains[idx].accessStatesBuffer(subdomains[idx].x0, 0, z), 
                    (subdomains[idx].x1 - subdomains[idx].x0)*sizeof(cm_state));

                done = true;
                for(cm_pos z = subdomains[idx].z0; z < subdomains[idx].z1; z++)
                for(cm_pos x = subdomains[idx].x0; x < subdomains[idx].x1; x++)
                {
                    if(subdomains[idx].getCell(x,0,z) == EMPTY)
                    {
                        done = false;
                        break;
                    }
                }

                if(done == true)
                {
                    #pragma omp critical
                    {
                    alldone += 1;
                    }
                }
            }
           
            #pragma omp barrier
        }
    }

    #ifdef LOGS
    std::cout<<"\t===\tFilling base - end\t==="<<std::endl;
    #endif

    }

    #ifdef LOGS
        std::cout<<"\t===\tMicrostructure generation - OMP - start\t===" << std::endl;
    #endif

    #pragma omp parallel default(none) shared(subdomains) num_threads(threadsNumber) firstprivate(threadsNumber) shared(std::cout)
    {
        int idx = omp_get_thread_num();
        bool done = false;
        static int alldone = 0;

        while(alldone != threadsNumber)
        {
            #pragma omp barrier
            
            if(subdomains[idx].y1 != subdomains[idx].dimY)
            {
                grainGrowth(subdomains[idx]);
                for(cm_pos y = subdomains[idx].y0; y < subdomains[idx].y1; y++)
                for(cm_pos z = subdomains[idx].z0; z < subdomains[idx].z1; z++)
                    std::memcpy(&subdomains[idx].getCell(subdomains[idx].x0, y, z),
                    &subdomains[idx].accessStatesBuffer(subdomains[idx].x0, y, z), 
                    (subdomains[idx].x1 - subdomains[idx].x0)*sizeof(cm_state));
                checkBottomLayer(subdomains[idx]);
                checkUpperLayer(subdomains[idx]);
                //subdomains[idx].y1 = (subdomains[idx].y1 < subdomains[idx].dimY) ? subdomains[idx].y1 : subdomains[idx].dimY - 1;
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
        std::cout<<"\t===\tMicrostructure generation - OMP - end\t===" << std::endl;
    #endif

    delete[] subdomains;
}

Subdomain* createSubdomains(GeneratorConfig& caDomain, const int threadsNumber)
{
    Subdomain* subdomains = new Subdomain[threadsNumber];
    cm_pos dx = (caDomain.getDimX() + threadsNumber - 1)/ceil(sqrt(threadsNumber));
    cm_pos dz = (caDomain.getDimZ() + threadsNumber - 1)/floor(sqrt(threadsNumber));
    cm_pos y0 = 0;
    cm_pos y1 = 5;

    int counter = 0;

    for(cm_pos z = 0; z < caDomain.getDimZ(); z+=dz)
    for(cm_pos x = 0; x < caDomain.getDimX(); x+=dx)
    {
        subdomains[counter].dimX = caDomain.getDimX();
        subdomains[counter].dimY = caDomain.getDimY();
        subdomains[counter].dimZ = caDomain.getDimZ();
        subdomains[counter].neighbourhood = caDomain.getNeighbourhood();
        subdomains[counter].baseNeighbourhood = caDomain.getBaseNeighbourhood();
        subdomains[counter].boundryCondition = caDomain.getBC();
        subdomains[counter].domain = caDomain.getDomain();
        subdomains[counter].statesBuffer = caDomain.getStatesBuffer();
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
        if(subdomain.statesBuffer[subdomain.getIdx(x, subdomain.y0, z)] == EMPTY)
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
        if(subdomain.statesBuffer[subdomain.getIdx(x, subdomain.y1 - 1, z)] != EMPTY)
        {
            moveUpperLayer = true;
        }
    }
    subdomain.y1 = (moveUpperLayer) ? subdomain.y1+1: subdomain.y1;
}
