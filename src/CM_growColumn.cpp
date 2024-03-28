#include<omp.h>
#include<iostream>
#include"CM_growColumn.hpp"

void generate_columns(GeneratorConfig& config)
{
    taskdata_array subdomains;
    nucleation(config);
    createSubdomains(config, subdomains);;
    int threadsNumber = config.getThreadsNumber();

    #pragma omp parallel num_threads(threadsNumber) shared(subdomains)
    {
        growColumns(subdomains[omp_get_thread_num()]);
    }
}