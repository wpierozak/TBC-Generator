#pragma once
#include<iostream>
#include"CM_domain.hpp"
#include"CM_mstypes.hpp"

struct Configuration
{
    _long_int dimX, dimY, dimZ;
    grains_array grains;
    _int grainsNumber;

    std::vector<Layer> layers;
    Neighbourhood neighbourhood;

    _int threadsNum;

    std::string outputFile;
    std::string inputFile;
    std::string outputDir;
    MsFileFormat msFileFormat;

    bool bitmaps{false};
};