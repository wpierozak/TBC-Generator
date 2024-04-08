#pragma once

#include"rapidxml/rapidxml.hpp"
#include"rapidxml/rapidxml_utils.hpp"
#include"CM_config.hpp"

Configuration* parseConfiguration(std::string filePath);

void parseMicrostructureProperties(rapidxml::xml_node<>* node, Microstructure_Properties& mscp);
void parseNeighbourhood(rapidxml::xml_node<>* node, Neighbourhood& neighbourhood);
