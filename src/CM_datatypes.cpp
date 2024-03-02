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
    std::cout<< "Output file name:\t" << _outputFile << std::endl;
    std::cout<< "Output directory:\t" << _outputDir << std::endl;
    std::cout<< "Dimension:\t" << _dimX << '\t' << _dimY << '\t' << _dimZ << std::endl;
    std::cout<< "Boundry condition:\t";
    if(_boundryCondition == BC::absorption) std::cout << "absorption" << std::endl;
    else if(_boundryCondition == BC::bouncy) std::cout << "bouncy" << std::endl;
    else if(_boundryCondition == BC::periodic) std::cout << "periodic" << std::endl;
    std::cout<< "Neighbourhood:\n";
    std::cout<<"\talpha:\t" << _neighbourhood.alpha << std::endl;
    std::cout<<"\tbeta:\t" << _neighbourhood.beta << std::endl;
    std::cout<<"\tradius:\t" << _neighbourhood.r << std::endl;
    std::cout<< "Base neighbourhood:\n";
    std::cout<<"\tradius:\t" << _baseNeighbourhood.r << std::endl;
    std::cout<<"Grains number:\t" << _grainsNumber << std::endl;
    std::cout<<"Runnig on: " << _threadsNum << " threads" << std::endl;
}

cm_state GeneratorConfig::getState(cm_pos x, cm_pos y, cm_pos z)
{
    bool inside = (x >= 0 && x < _dimX) && (y >= 0 && y < _dimY) && (z >= 0 && z < _dimZ);

    if(inside) return _domain[cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x)];
    cm_pos dx, dy, dz;

    switch (_boundryCondition)
    {
    case BC::absorption :
        return EMPTY;
    break;

    case BC::bouncy:
        if(x >= _dimX) dx = -2 * (x - (_dimX-1)) + 1;
        else dx = -2*x - 1;
        
        if(y >= _dimY) dy = -2 * (y - (_dimY-1)) + 1;
        else dy = -2*y - 1;

        if(z >= _dimZ) dz = -2 * (z - (_dimZ-1)) + 1;
        else dz = -2*z - 1;

        return _domain[cm_size(y + dy)*(_dimX * _dimZ) + cm_size(z + dz)*_dimX + cm_size(x + dx)];
    break;

    case BC::periodic:
        if(x >= _dimX) x = x - _dimX;
        else x = _dimX + x;

        if(y >= _dimY) y = y - _dimY;
        else y = _dimY + y;

        if(z >= _dimZ) z = z - _dimZ;
        else z = _dimZ + z;

        return _domain[cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x)];
    break;

    default:
        return EMPTY;
        break;
    }
}