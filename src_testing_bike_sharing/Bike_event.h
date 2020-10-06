#pragma once

#include <string>
#include "PatternMatcher.h"

class Bike_event
{
    public:

        Bike_event(){}

        attr_t eventID;

        attr_t duration_sec;
        attr_t start_time;
        attr_t end_time;

        attr_t start_station_id;
        
        attr_t end_station_id;
        
        attr_t bike_id; 

        std::string user_type;
        attr_t user_birth;
        attr_t user_gender;

};
