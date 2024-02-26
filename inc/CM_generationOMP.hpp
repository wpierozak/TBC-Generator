#pragma once

#include"CM_generation.hpp"

void generate(Domain& caDomain, const int threadsNumber);
Subdomain* createSubdomains(Domain& caDomain, const int threadsNumber);
void checkBottomLayer(Subdomain& subdomain);
void checkUpperLayer(Subdomain& subdomain);