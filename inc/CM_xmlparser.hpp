#pragma once

#include"rapidxml/rapidxml.hpp"
#include"rapidxml/rapidxml_utils.hpp"
#include"CM_config.hpp"

void parseConfiguration(std::string filePath, Configuration& configuration);

void parseMicrostructureProperties(rapidxml::xml_node<>* node, Microstructure_Properties& mscp);
void parseNeighbourhood(rapidxml::xml_node<>* node, Neighbourhood& neighbourhood);

std::shared_ptr<GaussianDistr> parseGaussian(rapidxml::xml_node<>* node);