#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include "process.h"

class Processor {
 public:
  Processor(std::vector<Process> &processes) : processes_(processes) {};
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 std::vector<Process> &processes_;
};

#endif