#include<omp.h>
#include"CM_run.hpp"
#include"CM_setup.hpp"
#include"CM_generation.hpp"

void run(Configuration& config)
{
    tasks_array tasks;
    nucleation(config);
    defineTasks(config, tasks);
    #pragma omp parallel num_threads(config.threadsNum) shared(tasks)
    {
        cm_int idx = omp_get_thread_num();
        Task& my_task = tasks[idx];

        while (my_task.input.y0 != my_task.input.domain->dimY)
        {
            #pragma omp barrier
            runTask(my_task);

            #pragma omp barrier

            my_task.input.y0 += 1;
            my_task.input.y1 += 1;
        }
    }
}