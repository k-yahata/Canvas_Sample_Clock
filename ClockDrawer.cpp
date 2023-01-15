#include "ClockDrawer.hpp"

const ColorRGB Drawer::color_dial(31,63,31);
const ColorRGB Drawer::color_hour_hand(31,0,0);
const ColorRGB Drawer::color_minute_hand(0,63,0);
const ColorRGB Drawer::color_second_hand(0,0,31);


void Drawer::init(){
    hour_hand.add_Point2D(46,17);
    hour_hand.add_Point2D(50,17);
    hour_hand.add_Point2D(50,34);
    hour_hand.add_Point2D(46,34);

    minute_hand.add_Point2D(46.5,6);
    minute_hand.add_Point2D(49.5,6);
    minute_hand.add_Point2D(49.5,34);
    minute_hand.add_Point2D(46.5,34);

    second_hand.add_Point2D(47,6);
    second_hand.add_Point2D(49,6);
    second_hand.add_Point2D(49,34);
    second_hand.add_Point2D(47,34);

    ColoredPolygon2D frame;
    Polygon2D temp;
    Point2D center(48,32);
    frame.polygon.circle24( center, 30 );
    frame.polygon.add_Point2D( frame.polygon.get_Point2D(0));
    temp.circle24(center, 28 );
    temp.add_Point2D(temp.get_Point2D(0));
    frame.polygon.concat_inversely( temp );
    frame.face_color = this->color_dial;
    frame.alpha = 0;
    this->dial.addColoredPolygon(frame);
    frame.polygon.print();

    // ticks
    frame.polygon.clear();
    frame.polygon.add_Point2D(  47, 6 );
    frame.polygon.add_Point2D(  47, 10 );
    frame.polygon.add_Point2D(  49, 10 );
    frame.polygon.add_Point2D(  49, 6 );
    this->dial.addColoredPolygon( frame );
    for( int t = 1; t < 12; t++ ){
        frame.polygon.rotate_equal(30, center);
        this->dial.addColoredPolygon( frame );
    }

    
    uint8_t np = this->dial.p.size();
    for( int p = 0; p < np; p++ ){
        canvas_with_dial.fill_polygon( this->dial.p[p] );
    }


}

void Drawer::draw_clock( Canvas_SSD1331 &canvas, int hour, int min, float second ){

    if( canvas.is_writable() ){
        // clear by dial
        canvas = canvas_with_dial;

        // draw hands
        float deg_hour_hand   = hour * 30 + min * 0.5f + second / 120.0f;
        float deg_minute_hand = min * 6 + second * 0.1f;
        float deg_second_hand = second * 6.0f;
        Point2D center(48,32);
        Polygon2D h = hour_hand.rotate( deg_hour_hand, center );
        Polygon2D m = minute_hand.rotate( deg_minute_hand, center );
        Polygon2D s = second_hand.rotate( deg_second_hand, center );

        canvas.fill_polygon( h, const_cast<ColorRGB&>(color_hour_hand), 0);
        canvas.fill_polygon( m, const_cast<ColorRGB&>(color_minute_hand), 0);
        canvas.fill_polygon( s, const_cast<ColorRGB&>(color_second_hand), 0);
    }

}
