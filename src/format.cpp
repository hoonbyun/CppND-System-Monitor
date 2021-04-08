#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) 
{ 
    long secPerMin = 60;
    long secPerHour = 60 * 60;
    int hour = seconds / secPerHour;
    int min = (seconds % secPerHour) / secPerMin;
    int sec = seconds % secPerMin;
    std::ostringstream os;
    os << std::setfill('0') << std::setw(2) << hour << ':' << std::setfill('0') << std::setw(2) << min << ':' << std::setfill('0') << std::setw(2) << sec;
    return os.str();
}