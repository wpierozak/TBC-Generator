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
    header("Grain");
  _outstream << "ID: " << grain.ID << std::endl;
  _outstream << "Center: (" << grain.center.x << ", " << grain.center.y << ", " << grain.center.z << ")" << std::endl;
  _outstream << "Growth Tensor: (" << grain.growth_tensor.x << ", " << grain.growth_tensor.y << ", " << grain.growth_tensor.z << ")" << std::endl;
  _outstream << "Cos Phi Upper Bound: " << grain.angle_of_widen << std::endl;
  _outstream << "RPV Norm Upper Bound: " << grain.ref_length << std::endl;
  _outstream << "h0 Norm Bounds:" << std::endl;
  _outstream << "\tSmooth Region: " << grain.h0_norm_smooth_region << std::endl;
  _outstream << "\tFeathered Region: " << grain.h0_norm_feathered_region << std::endl;
  _outstream << "\tTop Region: " << grain.h0_norm_top_region << std::endl;
  _outstream << "Reference Column Radius: " << grain.ref_column_rad << std::endl;
  _outstream << "Maximum Column Radius: " << grain.max_column_rad << std::endl;
}

void LogManager::printConfigData(const Configuration& config)
{
    open("Configuration");
   // _outstream << "Input file: " << config.inputFile << std::endl;
    _outstream << "Output dir" << config.outputDir << std::endl;
    _outstream << "Number of threads to be used: " << config.threadsNum << std::endl;
    _outstream << "Domain: ";
    if(config.domain != nullptr) _outstream << "allocated\n";
    else _outstream << "not allocated";
    _outstream << "Grains number: " << config.grainsNumber << std::endl;
    printMspData(config.msp);
    close("Configuration");
}

void LogManager::printMspData(const Microstructure_Properties& msp)
{
    header("Microstructure Properties");
    _outstream << "Maximum Angle of Widening: " << msp.max_angle_of_widen << std::endl;
    _outstream << "Maximum Tilt (YZ, XY Plane): " << msp.max_tilt << std::endl;
    _outstream << "Minimum Tilt (YZ, XY Plane): " << msp.min_tilt << std::endl;
    _outstream << "Smooth Region Length (%): " << msp.smooth_region_length << std::endl;
    _outstream << "Smooth Region Length Variance (%): " << msp.smooth_region_length_var << std::endl;
    _outstream << "Feathered Region Length (%): " << msp.feathered_region_length << std::endl;
    _outstream << "Feathered Region Length Variance (%): " << msp.feathered_region_length_var << std::endl;
    _outstream << "Top Region Length (%): " << msp.top_region_length << std::endl;
    _outstream << "Top Region Length Variance (%): " << msp.top_region_length_var << std::endl;
    _outstream << "Minimum Column Length: " << msp.min_length << std::endl;
    _outstream << "Maximum Column Length: " << msp.max_length << std::endl;
    _outstream << "Maximum Reference Radius: " << msp.max_reference_radius << std::endl;
}

void LogManager::exception(std::string mess)
{
    open("Exception");
    _outstream << "Message: " << mess << std::endl;
    close("Exception");
}