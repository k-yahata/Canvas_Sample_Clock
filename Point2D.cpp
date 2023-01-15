#include "Point2D.hpp"
#include <cmath>

#ifndef ESP32 
#include <iostream>
#endif


Point2D::Point2D(const float x, const float y){
    this->x = internal_scale * x;
    this->y = internal_scale * y;
}
Point2D::Point2D(const coordinate_t x, const coordinate_t y, const bool dummy ){
    this->x = x;
    this->y = y;
}


Point2D& Point2D::operator = (const Point2D p){
    this->x = p.x;
    this->y = p.y;
    return *this;
}
Point2D& Point2D::operator += (const Point2D p){
    this->x += p.x;
    this->y += p.y;
    return *this;
}
Point2D& Point2D::operator -= (const Point2D p){
    this->x -= p.x;
    this->y -= p.y;
    return *this;
}
Point2D& Point2D::operator *= (const float f){
    this->x *= f;
    this->y *= f;
    return *this;
}
Point2D& Point2D::operator /= (const float f){
    float f_inv = 1.0f / f;
    this->x *= f_inv;
    this->y *= f_inv;
    return *this;
}
bool Point2D::operator == (const Point2D p) const {
    return( this->x == p.x && this->y == p.y );
}
Point2D Point2D::operator + (const Point2D p) const {
    return Point2D(this->x + p.x, this->y + p.y, true);
}
Point2D Point2D::operator - (const Point2D p) const {
    return Point2D(this->x - p.x, this->y - p.y, true);
}
float Point2D::operator * (const Point2D p) const {
    return (this->x * p.x + this->y * p.y) / internal_scale / internal_scale;
}
Point2D Point2D::operator * (const float f) const {
    return Point2D(this->x * f, this->y * f, true);
}
Point2D Point2D::operator / (const float f) const {
    float f_inv = 1.0f / f;
    return Point2D(this->x * f_inv, this->y * f_inv, true);
}

float Point2D::abs() const{
    return sqrt(this->x * this->x + this->y * this-> y) / internal_scale;
}
float Point2D::normalize(){
    float r = abs();
    if( r != 0.0f ){
        float r_inv = 1.0f/r;
        this->x *= r_inv;
        this->y *= r_inv;
    }
    return r;
}

void Point2D::print(){
#ifndef ESP32
    std::cout << "  Point2D:print():" << x/internal_scale << " " << y/internal_scale << std::endl;
#endif
}

