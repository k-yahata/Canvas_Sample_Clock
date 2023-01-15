#ifndef __POINT2D_HPP__
#define __POINT2D_HPP__
/*==============================================================//
class Point2D 
   Cooridinates of point in 2 dimensional plane.
   The coordinates are treated as int16_t and the values are 
   internally multiplied by internal_scale.
   2次元の点の座標のデータクラス,マイコン向け 
//==============================================================*/
#include "resolution.hpp"
class Point2D{
    // member
    public:
    coordinate_t x;
    coordinate_t y;

    // constructors
    public:
    // Constructor for user coordinates. 
    Point2D(const float x = 0.0f, const float y = 0.0f); 
    private:
    // Constructor for internal coordinates.
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
    // return inner product in user coordinates
    float operator * (const Point2D p) const;
    Point2D operator * (const float f) const;
    Point2D operator / (const float f) const;
    // return absolute values in user coordinates
    float abs() const;
    // normalize the vector in user coordinates
    float normalize();


    void print();
};
#endif