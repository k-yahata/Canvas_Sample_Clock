#ifndef __DEBUG_FUNCTIONS_HPP__
#define __DEBUG_FUNCTIONS_HPP__

//#define USE_SERIAL

#ifdef DEBUG
#include <iostream>
#include <ctime>
#else
#include <Arduino.h>
#endif

template <typename T>
void debug_print(T val){
#ifdef DEBUG
    std::cout << val;
#else
#ifdef USE_SERIAL
    Serial.print(val);    
#endif
#endif
}
template <typename T, typename... Rest >
void debug_print(T val, Rest... rest){
    debug_print(val);
    debug_print(rest...);
}


template <typename T>
void debug_println(T val){
#ifdef DEBUG
    std::cout << val << std::endl;
#else
#ifdef USE_SERIAL
    Serial.println(val);    
#endif
#endif
}

template <typename T, typename... Rest >
void debug_println(T val, Rest... rest){
    debug_print(val);
    debug_println(rest...);
}

long long debug_millis();

#endif
