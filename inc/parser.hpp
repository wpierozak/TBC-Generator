#pragma once

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "XmlNodes.hpp"
#include"config.hpp"
#include <string>
#include <iostream>
#include <stdexcept>

class ConfigurationParser {
public:
    // Fields for each parsed structure
    XmlNodes::Space space;
    XmlNodes::Output output;
    XmlNodes::Neighbourhood neighbourhood;
    XmlNodes::Bond bond;
    XmlNodes::Layer layer;
    XmlNodes::Parallel parallel;

    // Method to load and parse an XML file
    void parseXmlFile(const std::string& filePath);
    Configuration createConfiguration() const;
};

