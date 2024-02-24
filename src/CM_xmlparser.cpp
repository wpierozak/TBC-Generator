#include<iostream>
#include"CM_xmlparser.hpp"

Domain* parseConfiguration(const std::string& filePath) {
    rapidxml::file<> xml_file(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_file.data());

    rapidxml::xml_node<>* domain_node = doc.first_node("Domain");
    rapidxml::xml_attribute<>* attr = nullptr;

    if (domain_node) {
        // Extract dimensions
        cm_pos dim[3];
        int i = 0;
        rapidxml::xml_node<>* dim_node = domain_node->first_node("dimX");
        dim[0] = std::stoi(dim_node->value());
        dim_node = domain_node->first_node("dimY");
        dim[1] = std::stoi(dim_node->value());
        dim_node = domain_node->first_node("dimZ");
        dim[2] = std::stoi(dim_node->value());

        // Extract output file path
        std::string output_file = domain_node->first_node("outputFile")->value();

        // Parse neighborhood
        Neighborhood neighborhood;
        rapidxml::xml_node<>* nb_node = domain_node->first_node("Neighbourhood");
        rapidxml::xml_node<>* child = nullptr;
        attr = nb_node->first_attribute("size");
        neighborhood.size = std::stoi(attr->value());
        neighborhood.neighbours = new cm_pos*[(int)neighborhood.size];
        int idx = 0;
        if (nb_node) {
            
            // Iterate through nb nodes
            for (child = nb_node->first_node(); child != nullptr; child = child->next_sibling()) {
                // Allocate memory for individual neighbor's coordinates
                neighborhood.neighbours[idx] = new cm_pos[3];
            
                std::string dx_str = child->first_attribute("dx")->value();
                std::string dy_str = child->first_attribute("dy")->value();
                std::string dz_str = child->first_attribute("dz")->value();
                cm_pos dx = std::stoi(dx_str);
                cm_pos dy = std::stoi(dy_str);
                cm_pos dz = std::stoi(dz_str);
                neighborhood.neighbours[idx][0] = dx;
                neighborhood.neighbours[idx][1] = dy;
                neighborhood.neighbours[idx][2] = dz;
                idx++;
            }
        }
        rapidxml::xml_node<>* nucleuses_node = domain_node->first_node("Nucleuses");
        cm_size nucleuses_num = std::stoi(nucleuses_node->first_attribute("num")->value());
        // Update the provided Domain object
        return (new Domain(dim, neighborhood, nucleuses_num, filePath, output_file)); 
    } else {
        throw std::runtime_error("Invalid XML format.");
    }
}
