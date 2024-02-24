#pragma once

#include <cstdint>
#include<string>

typedef int32_t cm_pos;
typedef int8_t cm_state;
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

/* Class Neighborhood contains information about neighboorhood type and parameters*/
struct Neighborhood
{
    cm_pos** neighbours;
    cm_smallsize size;
};

/* Class Domain represents cellular automata domain, it contains all data necessary to start a microstructure generating process */
class Domain
{
    private:
    cm_state* _Astates;
    cm_state* _Bstates;
    cm_colorampl* _colors;
    Neighborhood _neighborhood;

    cm_pos _dimX;
    cm_pos _dimY;
    cm_pos _dimZ;

    cm_size _nucleusNum;

    std::string _outputFile;
    std::string _inputFile;

    bool _2D;

    public:
    Domain(cm_pos dim[], Neighborhood neighborhood, cm_size nucleusesNum, std::string inputFile, std::string outputFile);
    ~Domain();

    cm_pos getDimX() const { return _dimX; }
    cm_pos getDimY() const { return _dimY; }
    cm_pos getDimZ() const { return _dimZ; }
    cm_state* getAbuffer() const { return _Astates; }
    cm_state* getBbuffer() const { return _Bstates; }
    cm_colorampl* getColorBuffer() const { return _colors; }

    cm_size getNucleusNum() const { return _nucleusNum; }
    cm_size getSizeInBytes() const { return sizeof(cm_state)*static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ); }
    cm_size getColorBufferSizeInBytes() const { return sizeof(cm_colorampl)*static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);}
    cm_size getCellSizeInBytes() const {return sizeof(cm_state); }
    cm_size getColorsSizeInBytes() const { return sizeof(cm_colorampl)*3; }
    cm_size getCellsNum() const {return static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);  }

    std::string getOutputFile() const { return _outputFile; }
    std::string getInputFile() const { return _inputFile; }

    bool is2D() const { return _2D; }

    Neighborhood getNeighborhood() const { return _neighborhood; }

    void setOutputFile(const std::string& outputFile) { _outputFile = outputFile; }
    void setColorBuffer(cm_colorampl* colorBuffer) { _colors = colorBuffer; }

    void printConfiguration() const;

    cm_size getIdx(cm_pos x, cm_pos y, cm_pos z)
    {
        return cm_size(y)*(_dimX + _dimZ) + cm_size(z)*_dimX + cm_size(x);;
    }
};

