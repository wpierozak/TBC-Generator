#include<iostream>
#include"CM_datatypes.hpp"

#define LOGS

GeneratorConfig::GeneratorConfig(cm_pos dim[]):
_dimX(dim[0]), _dimY(dim[1]), _dimZ(dim[2])
{
    _domain = new cm_state[cm_size(_dimX) * cm_size(_dimY) * cm_size(_dimZ)];
    _statesBuffer = new cm_state[cm_size(_dimX) * cm_size(_dimY) * cm_size(_dimZ)];
    for(cm_size i = 0; i < getCellsNum(); i++)
    {
        _domain[i] = EMPTY;
        _statesBuffer[i] = EMPTY;
    }
}

GeneratorConfig::~GeneratorConfig()
{
    delete[] _domain;
    delete[] _statesBuffer;
}

void GeneratorConfig::printConfiguration() const
{
    std::cout<< "\t===\tDomain configuration\t===\n";
    std::cout<< "Input file:\t" << _inputFile << std::endl;
    std::cout<< "Dimension:\t" << _dimX << '\t' << _dimY << '\t' << _dimZ << std::endl;
    std::cout<< "Nucleuses number:\t" << _grainsNumber << std::endl;
    std::cout<< "Neighbourhood:\n";
    std::cout<<"\talpha:\t" << _neighbourhood.alpha << std::endl;
    std::cout<<"\tbeta:\t" << _neighbourhood.beta << std::endl;
    std::cout<<"\tradius:\t" << _neighbourhood.r << std::endl;
    std::cout<< "Base neighbourhood:\n";
    std::cout<<"\tradius:\t" << _baseNeighbourhood.r << std::endl;
    std::cout<< "Output file:\t" << _outputFile << std::endl;
}
