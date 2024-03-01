#pragma once

#include <cstdint>
#include<string>


typedef int32_t cm_pos;
typedef uint16_t cm_state;
typedef uint8_t cm_colorampl;
typedef uint8_t cm_smallsize;
typedef size_t cm_size;

const cm_state EMPTY = 0;

/* Class Field represents single field of a cellular automata domain */
struct Field
{
    cm_state state;
    cm_colorampl r,g,b;
};

/* Class Neighbourhood contains information about neighboorhood type and parameters*/
struct Neighbourhood
{
    cm_pos** neighbours;
    cm_smallsize size;

    float alfa; // arm 1 angle
    float beta; // arm 2 angle;
    float radius;
};

/* Class GeneratorConfig contains all data necessary to start a microstructure generating process */
class GeneratorConfig
{
    private:
    cm_state* _Astates;
    cm_state* _Bstates;
    cm_colorampl* _colors;
    Neighbourhood _neighbourhood;

    cm_pos _dimX;
    cm_pos _dimY;
    cm_pos _dimZ;

    cm_size _grainsNumber;
    float _baseRadius;

    // For CPU parallel execution //
    cm_smallsize _threadsNum;

    std::string _outputFile;
    std::string _inputFile;

    public:
    GeneratorConfig(cm_pos dim[]);
    GeneratorConfig(cm_pos dim[], Neighbourhood neighbourhood, cm_size nucleusesNum, std::string inputFile, std::string outputFile);
    ~GeneratorConfig();

    cm_pos getDimX() const { return _dimX; }
    cm_pos getDimY() const { return _dimY; }
    cm_pos getDimZ() const { return _dimZ; }
    cm_state* getAbuffer()  { return _Astates; }
    cm_state* getBbuffer() { return _Bstates; }
    cm_colorampl* getColorBuffer()  { return _colors; }

    cm_size getNucleusNum() const { return _grainsNumber; }
    cm_size getSizeInBytes() const { return sizeof(cm_state)*static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ); }
    cm_size getColorBufferSizeInBytes() const { return sizeof(cm_colorampl)*static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);}
    cm_size getCellSizeInBytes() const {return sizeof(cm_state); }
    cm_size getColorsSizeInBytes() const { return sizeof(cm_colorampl)*3; }
    cm_size getCellsNum() const {return static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);  }

    float getBaseRadius() const { return _baseRadius; }
    cm_smallsize getThreadsNumber() const { return _threadsNum; }

    std::string getOutputFile() const { return _outputFile; }
    std::string getInputFile() const { return _inputFile; }


    Neighbourhood getNeighbourhood() const { return _neighbourhood; }

    void setOutputFile(const std::string& outputFile) { _outputFile = outputFile; }
    void setInputFile(const std::string& inputFile) {_inputFile = inputFile; }
    void setColorBuffer(cm_colorampl* colorBuffer) { _colors = colorBuffer; }
    void setDimX(cm_pos dimX) { _dimX = dimX; }
    void setDimY(cm_pos dimY) { _dimY = dimY; }
    void setDimZ(cm_pos dimZ) { _dimZ = dimZ; }
    void setGrainsNumber(cm_size nucleusNum) { _grainsNumber = nucleusNum; }
    void setBaseRadius(float radius) { _baseRadius = radius; }
    void setThreadsNumber(cm_smallsize num) {_threadsNum = num;}
    void setNeighbourhood(const Neighbourhood& neighbourhood) { _neighbourhood = neighbourhood; }
    void printConfiguration() const;

    cm_size getIdx(cm_pos x, cm_pos y, cm_pos z)
    {
        return cm_size(y)*(_dimX * _dimZ) + cm_size(z)*_dimX + cm_size(x);;
    }
};

