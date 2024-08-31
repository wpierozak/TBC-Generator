#include "parser.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

// Assuming XmlNodes namespace with Space, Output, Neighbourhood, Bond, Layer, and Parallel structs is already defined as before

void ConfigurationParser::parseXmlFile(const std::string& filePath) {
    // Load the XML file into memory
    rapidxml::file<> xmlFile(filePath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    // Find the root node (assuming the root node is <Configuration>)
    rapidxml::xml_node<>* root = doc.first_node("Configuration");
    if (!root) {
        throw std::runtime_error("Invalid XML file: missing <Configuration> root node.");
    }

    // Parse the <Space> node
    rapidxml::xml_node<>* spaceNode = root->first_node(XmlNodes::Space::n_name);
    if (spaceNode) {
        space = XmlNodes::Space::parse(spaceNode);
        std::cout << "Parsed Space: dimX=" << space.dimX << ", dimY=" << space.dimY << ", dimZ=" << space.dimZ << std::endl;
    }

    // Parse the <Output> node
    rapidxml::xml_node<>* outputNode = root->first_node(XmlNodes::Output::n_name);
    if (outputNode) {
        output = XmlNodes::Output::parse(outputNode);
        std::cout << "Parsed Output: filename=" << output.filename << ", format=" << output.format << std::endl;
    }

    // Parse the <Neighbourhood> node
    rapidxml::xml_node<>* neighbourhoodNode = root->first_node(XmlNodes::Neighbourhood::n_name);
    if (neighbourhoodNode) {
        neighbourhood = XmlNodes::Neighbourhood::parse(neighbourhoodNode);
        std::cout << "Parsed Neighbourhood: x0=" << neighbourhood.x0 << ", x1=" << neighbourhood.x1
                  << ", y0=" << neighbourhood.y0 << ", y1=" << neighbourhood.y1
                  << ", z0=" << neighbourhood.z0 << ", z1=" << neighbourhood.z1 << std::endl;
    }

    // Parse the <Bond> node
    rapidxml::xml_node<>* bondNode = root->first_node(XmlNodes::Bond::n_name);
    if (bondNode) {
        bond = XmlNodes::Bond::parse(bondNode);
        std::cout << "Parsed Bond: A=" << bond.A << ", B=" << bond.B << ", C=" << bond.C << std::endl;
        // Print other bond values similarly
    }

    // Parse the <Layer> node
    rapidxml::xml_node<>* layerNode = root->first_node(XmlNodes::Layer::n_name);
    if (layerNode) {
        layer = XmlNodes::Layer::parse(layerNode);
        std::cout << "Parsed Layer: grains_number=" << layer.grains_number << ", height=" << layer.height
                  << ", tilt_stddev=" << layer.tilt_stddev << std::endl;

        std::cout << "Layer's Preferred Orientation: x=" << layer.prefered_orientation.x
                  << ", y=" << layer.prefered_orientation.y
                  << ", z=" << layer.prefered_orientation.z << std::endl;
    }

    // Parse the <Parallel> node
    rapidxml::xml_node<>* parallelNode = root->first_node(XmlNodes::Parallel::n_name);
    if (parallelNode) {
        parallel = XmlNodes::Parallel::parse(parallelNode);
        std::cout << "Parsed Parallel: cpu_threads=" << parallel.cpu_threads << ", gpu=" << parallel.gpu << std::endl;
    }
}

Configuration ConfigurationParser::createConfiguration() const {
    Configuration config;

    // Set the Output configuration
    config.output.filename = output.filename;
    config.output.format = output.format;

    // Set the Neighbourhood configuration using the new Neighbourhood struct
    config.neighbourhood = Neighbourhood(
        neighbourhood.x0, neighbourhood.x1,
        neighbourhood.y0, neighbourhood.y1,
        neighbourhood.z0, neighbourhood.z1
    );

    // Set the Parallel configuration
    config.parallel.cpu_threads = parallel.cpu_threads;
    config.parallel.gpu = parallel.gpu;

    // Set the Layer configuration
    Configuration::Layer configLayer;
    configLayer.tilt_stddev = M_PI * layer.tilt_stddev / 180.0 ;
    configLayer.prefered_orientation = {layer.prefered_orientation.x, layer.prefered_orientation.y, layer.prefered_orientation.z};
    configLayer.prefered_orientation.normalize();
    configLayer.grainsNumber = layer.grains_number;
    configLayer.layer_height = layer.height;

    config.layers.push_back(configLayer);

    config.space.dimX = space.dimX;
    config.space.dimY = space.dimY;
    config.space.dimZ = space.dimZ;

    config.bond.parameters[0] = bond.A;
    config.bond.parameters[1] = bond.B;
    config.bond.parameters[2] = bond.C;
    config.bond.parameters[3] = bond.D;
    config.bond.parameters[4] = bond.E;
    config.bond.parameters[5] = bond.F;
    config.bond.parameters[6] = bond.G;
    config.bond.parameters[7] = bond.H;
    config.bond.parameters[8] = bond.Z;

    return config;
}


