#pragma once

#include"rapidxml/rapidxml.hpp"
#include"rapidxml/rapidxml_utils.hpp"
#include"CM_datatypes.hpp"

Configuration* parseConfiguration(const std::string& filePath);

void parseMicrostructureProperties(rapidxml::xml_node<>* node, Microstructure_Properties& mscp);
void parseNeighbourhood(rapidxml::xml_node<>* node, Neighbourhood& neighbourhood);