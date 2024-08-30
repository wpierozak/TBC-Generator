#pragma once

#include"rapidxml/rapidxml.hpp"
#include"rapidxml/rapidxml_utils.hpp"
#include"CM_config.hpp"

void parseConfiguration(std::string filePath, Configuration& configuration);

void parseLayer(rapidxml::xml_node<>* node, Layer& mscp);
void parseNeighbourhood(rapidxml::xml_node<>* node, Neighbourhood& neighbourhood);

