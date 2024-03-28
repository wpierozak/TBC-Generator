#include<list>
#include<memory>
#include"CM_datatypes.hpp"

/* Structure TaskData contains necessary information for executing a single subtask */
struct TaskData
{
    grains_array grains;

    cm_pos x0, x1;
    cm_pos y0, y1;
    cm_pos z0, z1;

    Microstructure_Properties msp;
    std::unique_ptr<Domain> domain;
};

typedef std::vector<TaskData> taskdata_array;

struct TaskResult
{
    cm_int code;
};

struct Task
{
    TaskData input;
    TaskResult output;
};

typedef std::vector<Task> tasks_array;
