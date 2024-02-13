#include<iostream>
#include"CM_datatypes.hpp"

Domain::Domain(cm_pos dim[], Neighborhood neighborhood, std::string inputFile, std::string outputFile):
    _dimX(dim[0]), _dimY(dim[1]), _dimZ(dim[2]), _neighborhood(neighborhood), _inputFile(inputFile), _outputFile(outputFile)
{
    _Astates = new cm_state[static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ)];
    _Bstates = new cm_state[static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ)];
}

Domain::~Domain()
{
    delete[] _Astates;
    delete[] _Bstates;
    delete[] _colors;
}

void Domain::printConfiguration() const
{
    std::cout<< "\t===\tDomain configuration\t===\n";
    std::cout<< "Input file:\t" << _inputFile << std::endl;
    std::cout<< "Dimension:]t" << _dimX << '\t' << _dimY << '\t' << _dimZ << std::endl;
    std::cout<< "Output file:\t" << _outputFile << std::endl;
}
    