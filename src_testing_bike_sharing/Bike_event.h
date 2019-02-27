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
        //double start_station_latitude;
        //double start_station_longitude;

        attr_t end_station_id;
        //double end_station_latitude;
        //double end_station_longitude;
        
        attr_t bike_id; 

        std::string user_type;
        attr_t user_birth;
        attr_t user_gender;


        //duser_type,member_birth_year,member_gender,bike_share_for_all_trip;
};
