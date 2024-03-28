#pragma once
#include<cmath>
#include <cstdint>
#include<vector>
#include<string>

/* State of an empty cell */
#define EMPTY 0

typedef int32_t cm_pos;
typedef uint16_t cm_state;
typedef uint8_t cm_colorampl;
typedef uint8_t cm_smallsize;
typedef size_t cm_size;

struct cm_pos_vec
{
    cm_pos x,y,z;

    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }
};

struct f_vec
{
    double x,y,z;
    double norm() const {
        return sqrt(x*x + y*y + z*z);
    }

    f_vec operator*(double f) const
    {
        return {x*f, y*f, z*f};
    }
};

void normalize(f_vec& vec);

inline f_vec crossProduct(const f_vec& a, const f_vec& b)
{
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}

inline double cos(const f_vec& a, const f_vec& b)
{
    return (a.x*b.x + a.y*b.y)/(a.norm()*b.norm());
}

inline f_vec add(const f_vec& a, const f_vec& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline f_vec substract(const f_vec& a, const f_vec& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

enum class BC{absorption, bouncy, periodic};
enum class MsFileFormat{xyz, xyzrgb};

struct Grain
{
    /* Grain global ID */
    cm_size ID;

    /* Coordinates of the center cell of the base of a column*/
    f_vec center;

    /* Growth tensor' coordinates */
    f_vec growth_tensor;

    /* Reference bound for the angle between growth vector and relative position vector, stored as its cos value */
    double cos_phi_ub;

    /* Reference bound for the RPV norm */
    double rpv_norm_ub;

    /* Reference bound for h0 norm of smooth region */
    double h0_norm_smooth_region;

    /* Reference bound for h0 norm of feathered region */
    double h0_norm_feathered_region;

    /* Reference bound for h0 norm of top region */
    double h0_norm_top_region;

    bool (*smooth_region_function)(double, double, const double*);
    #define SMOOTH_FUNCTION_OREDER 1
    double smooth_region_function_coeff[SMOOTH_FUNCTION_OREDER + 1];

    /* 
        Definition of the column shape within feathered region
        Shape is defined by two cubic functions:
            -> gap function - determines the width of gap between main column and a feather and an inner edge of a feather 
            -> feather function - determines the shape and width of the outer edge of a feather 
    */
    bool (*feathered_region_function)(double, double, const double*);
    #define FEATHERED_FUNCTION_OREDER 3
    double feathered_region_function_coeff[FEATHERED_FUNCTION_OREDER + 1];

    /* 
        Definition of the column shape within top region
        Shape is defined by splain of two linear function. Definition of the shape demands three parameters:
            -> gradient of the "left" function
            -> connecting point coordinate
            -> gradient of the "right" function 
    */
    bool (*top_region_function)(double, double, const double*);
    #define TOP_REGION_PARAM_NUM 3
    double top_region_function_param[TOP_REGION_PARAM_NUM];
    
    /* Reference column radius */
    double ref_column_rad;

    /* Maximum column radius */
    double max_column_rad;
};

typedef std::vector<Grain> grains_array;

void copy(Grain& dest, const Grain& src);

/* Contains information needed to define grain properties */
struct Microstructure_Properties
{
    /* Maximum value of angle widen of a column */
    double max_angle_of_widen;

    /* Maximum tilt of a column in the YZ and XY plane */
    double max_tilt;

    /* Minimum tilt of a column in the YZ and XY plane */
    double min_tilt;

    /* Length of the smooth region in % of total length*/
    double smooth_region_length;
    
    /* Maximum absolute variance of the smooth region in % of total length */
    double smooth_region_length_var;

    /* Length of the feathered region in % of total length*/
    double feathered_region_length;

    /* Maximum absolute variance of the feathered region in % of total length */
    double feathered_region_length_var;

    /* Length of the top region in % of total length*/
    double top_region_length;

    /* Maximum absolute variance of the top region in % of total length */
    double top_region_length_var;

    /* Minimum length of a column */
    double min_length;

    /* Maximum length of a column */
    double max_length;

    /* Maximum value for the reference radius */
    double max_reference_radius;
};

/* Structure subdomain contains necessary information about part of a domain assigned to a thread */
struct Subdomain
{
    grains_array grains;

    Microstructure_Properties msp;
    cm_pos dimX, dimY, dimZ;
};

typedef std::vector<Subdomain> subdomains_array;

/* Class GeneratorConfig contains all data necessary to start a microstructure generating process */
class GeneratorConfig
{
    private:

    // Buffers //

    cm_state* _domain;

    // Dimensions //

    cm_pos _dimX;
    cm_pos _dimY;
    cm_pos _dimZ;

    // Boundry conditions //
    BC _boundryCondition;

    // Grains //

    grains_array _grains;
    cm_size _grainsNumber;
    double _baseRadius;
    double _maxRadius;

    double _referenceRadius;
    double _minTilt;
    double _maxTilt;
    double _maxAngleOfWiden;

    Microstructure_Properties _msp;

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
    BC getBC() const { return _boundryCondition; }

    cm_size getGrainsNum() const { return _grainsNumber; }
    grains_array& getGrains() {return _grains; }
    cm_size getCellsNum() const {return static_cast<size_t>(_dimX) * static_cast<size_t>(_dimY) * static_cast<size_t>(_dimZ);  }
    double getMinTilt() const { return _minTilt; }
    double getMaxTilt() const { return _maxTilt; }
    double getRefRadius() const { return _referenceRadius; }
    double getMaxAngleOfWiden() const { return _maxAngleOfWiden; }

    double getBaseRadius() const { return _baseRadius; }
    cm_smallsize getThreadsNumber() const { return _threadsNum; }

    std::string getOutputFile() const { return _outputFile; }
    std::string getInputFile() const { return _inputFile; }
    std::string getOutpuDir() const { return _outputDir; }
    MsFileFormat getMsFileFormat() const {return _msFileFormat; }

    void setOutputFile(const std::string& outputFile) { _outputFile = outputFile; }
    void setInputFile(const std::string& inputFile) {_inputFile = inputFile; }
    void setOutputDir(const std::string& dir) { _outputDir = dir; }
    void setDimX(cm_pos dimX) { _dimX = dimX; }
    void setDimY(cm_pos dimY) { _dimY = dimY; }
    void setDimZ(cm_pos dimZ) { _dimZ = dimZ; }
    void setGrainsNumber(cm_size nucleusNum) { _grainsNumber = nucleusNum; }
    void setBaseRadius(double radius) { _baseRadius = radius; }
    void setThreadsNumber(cm_smallsize num) {_threadsNum = num;}
    void setBC(BC bc){ _boundryCondition = bc; }
    void setMsFileFormat(MsFileFormat ms) { _msFileFormat = ms; }
    void setGrainsConfiguration(grains_array& grains) { std::copy(grains.begin(), grains.end(), std::back_insert_iterator(_grains)); }
    void setMinTilt(double min) { _minTilt = min; }
    void setMaxTilt(double max) { _maxTilt = max; }
    void setRefRadius(double ref) {_referenceRadius = ref; }
    void setMaxAngleOfWiden(double maxang) {_maxAngleOfWiden = maxang;}

    void setMSP(Microstructure_Properties msp);
    Microstructure_Properties& getMSP();

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

