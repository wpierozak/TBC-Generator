#pragma once
#include<iostream>
#include<chrono>
#include<fstream>
#include"CM_config.hpp"

class LogManager
{
    public:

    static LogManager& Manager();

    void resetStream(std::string filename);

    void open(std::string);
    void close(std::string);
    void header(std::string);
    void text(std::string);

    void printGrainData(const Grain&);
    void printConfigData(const Configuration&);
    void printMspData(const Microstructure_Properties&);

    void timeStart(std::string="");
    void recordTimeElapsed(std::string="");

    bool logmode() { return _logmode; }
    void changeMode(bool mode) { _logmode = mode; }

    void exception(std::string);

    private:

    LogManager();
    LogManager(const LogManager&) = delete;
    LogManager(LogManager&&) = delete;
    LogManager& operator=(const LogManager&) = delete;

    ~LogManager();

    bool _logmode;
    std::ofstream _outstream;

    std::chrono::time_point<std::chrono::system_clock> _tstart;
    std::chrono::time_point<std::chrono::system_clock> _tend;
};
