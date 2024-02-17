#include<iostream>
#include"CM_datatypes.hpp"

#define LOGS

Domain::Domain(cm_pos dim[], Neighborhood neighborhood, std::string inputFile, std::string outputFile):
    _dimX(dim[0]), _dimY(dim[1]), _dimZ(dim[2]), _inputFile(inputFile), _outputFile(outputFile)
{
    _neighborhood = neighborhood;
    _Astates = new cm_state[cm_size(_dimX) * cm_size(_dimY) * cm_size(_dimZ)];
    _Bstates = new cm_state[cm_size(_dimX) * cm_size(_dimY) * cm_size(_dimZ)];
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
    delete[] _colors;
}

void Domain::printConfiguration() const
{
    std::cout<< "\t===\tDomain configuration\t===\n";
    std::cout<< "Input file:\t" << _inputFile << std::endl;
    std::cout<< "Dimension:\t" << _dimX << '\t' << _dimY << '\t' << _dimZ << std::endl;
    std::cout<< "Neighbourhood size:\t" << _neighborhood.size << std::endl;
    std::cout<< "Neighbourhood:\n";
    for(int i = 0; i < _neighborhood.size; i++)
        std::cout<< "\t" << _neighborhood.neighbours[i][0] << '\t' <<_neighborhood.neighbours[i][1] << '\t' <<_neighborhood.neighbours[i][2] << std::endl;
    std::cout<< "Output file:\t" << _outputFile << std::endl;
}
    