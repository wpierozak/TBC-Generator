#include<iostream>
#include<unordered_map>
#include"CM_xmlparser.hpp"

const std::string MAIN_NODE = "Configuration";
const std::string DIM_X = "dimX";
const std::string DIM_Y = "dimY";
const std::string DIM_Z = "dimZ";
const std::string OUTPUT_FILE = "output_file";
const std::string NEIGHBOURHOOD = "neighbourhood";
const std::string NEIGHBOUR_DX = "dx";
const std::string NEIGHBOUR_DY = "dy";
const std::string NEIGHBOUR_DZ = "dz";
const std::string GRAIN_NUMBER = "grains_number";
const std::string BASE_RADIUS =  "base_radius";
const std::string THREADS_NUMBER = "threads_number";

GeneratorConfig* parseConfiguration(const std::string& filePath) {
    rapidxml::file<> xml_file(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node(MAIN_NODE.c_str());
    rapidxml::xml_attribute<>* attr = nullptr;
    std::unordered_map<std::string, int> nodesMap;

    if (!domain_node) throw std::runtime_error("Invalid XML format.");

    cm_pos dim[3];
    std::string output_file;
    Neighbourhood neighbourhood;
    cm_size grains_number;
    float base_radius;
    cm_smallsize threads_number;

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
        else throw std::runtime_error("Invalid XML format.");
        node = node->next_sibling();
    }

    GeneratorConfig* config = new GeneratorConfig(dim);
    config->setOutputFile(output_file);
    config->setInputFile(filePath);
    config->setThreadsNumber(threads_number);
    config->setNeighbourhood(neighbourhood);
    config->setBaseRadius(base_radius);
    config->setGrainsNumber(grains_number);

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
    neighbourhood.size = std::stoi(node->first_attribute("size")->value());
    neighbourhood.neighbours = new cm_pos*[(int)neighbourhood.size];
    int idx = 0;
     for (rapidxml::xml_node<>* child = node->first_node(); child != nullptr; child = child->next_sibling()) 
     {
        neighbourhood.neighbours[idx] = new cm_pos[3];
            
        std::string dx_str = child->first_attribute("dx")->value();
        std::string dy_str = child->first_attribute("dy")->value();
        std::string dz_str = child->first_attribute("dz")->value();
        cm_pos dx = std::stoi(dx_str);
        cm_pos dy = std::stoi(dy_str);
        cm_pos dz = std::stoi(dz_str);
        neighbourhood.neighbours[idx][0] = dx;
        neighbourhood.neighbours[idx][1] = dy;
        neighbourhood.neighbours[idx][2] = dz;
        idx++;
    }

    return neighbourhood;
 }