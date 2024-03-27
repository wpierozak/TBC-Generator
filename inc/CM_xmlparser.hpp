#pragma once

#include"rapidxml/rapidxml.hpp"
#include"rapidxml/rapidxml_utils.hpp"
#include"CM_datatypes.hpp"

 GeneratorConfig* parseConfiguration(const std::string& filePath);

 cm_pos parseDimension(rapidxml::xml_node<>* node);
 std::string parseOutputFile(rapidxml::xml_node<>* node);

void parseMicrostructureProperties(rapidxml::xml_node<>* node, Microstructure_Properties& mscp);