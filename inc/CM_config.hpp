#pragma once
#include"CM_domain.hpp"
#include"CM_mstypes.hpp"

struct Configuration
{
    void print();

    std::unique_ptr<Domain> domain;

    grains_array grains;
    cm_size grainsNumber;

    Microstructure_Properties msp;

    cm_smallsize threadsNum;

    std::string outputFile;
    std::string inputFile;
    std::string outputDir;
    MsFileFormat msFileFormat;
};