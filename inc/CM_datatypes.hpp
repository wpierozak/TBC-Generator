#include <cstdint>
#include<string>

typedef uint16_t cm_pos;
typedef uint8_t cm_state;
typedef uint8_t cm_colorampl;

/* Class Field represents single field of a cellular automata domain */
struct Field
{
    cm_pos x,y,z;
    cm_state state;
    cm_colorampl r,g,b;
};

/* Class Neighborhood contains information about neighboorhood type and parameters*/
struct Neighborhood
{
    
};

/* Class Domain represents cellular automata domain, it contains all data necessary to start a microstructure generating process */
class Domain
{
    private:
    Field* _domain;
    Neighborhood _neighborhood;

    cm_pos _dimX;
    cm_pos _dimY;
    cm_pos _dimZ;

    std::string _outputFile;
    std::string _inputFile;

    public:
    Domain(cm_pos dim[], Neighborhood neighborhood, std::string inputFile, std::string outputFile);
    ~Domain();

    cm_pos getDimX() const { return _dimX; }
    cm_pos getDimY() const { return _dimY; }
    cm_pos getDimZ() const { return _dimZ; }

    std::string getOutputFile() const { return _outputFile; }
    std::string getInputFile() const { return _inputFile; }
    Field* getField() const { return _domain; }
    Neighborhood getNeighborhood() const { return _neighborhood; }

    void setOutputFile(const std::string& outputFile) { _outputFile = outputFile; }

    void printConfiguration() const;
};

