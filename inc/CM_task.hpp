#pragma once

#include<list>
#include<memory>
#include"CM_config.hpp"

struct Task
{
    grains_array grains;

    cm_pos x0, x1;
    cm_pos y0, y1;
    cm_pos z0, z1;

    Microstructure_Properties msp;
    std::unique_ptr<Domain> domain{nullptr};
};

typedef std::vector<Task> tasks_array;
