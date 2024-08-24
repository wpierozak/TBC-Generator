#pragma once
#include<iostream>
#include"CM_domain.hpp"
#include"CM_mstypes.hpp"

struct Configuration
{
    cm_pos dimX, dimY, dimZ;
    grains_array grains;
    cm_int grainsNumber;

    std::vector<Layer> layers;
    Neighbourhood neighbourhood;

    cm_int threadsNum;

    std::string outputFile;
    std::string inputFile;
    std::string outputDir;
    MsFileFormat msFileFormat;

    bool bitmaps{false};
};