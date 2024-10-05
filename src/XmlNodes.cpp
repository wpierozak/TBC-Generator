#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "XmlNodes.hpp"
#include <stdexcept>
#include <cstring>

namespace XmlNodes
{
    Space Space::parse(rapidxml::xml_node<>* node) {
        Space space;
        
        if (node == nullptr || std::strcmp(node->name(), Space::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <Space>.");
        }

        space.dimX = std::stol(node->first_node(n_dimX)->value());
        space.dimY = std::stol(node->first_node(n_dimY)->value());
        space.dimZ = std::stol(node->first_node(n_dimZ)->value());

        return space;
    }

    Output Output::parse(rapidxml::xml_node<>* node) {
        Output output;

        if (node == nullptr || std::strcmp(node->name(), Output::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <Output>.");
        }

        output.filename = node->first_node(n_filename)->value();
        output.format = node->first_node(n_format)->value();

        return output;
    }

    Neighbourhood Neighbourhood::parse(rapidxml::xml_node<>* node) {
        Neighbourhood neighbourhood;

        if (node == nullptr || std::strcmp(node->name(), Neighbourhood::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <Neighbourhood>.");
        }

        neighbourhood.x0 = std::stol(node->first_node(n_x0)->value());
        neighbourhood.x1 = std::stol(node->first_node(n_x1)->value());
        neighbourhood.y0 = std::stol(node->first_node(n_y0)->value());
        neighbourhood.y1 = std::stol(node->first_node(n_y1)->value());
        neighbourhood.z0 = std::stol(node->first_node(n_z0)->value());
        neighbourhood.z1 = std::stol(node->first_node(n_z1)->value());

        return neighbourhood;
    }

    Bond Bond::parse(rapidxml::xml_node<>* node) {
        Bond bond;

        if (node == nullptr || std::strcmp(node->name(), Bond::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <Bond>.");
        }

        bond.A = node->first_node(n_A) && node->first_node(n_A)->value()[0] != '\0' ? std::stod(node->first_node(n_A)->value()) : 0.0;
        bond.B = node->first_node(n_B) && node->first_node(n_B)->value()[0] != '\0' ? std::stod(node->first_node(n_B)->value()) : 0.0;
        bond.C = node->first_node(n_C) && node->first_node(n_C)->value()[0] != '\0' ? std::stod(node->first_node(n_C)->value()) : 0.0;
        bond.D = node->first_node(n_D) && node->first_node(n_D)->value()[0] != '\0' ? std::stod(node->first_node(n_D)->value()) : 0.0;
        bond.E = node->first_node(n_E) && node->first_node(n_E)->value()[0] != '\0' ? std::stod(node->first_node(n_E)->value()) : 0.0;
        bond.F = node->first_node(n_F) && node->first_node(n_F)->value()[0] != '\0' ? std::stod(node->first_node(n_F)->value()) : 0.0;
        bond.G = node->first_node(n_G) && node->first_node(n_G)->value()[0] != '\0' ? std::stod(node->first_node(n_G)->value()) : 0.0;
        bond.H = node->first_node(n_H) && node->first_node(n_H)->value()[0] != '\0' ? std::stod(node->first_node(n_H)->value()) : 0.0;
        bond.Z = node->first_node(n_Z) && node->first_node(n_Z)->value()[0] != '\0' ? std::stod(node->first_node(n_Z)->value()) : 0.0;

        return bond;
    }

    Layer::PreferedOrientation Layer::PreferedOrientation::parse(rapidxml::xml_node<>* node) {
        PreferedOrientation orientation;

        if (node == nullptr || std::strcmp(node->name(), PreferedOrientation::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <prefered_orientation>.");
        }

        orientation.x = std::stod(node->first_node(n_x)->value());
        orientation.y = std::stod(node->first_node(n_y)->value());
        orientation.z = std::stod(node->first_node(n_z)->value());

        return orientation;
    }

    Layer Layer::parse(rapidxml::xml_node<>* node) {
        Layer layer;

        if (node == nullptr || std::strcmp(node->name(), Layer::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <Layer>.");
        }

        layer.grains_number = std::stoi(node->first_node(n_grains_number)->value());
        layer.height = std::stol(node->first_node(n_height)->value());
        layer.tilt_stddev = std::stod(node->first_node(n_tilt_stddev)->value());
        if(node->first_node(n_alpha_g) != nullptr)
        {
            layer.alpha_g = std::stod(node->first_node(n_alpha_g)->value());
        }
        if(node->first_node(n_alpha_t) != nullptr)
        {
            layer.alpha_t = std::stod(node->first_node(n_alpha_t)->value());
        }

        layer.prefered_orientation = PreferedOrientation::parse(node->first_node(PreferedOrientation::n_name));

        return layer;
    }

    Parallel Parallel::parse(rapidxml::xml_node<>* node) {
        Parallel parallel;

        if (node == nullptr || std::strcmp(node->name(), Parallel::n_name) != 0) {
            throw std::runtime_error("Invalid XML node: expected <Parallel>.");
        }

        parallel.cpu_threads = std::stoi(node->first_node(n_cpu_threads)->value());
        parallel.gpu = node->first_node(n_gpu) && std::strcmp(node->first_node(n_gpu)->value(), "true") == 0;

        return parallel;
    }
}
