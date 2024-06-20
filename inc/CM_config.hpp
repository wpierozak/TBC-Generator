#pragma once
#include<iostream>
#include"CM_domain.hpp"
#include"CM_mstypes.hpp"

struct Configuration
{
    std::unique_ptr<Domain> domain;

    grains_array grains;
    cm_int grainsNumber;

    Microstructure_Properties msp;

    cm_int threadsNum;

    std::string outputFile;
    std::string inputFile;
    std::string outputDir;
    MsFileFormat msFileFormat;

    bool bitmaps{false};
};