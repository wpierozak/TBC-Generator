#pragma once

#include <cstdint>
#include<string>


typedef int32_t cm_pos;
typedef uint16_t cm_state;
typedef uint8_t cm_colorampl;
typedef uint8_t cm_smallsize;
typedef size_t cm_size;

#define EMPTY 0

/* Class Field represents single field of a cellular automata domain */
struct Field
{
    cm_state state;
    cm_colorampl r,g,b;
};

/* Class Neighbourhood contains information about neighboorhood type and parameters*/
class Neighbourhood
{
    public:
    Neighbourhood();
    Neighbourhood(const Neighbourhood& obj);
    ~Neighbourhood();
    Neighbourhood& operator=(const Neighbourhood& obj);

    cm_pos** neighbours;
    cm_smallsize size;
};

/* Class NeighbourhoodPlane contains information about neighboorhood type and parameters for a flat field*/
class NeighbourhoodPlane
{
    public:
    NeighbourhoodPlane();
    NeighbourhoodPlane(const NeighbourhoodPlane& obj);
    ~NeighbourhoodPlane();
    NeighbourhoodPlane& operator=(const NeighbourhoodPlane& obj);

    cm_pos** neighbours;
    cm_smallsize size;
};

/* Class GeneratorConfig contains all data necessary to start a microstructure generating process */
class GeneratorConfig
{
    private:

    // Buffers //

    cm_state* _domain;
    cm_state* _statesBuffer;

    // Dimensions //

    cm_pos _dimX;
    cm_pos _dimY;
    cm_pos _dimZ;


    // Neighbourhood //

    Neighbourhood _neighbourhood;
    NeighbourhoodPlane _baseNeighbourhood;
    bool _fillBase;

    // Grains //

    cm_size _grainsNumber;
    float _baseRadius;
    float _maxRadius;

    // For CPU parallel execution //

    cm_smallsize _threadsNum;

    // Files //

    std::string _outputFile;
    std::string _inputFile;

    public:
    GeneratorConfig(cm_pos dim[]);
    ~GeneratorConfig();

    cm_pos getDimX() const { return _dimX; }
    cm_pos getDimY() const { return _dimY; }
    cm_pos getDimZ() const { return _dimZ; }
    cm_state* getDomain()  { return _domain; }
    cm_state* getStatesBuffer() { return _statesBuffer; }

    cm_size getNucleusNum() const { return _grainsNumber; }
    cm_size getCellsNum() const {return static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);  }

    float getBaseRadius() const { return _baseRadius; }
    cm_smallsize getThreadsNumber() const { return _threadsNum; }

    std::string getOutputFile() const { return _outputFile; }
    std::string getInputFile() const { return _inputFile; }


    const Neighbourhood& getNeighbourhood() { return _neighbourhood; }
    const NeighbourhoodPlane& getBaseNeighbourhood() { return _baseNeighbourhood; }
    bool fillBase() const { return _fillBase; }

    void setOutputFile(const std::string& outputFile) { _outputFile = outputFile; }
    void setInputFile(const std::string& inputFile) {_inputFile = inputFile; }
    void setDimX(cm_pos dimX) { _dimX = dimX; }
    void setDimY(cm_pos dimY) { _dimY = dimY; }
    void setDimZ(cm_pos dimZ) { _dimZ = dimZ; }
    void setGrainsNumber(cm_size nucleusNum) { _grainsNumber = nucleusNum; }
    void setBaseRadius(float radius) { _baseRadius = radius; }
    void setThreadsNumber(cm_smallsize num) {_threadsNum = num;}
    void setNeighbourhood(const Neighbourhood& neighbourhood) { _neighbourhood = neighbourhood; }
    void setBaseNeighbourhood(const NeighbourhoodPlane& neighbourhood) { _baseNeighbourhood = neighbourhood; }
    void setIfFillBase(bool fillBase) { _fillBase = fillBase; }

    void printConfiguration() const;

    cm_size getIdx(cm_pos x, cm_pos y, cm_pos z)
    {
        return cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x);;
    }

    cm_state& getCell(cm_pos x, cm_pos y, cm_pos z)
    {
        return _domain[cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x)];
    }
};

