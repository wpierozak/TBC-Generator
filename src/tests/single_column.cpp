#include<chrono>
#include<iostream>

#include"CM_run.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"

const std::string XML_INPUT = "/home/wiktor/Desktop/engineering_thesis/xml/test_single_column.xml";

int main()
{
    clock_t t0, t1;
    Configuration* d0 = nullptr;
    try
    {
         d0 = parseConfiguration(XML_INPUT);
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    run(*d0);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    std::cout<<"Time of the generation process: " << elapsed.count() << " milliseconds" <<std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    createBitmap(*d0);
    end_time = std::chrono::high_resolution_clock::now();

    elapsed = end_time - start_time;
    std::cout<<"Time of the bitmap creation process: " << elapsed.count() << " milliseconds" <<std::endl;

    saveMicrostructureFile(*d0);

    delete d0;
    return 0;
}