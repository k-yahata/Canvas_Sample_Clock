#ifndef __CLOCK_DRAWER_HPP__
#define __CLOCK_DRAWER_HPP__

#include "Canvas_SSD1331.hpp"
#include "Color.hpp"

class Drawer{

    public:
    // initialize polygons
    void init();
    // draw frame and hands
    void draw_clock( Canvas_SSD1331 &canvas, int hour, int min, float second );

    private:
    VectorPicture dial;
    Polygon2D hour_hand;
    Polygon2D minute_hand;
    Polygon2D second_hand;

    static const ColorRGB color_dial;
    static const ColorRGB color_hour_hand;
    static const ColorRGB color_minute_hand;
    static const ColorRGB color_second_hand;

    Canvas_SSD1331 canvas_with_dial;

};


#endif