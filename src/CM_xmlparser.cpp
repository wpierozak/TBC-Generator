#include<iostream>
#include<unordered_map>
#include"CM_xmlparser.hpp"

const std::string DOMAIN_NODE = "Domain";
const std::string DIM_X = "dimX";
const std::string DIM_Y = "dimY";
const std::string DIM_Z = "dimZ";
const std::string OUTPUT_FILE = "outputFile";
const std::string NEIGHBOURHOOD = "Neighbourhood";
const std::string NEIGHBOUR_DX = "dx";
const std::string NEIGHBOUR_DY = "dy";
const std::string NEIGHBOUR_DZ = "dz";
const std::string NUCLEUSES = "Nucleuses";
const std::string NUCLEUSES_NUM = "num";

Domain* parseConfiguration(const std::string& filePath) {
    rapidxml::file<> xml_file(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node("Domain");
    rapidxml::xml_attribute<>* attr = nullptr;
    std::unordered_map<std::string, int> nodesMap;

    if (!domain_node) throw std::runtime_error("Invalid XML format.");
    
    cm_pos dim[3];
    std::string output_file;
    Neighbourhood neighbourhood;
    cm_size nucleuses_num;
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
        else if(NUCLEUSES == node->name())
        {
            nucleuses_num = std::stoi(node->first_attribute("num")->value());
        }
        else throw std::runtime_error("Invalid XML format.");
        node = node->next_sibling();
    }

    return (new Domain(dim, neighbourhood, nucleuses_num, filePath, output_file)); 
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