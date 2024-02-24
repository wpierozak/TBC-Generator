#include<iostream>
#include<omp.h>
#include<chrono>
#include"CM_datatypes.hpp"
#include"CM_generationOMP.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"

int main()
{
    clock_t t0, t1;
    Domain* d0 = parseConfiguration("/home/wiktor/Desktop/engineering_thesis/xml/test_config.xml");
    d0->printConfiguration();

    auto start_time = std::chrono::high_resolution_clock::now();
    generate(*d0, 12);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout<<"Time of the generation process: " << elapsed.count() << " milliseconds" <<std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    createBitmap(*d0, 12);
    end_time = std::chrono::high_resolution_clock::now();

    elapsed = end_time - start_time;
    std::cout<<"Time of the bitmap creation process: " << elapsed.count() << " milliseconds" <<std::endl;

    delete d0;
    return 0;
}
