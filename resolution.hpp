#pragma once
#include <stdint.h>

#define COODINATES_RESOLUTION 5

#define USE_SINGLE_PRECISION_FLOATING_COORDINATES

#ifndef USE_SINGLE_PRECISION_FLOATING_COORDINATES
    typedef int16_t coordinate_t;
    typedef int32_t coordinate_sq_t;
    #define internal_scale 10
    #define half_internal_scale 5
    typedef int16_t pixel_index_t;
#else
    typedef float coordinate_t;
    typedef float coordinate_sq_t;
    #define internal_scale 1.0f
    #define half_internal_scale 0.5f
    typedef int16_t pixel_index_t;
#endif