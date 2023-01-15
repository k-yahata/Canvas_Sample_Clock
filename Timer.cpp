#include "Timer.hpp"
#include <Arduino.h>
#include <sntp.h>

bool Timer::called_configTz = false;

Timer::Timer(){
}

void Timer::sync_time(){
    if( called_configTz == false ){
        // start time synchronization
        configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp"); // Do not call this function two times or more.
        called_configTz = true;
    }
}

bool Timer::get_hms( int &hour, int &min, float &sec ){
    //if (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
    //    Serial.println(sntp_get_sync_status());
    //  return false;
    //}else{
        gettimeofday( &(this->st_timeval), NULL );
        struct tm *p_tm = localtime( (&(this->st_timeval).tv_sec) );
        hour = p_tm->tm_hour;
        min = p_tm->tm_min;
        sec = p_tm->tm_sec + st_timeval.tv_usec/1000000.0f;
        return true;
    //}
}

