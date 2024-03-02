#include<iostream>
#include<cmath>
#include"CM_xmlparser.hpp"

const std::string MAIN_NODE = "Configuration";
const std::string DIM_X = "dimX";
const std::string DIM_Y = "dimY";
const std::string DIM_Z = "dimZ";

const std::string BOUNDRY_CONDITIONS = "boundry_conditions";
const std::string ABSORPTION = "absorption";
const std::string BOUNCY = "bouncy";
const std::string PERIODIC = "periodic";

const std::string OUTPUT_FILE = "output_filename";
const std::string OUTPUT_DIR = "output_dir";

const std::string NEIGHBOURHOOD = "neighbourhood";
const std::string ALPHA = "alpha";
const std::string BETA = "beta";
const std::string RADIUS = "radius";

const std::string GRAIN_NUMBER = "grains_number";
const std::string BASE_RADIUS =  "base_radius";
const std::string THREADS_NUMBER = "threads_number";
const std::string FILL_BASE = "fill_base";
const std::string BASE_NEIGHBOURHOOD = "base_neighbourhood";

const std::string MODE = "mode";
const std::string DEGREES = "degrees";
const std::string RADIANS = "radians";

const std::string TRUE = "true";
const std::string FALSE = "false";

GeneratorConfig* parseConfiguration(const std::string& filePath) {
    rapidxml::file<> xml_file(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node(MAIN_NODE.c_str());
    rapidxml::xml_attribute<>* attr = nullptr;

    if (!domain_node) throw std::runtime_error("Invalid XML format.");

    cm_pos dim[3];
    std::string output_file;
    std::string output_dir;
    Neighbourhood neighbourhood;
    NeighbourhoodPlane base_neighbourhood;
    cm_size grains_number;
    float base_radius;
    cm_smallsize threads_number;
    bool fill_base;
    BC boundry_conditon;

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
        else if(NEIGHBOURHOOD == node->name())
        {
            neighbourhood = parseNeighbourhood(node);
        }
        else if(GRAIN_NUMBER == node->name())
        {
            grains_number = std::stoi(node->value());
        }
        else if(BASE_RADIUS == node->name())
        {
            base_radius = std::stoi(node->value());
        }
        else if(THREADS_NUMBER == node->name())
        {
            threads_number = std::stoi(node->value());
        }
        else if(FILL_BASE == node->name())
        {
            if(TRUE == node->value()) fill_base = true;
            else fill_base = false;
        }
        else if(BASE_NEIGHBOURHOOD == node->name())
        {
            base_neighbourhood = parseBaseNeighbourhood(node);
        }
        else if(OUTPUT_DIR == node->name())
        {
            output_dir = node->value();
        }
        else if(BOUNDRY_CONDITIONS == node->name())
        {
            std::string bc = node->value();
            if(bc == ABSORPTION) boundry_conditon = BC::absorption;
            else if(bc == BOUNCY) boundry_conditon = BC::bouncy;
            else if(bc == PERIODIC) boundry_conditon = BC::periodic;
            else throw std::runtime_error("Invalid XML format - invalid boundry condition");
        }
        else throw std::runtime_error("Invalid XML format - invalid node");
        node = node->next_sibling();
    }

    GeneratorConfig* config = new GeneratorConfig(dim);
    config->setOutputFile(output_file);
    config->setInputFile(filePath);
    config->setThreadsNumber(threads_number);
    config->setNeighbourhood(neighbourhood);
    config->setBaseRadius(base_radius);
    config->setGrainsNumber(grains_number);
    config->setBaseNeighbourhood(base_neighbourhood);
    config->setOutputDir(output_dir);
    config->setBC(boundry_conditon);

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

Neighbourhood parseNeighbourhood(rapidxml::xml_node<>* node)
{
    Neighbourhood neighbourhood;
    rapidxml::xml_attribute<>* attr = node->first_attribute(MODE.c_str());
    if(attr == nullptr ) throw std::runtime_error("Invalid XML format - neighbourhood angles mode attribute is missing");
    std::string mode = attr->value();

    rapidxml::xml_node<>*child_node = node->first_node(ALPHA.c_str());
    if(child_node == nullptr) throw std::runtime_error("Invalid XML format.");
    if(mode == DEGREES) neighbourhood.alpha = strtof(child_node->value(), nullptr) * (M_PIf/180.0f);
    else  neighbourhood.alpha = strtof(child_node->value(), nullptr);

    child_node = node->first_node(BETA.c_str());
    if(child_node == nullptr) throw std::runtime_error("Invalid XML format.");
    if(mode == DEGREES) neighbourhood.beta = strtof(child_node->value(), nullptr) * (M_PIf/180.0f);
    else neighbourhood.beta = strtof(child_node->value(), nullptr);

    child_node = node->first_node(RADIUS.c_str());
    if(child_node == nullptr) throw std::runtime_error("Invalid XML format.");
    neighbourhood.r = strtof(child_node->value(), nullptr);

    return neighbourhood;
}

NeighbourhoodPlane parseBaseNeighbourhood(rapidxml::xml_node<>* node)
{
    NeighbourhoodPlane neighbourhood;
    rapidxml::xml_node<>* child_node = node->first_node(RADIUS.c_str());
    if(child_node == nullptr) throw std::runtime_error("Invalid XML format.");
    neighbourhood.r = strtof(child_node->value(), nullptr);

    return neighbourhood;
}