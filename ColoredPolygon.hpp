#ifndef __COLORED_POLYGON_2D_HPP_
#define __COLORED_POLYGON_2D_HPP_

#include "Polygon2D.hpp"
#include "Color.hpp"

class ColoredPolygon2D{

    //================
    // variables 
    //================
    public:
    Polygon2D polygon;
    ColorRGB face_color;
    uint8_t alpha;

    public:
    //================
    // constructor / コンストラクタ
    //================
    ColoredPolygon2D();
    ColoredPolygon2D( const Polygon2D &polygon );
    ColoredPolygon2D( const Polygon2D &polygon, const ColorRGB face_color );
    ColoredPolygon2D( const ColorRGB face_color );
    ColoredPolygon2D( const color_t r, const color_t g, const color_t b, const uint8_t alpha = 0 );

    //================
    // funcsions 
    //================
    public:
    inline void set_color( const color_t r, const color_t g, const color_t b, const uint8_t alpha = 0 ){
        this->face_color.color[0] = r;
        this->face_color.color[1] = g;
        this->face_color.color[2] = b;
        this->alpha = alpha;
    }
};

#endif