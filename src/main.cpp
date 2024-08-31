#include"config.hpp"
#include"parser.hpp"
#include"postproc.hpp"
#include"run.hpp"
#include<iostream>

int main(int argc, const char** argv)
{
    ConfigurationParser parser;
    Configuration config;

    std::string inputFile = argv[1];
    try
    {
    
        parser.parseXmlFile(inputFile);
        config = parser.createConfiguration();
        GenerationManager manager(config);
        manager.generate();
        //run(config);
        saveMicrostructureFile(manager.domain(),config);
    }
    catch(const std::exception& e)
    {
        std::cout << (e.what());
        return -1;
    }
    return 0;
}