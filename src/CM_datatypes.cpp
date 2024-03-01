#include<iostream>
#include"CM_datatypes.hpp"

#define LOGS

Domain::Domain(cm_pos dim[], Neighbourhood neighbourhood, cm_size nucleusesNum, std::string inputFile, std::string outputFile):
    _dimX(dim[0]), _dimY(dim[1]), _dimZ(dim[2]), _inputFile(inputFile), _outputFile(outputFile)
{
    _neighbourhood = neighbourhood;
    _Astates = new cm_state[cm_size(_dimX) * cm_size(_dimY) * cm_size(_dimZ)];
    _Bstates = new cm_state[cm_size(_dimX) * cm_size(_dimY) * cm_size(_dimZ)];
    _colors = nullptr;
    _nucleusNum = nucleusesNum;
    for(cm_size i = 0; i < getCellsNum(); i++)
    {
        _Astates[i] = EMPTY;
        _Bstates[i] = EMPTY;
    }
}

Domain::~Domain()
{
    delete[] _Astates;
    delete[] _Bstates;
    if(_colors != nullptr) delete[] _colors;
}

void Domain::printConfiguration() const
{
    std::cout<< "\t===\tDomain configuration\t===\n";
    std::cout<< "Input file:\t" << _inputFile << std::endl;
    std::cout<< "Dimension:\t" << _dimX << '\t' << _dimY << '\t' << _dimZ << std::endl;
    std::cout<< "Nucleuses number:\t" << _nucleusNum << std::endl;
    std::cout<< "Neighbourhood size:\t" << _neighbourhood.size << std::endl;
    std::cout<< "Neighbourhood:\n";
    for(int i = 0; i < _neighbourhood.size; i++)
        std::cout<< "\t" << _neighbourhood.neighbours[i][0] << '\t' <<_neighbourhood.neighbours[i][1] << '\t' <<_neighbourhood.neighbours[i][2] << std::endl;
    std::cout<< "Output file:\t" << _outputFile << std::endl;
}
    