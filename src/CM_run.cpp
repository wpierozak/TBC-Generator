#include"CM_run.hpp"
#include"CM_setup.hpp"

void run(Configuration& config)
{
    tasks_array tasks;
    nucleation(config);
    defineTasks(config, tasks);
}