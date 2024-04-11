#pragma once
#include<iostream>
#include"CM_domain.hpp"
#include"CM_mstypes.hpp"
#include"CM_shapefun.hpp"

struct SectionProfile
{
    SectionProfile() = default;
    SectionProfile(const SectionProfile& tocpy): profile(tocpy.profile), coeff(tocpy.coeff) {}
    sfptr profile;
    std::vector<double> coeff;
};

struct Configuration
{
    std::unique_ptr<Domain> domain;

    grains_array grains;
    cm_size grainsNumber;

    Microstructure_Properties msp;
    std::vector<SectionProfile> profilesSmooth;
    std::vector<SectionProfile> profilesFeathered;
    std::vector<SectionProfile> profilesTop;

    cm_smallsize threadsNum;

    std::string outputFile;
    std::string inputFile;
    std::string outputDir;
    MsFileFormat msFileFormat;

};