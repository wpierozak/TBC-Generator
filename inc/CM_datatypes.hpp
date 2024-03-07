#pragma once

#include <cstdint>
#include<string>

/* State of an empty cell */
#define EMPTY 0

typedef int32_t cm_pos;
typedef uint16_t cm_state;
typedef uint8_t cm_colorampl;
typedef uint8_t cm_smallsize;
typedef size_t cm_size;

enum class BC{absorption, bouncy, periodic};
enum class MsFileFormat{xyz, xyzrgb};

/* Class Field represents single field of a cellular automata domain */
struct Field
{
    cm_state state;
    cm_colorampl r,g,b;
};

/* Struct Neighbourhood contains information about neighboorhood type and parameters*/
struct Neighbourhood
{
    float alpha;
    float beta;
    float r;
};

/* Struct NeighbourhoodPlane contains information about neighboorhood type and parameters for a flat field*/
struct NeighbourhoodPlane
{
    float r;
};

struct Grain
{
    cm_pos x,y,z;
    cm_pos tx, ty, tz;
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

    // Boundry conditions //
    BC _boundryCondition;

    // Neighbourhood //

    Neighbourhood _neighbourhood;
    NeighbourhoodPlane _baseNeighbourhood;
    bool _fillBase;

    // Grains //

    Grain* _grains;
    cm_size _grainsNumber;
    float _baseRadius;
    float _maxRadius;

    // For CPU parallel execution //

    cm_smallsize _threadsNum;

    // Files //

    std::string _outputFile;
    std::string _inputFile;
    std::string _outputDir;
    MsFileFormat _msFileFormat;

    public:
    GeneratorConfig(cm_pos dim[]);
    ~GeneratorConfig();

    cm_pos getDimX() const { return _dimX; }
    cm_pos getDimY() const { return _dimY; }
    cm_pos getDimZ() const { return _dimZ; }
    cm_state* getDomain()  { return _domain; }
    cm_state* getStatesBuffer() { return _statesBuffer; }
    BC getBC() const { return _boundryCondition; }

    Grain* getGrains() { return _grains; }
    cm_size getGrainsNum() const { return _grainsNumber; }
    cm_size getCellsNum() const {return static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);  }

    float getBaseRadius() const { return _baseRadius; }
    cm_smallsize getThreadsNumber() const { return _threadsNum; }

    std::string getOutputFile() const { return _outputFile; }
    std::string getInputFile() const { return _inputFile; }
    std::string getOutpuDir() const { return _outputDir; }
    MsFileFormat getMsFileFormat() const {return _msFileFormat; }


    const Neighbourhood& getNeighbourhood() { return _neighbourhood; }
    const NeighbourhoodPlane& getBaseNeighbourhood() { return _baseNeighbourhood; }
    bool fillBase() const { return _fillBase; }

    void setOutputFile(const std::string& outputFile) { _outputFile = outputFile; }
    void setInputFile(const std::string& inputFile) {_inputFile = inputFile; }
    void setOutputDir(const std::string& dir) { _outputDir = dir; }
    void setDimX(cm_pos dimX) { _dimX = dimX; }
    void setDimY(cm_pos dimY) { _dimY = dimY; }
    void setDimZ(cm_pos dimZ) { _dimZ = dimZ; }
    void setGrainsNumber(cm_size nucleusNum) { _grainsNumber = nucleusNum; }
    void setBaseRadius(float radius) { _baseRadius = radius; }
    void setThreadsNumber(cm_smallsize num) {_threadsNum = num;}
    void setNeighbourhood(const Neighbourhood& neighbourhood) { _neighbourhood = neighbourhood; }
    void setBaseNeighbourhood(const NeighbourhoodPlane& neighbourhood) { _baseNeighbourhood = neighbourhood; }
    void setIfFillBase(bool fillBase) { _fillBase = fillBase; }
    void setBC(BC bc){ _boundryCondition = bc; }
    void setMsFileFormat(MsFileFormat ms) { _msFileFormat = ms; }
    void setGrains(Grain* grains) { _grains = grains; }

    void printConfiguration() const;

    cm_size getIdx(cm_pos x, cm_pos y, cm_pos z)
    {
        return cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x);;
    }

    cm_state& getCell(cm_pos x, cm_pos y, cm_pos z)
    {
        return _domain[cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x)];
    }

    cm_state getState(cm_pos x, cm_pos y, cm_pos z);
};

