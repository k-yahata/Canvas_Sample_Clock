#include "ColoredPolygon.hpp"


ColoredPolygon2D::ColoredPolygon2D(){
    this->set_color( 0, 0, 0, 0 );
}
ColoredPolygon2D::ColoredPolygon2D( const Polygon2D &polygon ){
    this->polygon = polygon;
}
ColoredPolygon2D::ColoredPolygon2D( const Polygon2D &polygon, const ColorRGB face_color ){
    this->polygon = polygon;
    this->face_color = face_color;    
}
ColoredPolygon2D::ColoredPolygon2D( const ColorRGB face_color ){
    this->face_color = face_color;    
}
ColoredPolygon2D::ColoredPolygon2D( const color_t r, const color_t g, const color_t b, const uint8_t alpha ){
    this->set_color( r, g, b, alpha );
};