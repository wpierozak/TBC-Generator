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
    _outstream << "\n\t\t---> BEGIN " << input << " <---\n"; 
}

void LogManager::close(std::string input)
{
     _outstream << "\n\t\t---> END " << input << " <---\n"; 
}

void LogManager::header(std::string input)
{
    _outstream << "\t=== " << input << " ===\n";
}
    
void LogManager::text(std::string input)
{
    _outstream << input << std::endl;
}

void LogManager::timeStart(std::string input)
{
    header(std::string("TIME MEASURMENT") + input);
    _tstart = std::chrono::high_resolution_clock::now();
}

void LogManager::recordTimeElapsed(std::string input)
{
    _tend = std::chrono::high_resolution_clock::now();
    _outstream << "[T] ELAPSED TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(_tend - _tstart).count() << std::endl;
    header(std::string("STOP TIME MEASURMENT") + input);
}

void LogManager::printGrainData(const Grain& grain)
{
    header("Grain");
  _outstream << "\tID: " << grain.ID << std::endl;
  _outstream << "\tCenter: (" << grain.center.x << ", " << grain.center.y << ", " << grain.center.z << ")" << std::endl;
  _outstream << "\tGrowth Tensor: (" << grain.growth_tensor.x << ", " << grain.growth_tensor.y << ", " << grain.growth_tensor.z << ")" << std::endl;
  _outstream << "\tCos Phi Upper Bound: " << grain.angle_of_widen << std::endl;
  _outstream << "\tRPV Norm Upper Bound: " << grain.ref_length << std::endl;
  _outstream << "\th0 Norm Bounds:" << std::endl;
  _outstream << "\t\tSmooth Region: " << grain.h0_norm_smooth_region << std::endl;
  _outstream << "\t\tFeathered Region: " << grain.h0_norm_feathered_region << std::endl;
  _outstream << "\t\tTop Region: " << grain.h0_norm_top_region << std::endl;
  _outstream << "\tReference Column Radius: " << grain.ref_column_rad << std::endl;
  _outstream << "\tMaximum Column Radius: " << grain.max_column_rad << std::endl;
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
    _outstream << "\tMaximum Angle of Widening: " << msp.max_angle_of_widen << std::endl;
    _outstream << "\tMaximum Tilt (YZ, XY Plane): " << msp.max_tilt << std::endl;
    _outstream << "\tMinimum Tilt (YZ, XY Plane): " << msp.min_tilt << std::endl;
    _outstream << "\tSmooth Region Length (%): " << msp.smooth_region_length << std::endl;
    _outstream << "\tSmooth Region Length Variance (%): " << msp.smooth_region_length_var << std::endl;
    _outstream << "\tFeathered Region Length (%): " << msp.feathered_region_length << std::endl;
    _outstream << "\tFeathered Region Length Variance (%): " << msp.feathered_region_length_var << std::endl;
    _outstream << "\tTop Region Length (%): " << msp.top_region_length << std::endl;
    _outstream << "\tTop Region Length Variance (%): " << msp.top_region_length_var << std::endl;
    _outstream << "\tMinimum Column Length: " << msp.min_length << std::endl;
    _outstream << "\tMaximum Column Length: " << msp.max_length << std::endl;
    _outstream << "\tMaximum Reference Radius: " << msp.max_reference_radius << std::endl;
}

void LogManager::exception(std::string mess)
{
    open("Exception");
    _outstream << "Message: " << mess << std::endl;
    close("Exception");
}