#include<iostream>
#include<cmath>
#include"CM_xmlparser.hpp"

const std::string MAIN_NODE = "Configuration";
const std::string DIM_X = "dimX";
const std::string DIM_Y = "dimY";
const std::string DIM_Z = "dimZ";

const std::string OUTPUT_FILE = "output_filename";
const std::string OUTPUT_DIR = "output_dir";

const std::string MS_FILE_FORMAT = "ms_file_format";
const std::string MS_XYZ = "xyz";
const std::string MS_RGB = "xyzrgb";

const std::string GRAIN_CONFIG = "Grain";
const std::string GRAIN_NUMBER = "grain_number";
const std::string MAX_COLUMN_TILT = "max_tilt";
const std::string MIN_COLUMN_TILT = "min_tilt";
const std::string REFERENCE_RADIUS = "ref_radius";
const std::string MAX_ANGLE_OF_WIDEN = "max_angle_of_widen";
const std::string SMOOTH_REGION_LENGTH = "smooth_region_length";
const std::string SMOOTH_REGION_LENGTH_VAR = "smooth_region_length_var";
const std::string FEATHERED_REGION_LENGTH = "feathered_region_length";
const std::string FEATHERED_REGION_LENGTH_VAR = "feathered_region_length_var";
const std::string TOP_REGION_LENGTH = "top_region_length";
const std::string TOP_REGION_LENGTH_VAR = "top_region_length_var";
const std::string MAX_LENGTH = "max_length";
const std::string MIN_LENGTH = "min_length";
const std::string BASE_RADIUS =  "base_radius";

const std::string THREADS_NUMBER = "threads_number";

const std::string TRUE = "true";
const std::string FALSE = "false";

Configuration* parseConfiguration(const std::string& filePath) {
    rapidxml::file<> xml_file(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node(MAIN_NODE.c_str());
    rapidxml::xml_attribute<>* attr = nullptr;

    if (!domain_node) throw std::runtime_error("Invalid XML format.");

    cm_pos dim[3];
    std::string output_file;
    std::string output_dir;

    cm_size grains_number;

    Microstructure_Properties msp;

    cm_smallsize threads_number;
    MsFileFormat ms_file_format;

    rapidxml::xml_node<>* node = domain_node->first_node();
    while(node)
    {
        if(DIM_X == node->name())
        {
            dim[0] = parseDimension(node);
        }
        else if(DIM_Y == node->name())
        {
            dim[1] = parseDimension(node);
        }
        else if(DIM_Z == node->name())
        {
            dim[2] = parseDimension(node);
        }
        else if(OUTPUT_FILE == node->name())
        {
            output_file = parseOutputFile(node);
        }
        else if(THREADS_NUMBER == node->name())
        {
            threads_number = std::stoi(node->value());
        }
        else if(OUTPUT_DIR == node->name())
        {
            output_dir = node->value();
        }
        else if(MS_FILE_FORMAT == node->name())
        {
            std::string format = node->value();
            if(format == MS_XYZ) ms_file_format = MsFileFormat::xyz;
            else if(format == MS_RGB) ms_file_format = MsFileFormat::xyzrgb;
        }
        else if(GRAIN_CONFIG == node->name())
        {
            parseMicrostructureProperties(node, msp);
        }
        else if(GRAIN_NUMBER == node->name())
        {
            grains_number = std::stoi(node->value());
        }
        else throw std::runtime_error("Invalid XML format - invalid node");
        node = node->next_sibling();
    }

    Configuration* config = new Configuration;
    config->dimX = dim[0];
    config->dimY = dim[1];
    config->dimZ = dim[2];
    config->outputFile = output_file;
    config->inputFile = filePath;
    config->threadsNum = threads_number;
    config->grainsNumber = grains_number;
    config->outputDir = output_dir;
    config->msFileFormat = ms_file_format;
    config->msp = msp;

    return config; 
}

cm_pos parseDimension(rapidxml::xml_node<>* node)
{
    return std::stoi(node->value());
}

std::string parseOutputFile(rapidxml::xml_node<>* node)
{
    return node->value();
}

void parseMicrostructureProperties(rapidxml::xml_node<>* node, Microstructure_Properties& mscp)
{
    rapidxml::xml_node<>* child_node = node->first_node();

    while(child_node)
    {
        if(REFERENCE_RADIUS == child_node->name())
        {
            mscp.max_reference_radius = std::stoi(child_node->value());
        }
        else if(MAX_COLUMN_TILT == child_node->name())
        {
            mscp.max_tilt = std::stoi(child_node->value());
        }
        else if(MIN_COLUMN_TILT == child_node->name())
        {
            mscp.min_tilt = std::stoi(child_node->value());
        }
        else if(MAX_COLUMN_TILT == child_node->name())
        {
            mscp.max_tilt = std::stoi(child_node->value());
        }
        else if(SMOOTH_REGION_LENGTH == child_node->name())
        {
            mscp.smooth_region_length = std::stoi(child_node->value());
        }
        else if(SMOOTH_REGION_LENGTH_VAR == child_node->name())
        {
            mscp.smooth_region_length_var = std::stoi(child_node->value());
        }
        else if(FEATHERED_REGION_LENGTH == child_node->name())
        {
            mscp.feathered_region_length = std::stoi(child_node->value());
        }
        else if(FEATHERED_REGION_LENGTH_VAR == child_node->name())
        {
            mscp.feathered_region_length_var = std::stoi(child_node->value());
        }
        else if(TOP_REGION_LENGTH == child_node->name())
        {
            mscp.top_region_length = std::stoi(child_node->value());
        }
        else if(TOP_REGION_LENGTH_VAR == child_node->name())
        {
            mscp.top_region_length_var = std::stoi(child_node->value());
        }
        else if(MIN_LENGTH == child_node->name())
        {
            mscp.min_length = std::stoi(child_node->value());
        }
        else if(MAX_LENGTH == child_node->name())
        {
            mscp.max_length = std::stoi(child_node->value());
        }
        child_node = child_node->next_sibling();
    }
}