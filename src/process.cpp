#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() 
{ 
    long active_jiffies = LinuxParser::ActiveJiffies(Pid());
    long total_jiffies = LinuxParser::Jiffies();
    return total_jiffies > 0 ? (float)active_jiffies/(float)total_jiffies : 0.0;
}

string Process::Command() { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process & a) 
{
    return CpuUtilization() < a.CpuUtilization();
}