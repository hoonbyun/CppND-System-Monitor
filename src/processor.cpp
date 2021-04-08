#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    float total_cpu_util = 0.0;
    for (Process p : processes_)
    {
        total_cpu_util += p.CpuUtilization();
    }
    return total_cpu_util;
}