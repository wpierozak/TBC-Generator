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
        if(LogManager::Manager().logmode()) LogManager::Manager().text(inputFile);
        parseConfiguration(inputFile, config);
        if(LogManager::Manager().logmode()) LogManager::Manager().printConfigData(config);
        if(LogManager::Manager().logmode()) LogManager::Manager().close("Parsing");
   
        if(LogManager::Manager().logmode()) LogManager::Manager().timeStart("RUN");
        run(config);
        if(LogManager::Manager().logmode()) LogManager::Manager().recordTimeElapsed("RUN ENDED");

        if(LogManager::Manager().logmode()) LogManager::Manager().open("SAVING RESULTS");
        saveMicrostructureFile(&config);
        if(LogManager::Manager().logmode()) LogManager::Manager().close("SAVING RESULTS");
    }
    catch(const std::exception& e)
    {
        if(LogManager::Manager().logmode()) LogManager::Manager().header("EXCEPTION");
        if(LogManager::Manager().logmode()) LogManager::Manager().text(e.what());
        else e.what();
        return -1;
    }
    return 0;
}