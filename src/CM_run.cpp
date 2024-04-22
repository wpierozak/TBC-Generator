#include<omp.h>
#include"CM_run.hpp"
#include"CM_setup.hpp"
#include"CM_generation.hpp"
#include"CM_logs.hpp"

void run(Configuration& config)
{
    tasks_array tasks;
    if(LogManager::Manager().logmode()) LogManager::Manager().open("Nucleation");
    nucleation(config);
    if(LogManager::Manager().logmode()) LogManager::Manager().close("Nucleation");
    if(LogManager::Manager().logmode()) LogManager::Manager().open("Task definition");
    defineTasks(config, tasks);
    if(LogManager::Manager().logmode()) LogManager::Manager().close("Task definition");

    if(LogManager::Manager().logmode()) LogManager::Manager().open("Run tasks");
    #pragma omp parallel default(none) num_threads(config.threadsNum) shared(tasks)
    {
        cm_int idx = omp_get_thread_num();

        while (tasks[idx].input.y0 != tasks[idx].input.domain->dimY)
        {
            #pragma omp barrier
            runTask(&tasks[idx]);

            #pragma omp barrier

            #pragma omp master
            {
                if(LogManager::Manager().logmode())
                LogManager::Manager().header(std::string("Y = ") + std::to_string(tasks[idx].input.y0));
            }

            tasks[idx].input.y0 += 1;
            tasks[idx].input.y1 += 1;
        }
    }
    if(LogManager::Manager().logmode()) LogManager::Manager().close("Run tasks");
}