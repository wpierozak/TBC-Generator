#include<iostream>
#include<omp.h>
#include<chrono>
#include"CM_datatypes.hpp"
#include"CM_generationOMP.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"

int main(int argc, const char* argv[])
{
    clock_t t0, t1;
    GeneratorConfig* d0 = nullptr;

    if(argc == 1)
    {
        std::cout <<"Configuration file has been not passed to the program!" <<std::endl;
        return -1;
    }

    try
    {
         d0 = parseConfiguration(argv[1]);
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    
    d0->printConfiguration();

    auto start_time = std::chrono::high_resolution_clock::now();
    generate(*d0, d0->getThreadsNumber());
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout<<"Time of the generation process: " << elapsed.count() << " milliseconds" <<std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    createBitmap(*d0, d0->getThreadsNumber());
    end_time = std::chrono::high_resolution_clock::now();

    elapsed = end_time - start_time;
    std::cout<<"Time of the bitmap creation process: " << elapsed.count() << " milliseconds" <<std::endl;

    saveMicrostructureFile(*d0);

    delete d0;
    return 0;
}
