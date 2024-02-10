#include<iostream>
#include"CM_xmlparser.hpp"

void parseConfiguration(std::string filePath, Domain* dest)
{
    rapidxml::file<> xml_file(filePath.c_str()); 
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node("Domain");

    if (domain_node) {
        cm_pos dim[3];
        int i = 0;

        for (rapidxml::xml_node<>* dim_node = domain_node->first_node(); dim_node; dim_node = dim_node->next_sibling()) {
            dim[i++] = std::stoi(dim_node->value());
        }

        std::string output_file = domain_node->first_node("outputFile")->value();

        Domain domain(dim, Neighborhood(), filePath, output_file); 

    } else {
        std::cerr << "Error: Invalid XML format." << std::endl;
    }
}