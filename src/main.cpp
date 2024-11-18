#include"config.hpp"
#include"parser.hpp"
#include"postproc.hpp"
#include"run.hpp"
#include<iostream>
#include<chrono>

int main(int argc, const char** argv)
{
    ConfigurationParser parser;
    Configuration config;

    std::string inputFile = argv[1];
    try
    {
    
        parser.parseXmlFile(inputFile);
        config = parser.createConfiguration();
        //config.print();
        GenerationManager manager(config);
        // Start time measurement for `manager.generate()`
        auto start = std::chrono::high_resolution_clock::now();

        manager.generate();

        // End time measurement
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate the elapsed time in milliseconds
        std::chrono::duration<double, std::milli> elapsed = end - start;

        // Print out the elapsed time
        std::cout << "Generation time: " << elapsed.count() << " ms" << std::endl;

        saveMicrostructureFile(manager.domain(),config);
    }
    catch(const std::exception& e)
    {
        std::cout << (e.what());
        return -1;
    }
    return 0;
}