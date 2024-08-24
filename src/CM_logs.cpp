#include<string>
#include"CM_logs.hpp"

const std::string LOGS_DIR = "/home/wiktor/Desktop/engineering_thesis/logs/";

LogManager& LogManager::Manager()
{
    static LogManager logs;
    return logs;
}

LogManager::LogManager()
{
    _logmode = false;
}

LogManager::~LogManager()
{
    if(_outstream.is_open())
    _outstream.close();
}

void LogManager::resetStream(std::string filename)
{
    if(_outstream.is_open())
    _outstream.close();

    std::string file = LOGS_DIR + filename;
     _outstream.open(file);
    if(_outstream.is_open() == false)
    throw std::runtime_error("Cannot open/create logs file");

    _logmode = true;
}

void LogManager::open(std::string input)
{
    _outstream << "\n\t\t---> BEGIN " << input << " <---\n\n"; 
}

void LogManager::close(std::string input)
{
     _outstream << "\n\t\t---> END " << input << " <---\n\n"; 
}

void LogManager::header(std::string input)
{
    _outstream << "=== " << input << " ===\n";
}
    
void LogManager::text(std::string input)
{
    _outstream << input << std::endl;
}

void LogManager::timeStart(std::string input)
{
    header(std::string("Time measurment - ") + input + std::string(" - start"));
    _tstart = std::chrono::high_resolution_clock::now();
}

void LogManager::recordTimeElapsed(std::string input)
{
    _tend = std::chrono::high_resolution_clock::now();
    header(std::string("Time measurment - ") + input + std::string(" - stop"));
    _outstream << "[T] Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(_tend - _tstart).count() << std::endl;
}

void LogManager::printGrainData(const Grain& grain)
{

}
void LogManager::printConfigData(const Configuration& config)
{
    open("Configuration");
   // _outstream << "Input file: " << config.inputFile << std::endl;
    _outstream << "Output dir" << config.outputDir << std::endl;
    _outstream << "Number of threads to be used: " << config.threadsNum << std::endl;
    _outstream << "Domain: ";
    //if(config.domain != nullptr) _outstream << "allocated\n";
    //else _outstream << "not allocated";
    _outstream << "Grains number: " << config.grainsNumber << std::endl;
   // printMspData(config.layers);
    close("Configuration");
}

void LogManager::printMspData(const Layer& msp)
{
    header("Microstructure Properties");
}

void LogManager::exception(std::string mess)
{
    open("Exception");
    _outstream << "Message: " << mess << std::endl;
    close("Exception");
}