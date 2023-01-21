#ifndef __POINT2D_HPP__
#define __POINT2D_HPP__
/*==============================================================//
class Point2D 
   Cooridinates of point in 2 dimensional plane.
   The type of coordinates( int16_t or float) can be selected in resolution.hpp.
   When int16_t is used, the values are multiplied by the internal_scale defined in the resolution.hpp.
//==============================================================*/
#include "resolution.hpp"
class Point2D{
    // member
    public:
    // internal coorinates, which are scaled by internal_scale
    coordinate_t x;
    coordinate_t y;

    // constructors
    public:
    // Constructor to define the point in user coordinates. 
    // The x and y will be multiplied by the internal_scale.
    Point2D(const float x = 0.0f, const float y = 0.0f); 
    // Constructor to define the point in internal coordinates. 
    Point2D(const coordinate_t x, const coordinate_t y, const bool dummy );

    // methods
    public:
    Point2D & operator = (const Point2D p);
    Point2D & operator += (const Point2D p);
    Point2D & operator -= (const Point2D p);
    Point2D & operator *= (const float f);
    Point2D & operator /= (const float f);
    bool operator == (const Point2D p) const;
    Point2D operator + (const Point2D p) const;
    Point2D operator - (const Point2D p) const;
    Point2D operator * (const float f) const;
    Point2D operator / (const float f) const;

    // operation for integer operations
    Point2D & mul_equal_int(const int16_t one_is_128);
    Point2D & div_equal_int(const int16_t one_is_128);
    Point2D mul_int(const int16_t one_is_128) const;
    Point2D div_int(const int16_t one_is_128) const;


    // return inner product in user coordinates
    float operator * (const Point2D p) const;
    // return absolute values in user coordinates
    float abs() const;
    // normalize the vector in user coordinates
    float normalize();


    void print();
};
#endif