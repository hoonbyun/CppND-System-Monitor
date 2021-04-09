#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() 
{ 
    string line;
    string key;
    string value;
    float memTotal = 0.0, memFree = 0.0;
    std::ifstream filestream(kProcDirectory + kMeminfoFilename);
    if (filestream.is_open()) 
    {
        while (std::getline(filestream, line)) 
            {
                std::istringstream linestream(line);
                while (linestream >> key >> value) 
                {
                    if (key == "MemTotal:") memTotal = stof(value);
                    else if (key == "MemFree:") memFree = stof(value);
                }
            }
    }
    filestream.close();
    return memTotal > 0.0 ? (memTotal - memFree ) / memTotal : 0.0;
}

long LinuxParser::UpTime() 
{ 
    string line;
    string key;
    string value;
    long uptime = 0;
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()) 
    {
        std::getline(filestream, line);
        std::istringstream  linestream(line);
        linestream >> uptime;
    }
    filestream.close();
    return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) 
{ 
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    long active_jifs = 0;
    string line, value;
     if (filestream.is_open()) 
     {
      if(getline(filestream, line)) 
      {
      std::istringstream linestream(line);
      for(int i = 1; i < 18; i++) 
      {
        linestream >> value;
        if(i == 14 || i == 15 || i == 16 || i == 17)
        {
          active_jifs += stol(value);
        }
      }
    }
  }
  filestream.close();
  return active_jifs;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{   
  vector<string> cpuUtil = LinuxParser::CpuUtilization();
  long sum= 0;
  for(string str : cpuUtil)
  {
    sum+=stol(str);
  }
  return sum; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
 vector<string> cpuUtil = LinuxParser::CpuUtilization();
  long idle, iowait;
  idle = stol(cpuUtil[3]);
  iowait = stol(cpuUtil[4]);
  return idle + iowait;   
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{   
  std::ifstream filestream(kProcDirectory +  kStatFilename);
  vector<string> cpuUtil{};
  string line, value;
  if (filestream.is_open()) 
  {
    if(getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> value;
      for(int i = 0; i < 10 ; i ++) 
      {
        if(linestream >> value)
        cpuUtil.push_back(value);
      }
    }
  }
  filestream.close();
  return cpuUtil;  
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string key, line;
  int value = 0;
  if (filestream.is_open()) 
  {
     while (getline(filestream, line)) 
     {
      std::istringstream linestream(line);
      linestream >> key >> value; 
        if (key == "processes") 
        {
          return value;
        }
     }
  }
  filestream.close();
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string key, line;
  int value = 0;
  if (filestream.is_open()) 
  {
     while (getline(filestream, line)) 
     {
      std::istringstream linestream(line);
      linestream >> key >> value; 
        if (key == "procs_running") 
        {
          return value;
        }
     }
  }
  filestream.close();
  return value; 
}

string LinuxParser::Command(int pid) 
{ 
  std::string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open())
  {
      std::getline(filestream, line);
  }
  line.resize(40);
  filestream.close();
  return line;
}

string LinuxParser::Ram(int pid) 
{ 
  std::string line, key, value;
  int ram_mb = 0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open())
  {
      while (std::getline(filestream, line))
      {
        std::istringstream linestream(line);
        linestream >> key >> value;
        // Use VmData instead of VmSize to get actual physical memory size
        if (key == "VmData:") 
        {
          ram_mb = stoi(value)/1000;
          return to_string(ram_mb);
        }
      }      
  }
  filestream.close();
  return to_string(ram_mb);
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{ 
  std::string line, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  long uptime = 0;
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 1 ; i < 23 ; ++i) linestream >> value;
  }
  filestream.close();
  if (!value.empty()) uptime = stol(value)/sysconf(_SC_CLK_TCK);
  return LinuxParser::UpTime() - uptime;
}

string LinuxParser::Uid(int pid) 
{
  string line;
  string key, uid;
  const string pidStr = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pidStr + kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") 
      {
        return uid;
      }
    }
  }
  filestream.close();
  return string();
}

string LinuxParser::User(int pid) 
{
  string line;
  string uid = LinuxParser::Uid(pid);
  if (!uid.empty()) 
  {
    string searchUID = ":x:" + uid;
    string username;
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) 
    {
      while (std::getline(filestream, line)) 
      {
        auto pos = line.find(searchUID);
        username = line.substr(0, pos);
        return username;
      }
    }
    filestream.close();
  }
  return string();
}
