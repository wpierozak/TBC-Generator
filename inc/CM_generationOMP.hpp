#pragma once

#include"CM_generation.hpp"

void generate(GeneratorConfig& caDomain, const int threadsNumber);
Subdomain* createSubdomains(GeneratorConfig& caDomain, const int threadsNumber);
void checkBottomLayer(Subdomain& subdomain);
void checkUpperLayer(Subdomain& subdomain);