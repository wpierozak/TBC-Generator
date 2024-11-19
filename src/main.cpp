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

    if(argc != 2){
        std::cout<< "Please provide path to the configuration file" << std::endl;
        return -1;
    }

    std::string inputFile = argv[1];
    try
    {
        parser.parseXmlFile(inputFile);
        config = parser.createConfiguration();
        GenerationManager manager(config);
        manager.generate();

        saveMicrostructureFile(manager.domain(),config);
    }
    catch(const std::exception& e)
    {
        std::cout << (e.what());
        return -1;
    }
    return 0;
}