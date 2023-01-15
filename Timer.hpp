#ifndef __TIMER_HPP__
#define __TIMER_HPP__
#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

class Timer{

    private:
    static bool called_configTz;
    timeval st_timeval;
    //tm* p_tm;

    public:
    Timer();
    void sync_time();
    bool get_hms( int &hour, int &min, float &sec );
};

#endif