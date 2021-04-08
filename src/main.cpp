#include "ncurses_display.h"
#include "system.h"
#include <iostream>

int main() {
  System system;
  NCursesDisplay::Display(system);
  // std::cout << "kernel " << system.Kernel() << std::endl;
  // std::cout << "OperatingSystem " << system.OperatingSystem() << std::endl;
  // std::cout << "MemoryUtilization " << system.MemoryUtilization() << std::endl;
  // std::cout << "Uptime " << system.UpTime() << std::endl;
}