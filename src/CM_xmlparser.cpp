#include<iostream>
#include<cmath>
#include"CM_xmlparser.hpp"

const std::string MAIN_NODE = "Configuration";
const std::string DIM_X = "dimX";
const std::string DIM_Y = "dimY";
const std::string DIM_Z = "dimZ";

const std::string NEIGHBOURHOOD = "neighbourhood";
const std::string X0 = "x0";
const std::string X1 = "x1";
const std::string Y0 = "y0";
const std::string Y1 = "y1";
const std::string Z0 = "z0";
const std::string Z1 = "z1";
const std::string BOUNDRY_CONDITION = "boundry_conditions";
const std::string ABSORPTION = "absorption";
const std::string BOUNCY = "bouncy";
const std::string PERIODIC = "periodic";

const std::string OUTPUT_FILE = "output_filename";
const std::string OUTPUT_DIR = "output_dir";

const std::string MS_FILE_FORMAT = "ms_file_format";
const std::string MS_XYZ = "xyz";
const std::string MS_RGB = "xyzrgb";

const std::string LAYER = "Layer";
const std::string GRAIN_NUMBER = "grains_number";

const std::string THREADS_NUMBER = "threads_number";

const std::string BITMAPS = "bitmaps";
const std::string TRUE = "true";
const std::string FALSE = "false";

const std::string MEAN = "mean";
const std::string STD = "std";
const std::string MAX = "max";
const std::string MIN = "min";
const std::string LENGTH = "length";
const std::string RADIUS = "radius";
const std::string WIDEN = "widen";
const std::string TILT = "tilt";
const std::string TOP_FRAC = "top_frac";
const std::string RESOLUTION = "resolution";
const std::string LAYER_HEIGHT = "layer_height";

void parseConfiguration(std::string filePath, Configuration& configuration) {
    configuration.inputFile = filePath;
    rapidxml::file<> xml_file(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node(MAIN_NODE.c_str());
    rapidxml::xml_attribute<>* attr = nullptr;

    if (!domain_node) throw std::runtime_error("Invalid XML format.");

    cm_pos dim[3];
    Neighbourhood neighbourhood;
    BC boundry_condition = BC::absorption;

    rapidxml::xml_node<>* node = domain_node->first_node();
    while(node)
    {
        if(DIM_X == node->name())
        {
            dim[0] = std::stoi(node->value());
        }
        else if(DIM_Y == node->name())
        {
            dim[1] = std::stoi(node->value());
        }
        else if(DIM_Z == node->name())
        {
            dim[2] = std::stoi(node->value());
        }
        else if(OUTPUT_FILE == node->name())
        {
            configuration.outputFile = node->value();
        }
        else if(THREADS_NUMBER == node->name())
        {
            configuration.threadsNum = std::stoi(node->value());
        }
        else if(OUTPUT_DIR == node->name())
        {
            configuration.outputDir = node->value();
        }
        else if(MS_FILE_FORMAT == node->name())
        {
            std::string format = node->value();
            if(format == MS_XYZ) configuration.msFileFormat = MsFileFormat::xyz;
            else if(format == MS_RGB) configuration.msFileFormat = MsFileFormat::xyzrgb;
        }
        else if(LAYER == node->name())
        {
            configuration.layers.resize(configuration.layers.size()+1);
            parseMicrostructureProperties(node, configuration.layers.back());
        }
        else if(NEIGHBOURHOOD == node->name())
        {
            parseNeighbourhood(node, neighbourhood);
        }
        else if(BOUNDRY_CONDITION == node->name())
        {
            std::string bc = node->value();
            if(bc == ABSORPTION) boundry_condition = BC::absorption;
            else if(bc == BOUNCY) boundry_condition = BC::bouncy;
            else if(bc == PERIODIC) boundry_condition = BC::periodic;
            else throw std::runtime_error("Invalid XML format - invalid boundry condition");
        }
        else if(BITMAPS == node->name())
        {
            if(node->value() == TRUE) configuration.bitmaps = true;
            else configuration.bitmaps = false;
        }
        else throw std::runtime_error("Invalid XML format - invalid node");
        node = node->next_sibling();
    }
    configuration.dimX = dim[0];
    configuration.dimY = dim[1];
    configuration.dimZ = dim[2];
    configuration.neighbourhood = neighbourhood;
}

void parseMicrostructureProperties(rapidxml::xml_node<>* node, Microstructure_Properties& mscp)
{
    rapidxml::xml_node<>* child_node = node->first_node();

    while(child_node)
    {
        if(LENGTH == child_node->name()) mscp.length = parseGaussian(child_node);
        else if(RADIUS == child_node->name()) mscp.radius = parseGaussian(child_node);
        else if(TILT == child_node->name()) mscp.tilt = parseGaussian(child_node);
        else if(WIDEN == child_node->name()) mscp.widen = parseGaussian(child_node);
        else if(TOP_FRAC == child_node->name()) mscp.top_frac = parseGaussian(child_node);
        else if(RESOLUTION == child_node->name())
        {
            if(child_node->value() == "HIGH") mscp.resolution = Resolution::HIGH;
            else mscp.resolution = Resolution::LOW;
        }
        else if(GRAIN_NUMBER == child_node->name())
        {
            mscp.grainsNumber = std::stoi(child_node->value());
        }
        else if(LAYER_HEIGHT == child_node->name())
        {
            mscp.layer_height = std::stoi(child_node->value());
        }
        else throw std::runtime_error("MSP - Invalid XML format - invalid node");
        child_node = child_node->next_sibling();
    }
}

void parseNeighbourhood(rapidxml::xml_node<>* node, Neighbourhood& neighbourhood)
{
    rapidxml::xml_node<>* child_node = node->first_node();

    while(child_node)
    {
        if(X0 == child_node->name())
        {
            neighbourhood.dx0 = std::stoi(child_node->value());
        }
        else if(X1 == child_node->name())
        {
            neighbourhood.dx1 = std::stoi(child_node->value());
        }
        else if(Y0 == child_node->name())
        {
            neighbourhood.dy0 = std::stoi(child_node->value());
        }
        else if(Y1 == child_node->name())
        {
            neighbourhood.dy1 = std::stoi(child_node->value());
        }
        else if(Z0 == child_node->name())
        {
            neighbourhood.dz0 = std::stoi(child_node->value());
        }
        else if(Z1 == child_node->name())
        {
            neighbourhood.dz1 = std::stoi(child_node->value());
        }
        else throw std::runtime_error("N - Invalid XML format - invalid node");
        child_node = child_node->next_sibling();
    }
}

std::shared_ptr<GaussianDistr> parseGaussian(rapidxml::xml_node<>* node)
{
    double mean, std;
    double min = 0.0; double max = 0.0;
    auto child_node = node->first_node();
    while(child_node)
    {
        if(child_node->name() == MEAN)
        {
            mean = std::stod(child_node->value());
        }
        else if(child_node->name() == STD)
        {
            std = std::stod(child_node->value());
        }
        else if(child_node->name() == MIN)
        {
            min = std::stod(child_node->value());
        }
        else if(child_node->name() == MAX)
        {
            max = std::stod(child_node->value());
        }
        child_node = child_node->next_sibling();
    }
    return std::make_shared<GaussianDistr>(mean, std, min, max);
}