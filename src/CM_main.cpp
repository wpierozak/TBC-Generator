#include"CM_config.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"
#include"CM_run.hpp"
#include<iostream>

int main(int argc, const char** argv)
{
    Configuration config;

    std::string inputFile = argv[1];
    try
    {
        parseConfiguration(inputFile, config);

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