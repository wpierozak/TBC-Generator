#include"CM_config.hpp"
#include"CM_xmlparser.hpp"
#include"CM_postproc.hpp"
#include"CM_run.hpp"
#include"CM_logs.hpp"

int main(int argc, const char** argv)
{
    Configuration config;

    std::string inputFile = argv[1];
    bool logmode = false;
    logmode = argc >= 2;
    LogManager::Manager().changeMode(logmode);
    if(LogManager::Manager().logmode()) 
        LogManager::Manager().resetStream(argv[2]);      
    try
    {
        if(LogManager::Manager().logmode()) LogManager::Manager().open("Parsing");
        if(LogManager::Manager().logmode()) LogManager::Manager().text(std::string("Input file: ") + inputFile);
        parseConfiguration(inputFile, config);
        if(LogManager::Manager().logmode()) LogManager::Manager().printConfigData(config);
        if(LogManager::Manager().logmode()) LogManager::Manager().close("Parsing");
        if(LogManager::Manager().logmode()) LogManager::Manager().timeStart("Run");
        GenerationManager manager;
        manager.generate_single_layer(config);
        //run(config);
        if(LogManager::Manager().logmode()) LogManager::Manager().recordTimeElapsed("Run");

        if(LogManager::Manager().logmode()) LogManager::Manager().open("Results saving");
        if(LogManager::Manager().logmode()) LogManager::Manager().timeStart("File generation");
        saveMicrostructureFile(&config);
        if(config.bitmaps) createBitmap(config);
        if(LogManager::Manager().logmode()) LogManager::Manager().recordTimeElapsed("File generation");
        if(LogManager::Manager().logmode()) LogManager::Manager().close("Results saving");
    }
    catch(const std::exception& e)
    {
        if(LogManager::Manager().logmode()) LogManager::Manager().exception(e.what());
        else e.what();
        return -1;
    }
    return 0;
}