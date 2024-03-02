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
    std::cout<< "Neighbourhood size:\t" << (int)_neighbourhood.size << std::endl;
    std::cout<< "Neighbourhood:\n";
    for(int i = 0; i < _neighbourhood.size; i++)
        std::cout<< "\t" << _neighbourhood.neighbours[i][0] << '\t' <<_neighbourhood.neighbours[i][1] << '\t' <<_neighbourhood.neighbours[i][2] << std::endl;
    std::cout<< "Base neighbourhood size:\t" << (int)_baseNeighbourhood.size << std::endl;
    std::cout<< "Base neighbourhood:\n";
    for(int i = 0; i < _baseNeighbourhood.size; i++)
        std::cout<< "\t" << _baseNeighbourhood.neighbours[i][0] << '\t' <<_baseNeighbourhood.neighbours[i][1] << '\t' <<_baseNeighbourhood.neighbours[i][2] << std::endl;
    std::cout<< "Output file:\t" << _outputFile << std::endl;
}

Neighbourhood::Neighbourhood()
{
    neighbours = nullptr;
    size = 0;
}

Neighbourhood::Neighbourhood(const Neighbourhood& obj)
{
    size = obj.size;
    neighbours = new cm_pos*[size];
    for(int i = 0; i < size; i++)
    {
        neighbours[i] = new cm_pos[3];
        neighbours[i][0] = obj.neighbours[i][0];
        neighbours[i][1] = obj.neighbours[i][1];
        neighbours[i][2] = obj.neighbours[i][2];
    }
}

Neighbourhood::~Neighbourhood()
{
    if(neighbours == nullptr) return;
    for(int i = 0; i < size; i++)
        delete[] neighbours[i];
    delete[] neighbours;
}

Neighbourhood& Neighbourhood::operator=(const Neighbourhood& obj)
{
    size = obj.size;
    neighbours = new cm_pos*[size];
    
    for(int i = 0; i < size; i++)
    {
        neighbours[i] = new cm_pos[3];
        neighbours[i][0] = obj.neighbours[i][0];
        neighbours[i][1] = obj.neighbours[i][1];
        neighbours[i][2] = obj.neighbours[i][2];
    }

    return *this;
}

NeighbourhoodPlane::NeighbourhoodPlane()
{
    neighbours = nullptr;
    size = 0;
}

NeighbourhoodPlane::NeighbourhoodPlane(const NeighbourhoodPlane& obj)
{
    size = obj.size;
    neighbours = new cm_pos*[size];
    for(int i = 0; i < size; i++)
    {
        neighbours[i] = new cm_pos[3];
        neighbours[i][0] = obj.neighbours[i][0];
        neighbours[i][1] = obj.neighbours[i][1];
        neighbours[i][2] = obj.neighbours[i][2];
    }
}

NeighbourhoodPlane::~NeighbourhoodPlane()
{
    if(neighbours == nullptr) return;
    for(int i = 0; i < size; i++)
        delete[] neighbours[i];
    delete[] neighbours;
}

NeighbourhoodPlane& NeighbourhoodPlane::operator=(const NeighbourhoodPlane& obj)
{
    size = obj.size;
    neighbours = new cm_pos*[size];
    for(int i = 0; i < size; i++)
    {
        neighbours[i] = new cm_pos[3];
        neighbours[i][0] = obj.neighbours[i][0];
        neighbours[i][1] = obj.neighbours[i][1];
        neighbours[i][2] = obj.neighbours[i][2];
    }
    return *this;
}