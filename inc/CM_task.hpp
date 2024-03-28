#include<list>
#include"CM_datatypes.hpp"

/* Structure TaskData contains necessary information for executing a single subtask */
struct TaskData
{
    grains_array grains;

    Microstructure_Properties msp;
    cm_pos dimX, dimY, dimZ;
};

typedef std::vector<TaskData> taskdata_array;

struct TaskResult
{
    struct Grain{cm_size ID; std::list<cm_pos_vec> assignedCells;};
    std::list<Grain> grains;
};

struct Task
{
    TaskData input;
    TaskResult output;
};
