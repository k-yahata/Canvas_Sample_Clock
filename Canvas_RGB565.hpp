#ifndef __CANVAS_RGB565_HPP__
#define __CANVAS_RGB565_HPP__

#include "Canvas.hpp"
#include "Color.hpp"

template < unsigned int WIDTH, unsigned int HEIGHT >
class Canvas_RGB565 : public Canvas<WIDTH, HEIGHT, 2, ColorRGB >{

    inline void get_Color( uint8_t *p_data, ColorRGB &color ) const override{
        // RRRRRGGGGGGBBBBB
        uint8_t b0 = *p_data;
        uint8_t b1 = *(p_data+1);
        color.color[0] = ( b0 >> 3 );
        color.color[1] = (((b0 & 0x7) << 3) + (b1 >> 5));
        color.color[2] = (b1 & 0x1F);
    };
    inline void set_Color( uint8_t *p_data, ColorRGB &color ) override{
        *p_data     = (color.color[0] << 3) + (color.color[1] >> 3);
        *(p_data+1) = ( (color.color[1] & 0x7) << 5 ) + color.color[2];
    };

    void Color_to_RGB888( ColorRGB &color,  uint8_t &r8, uint8_t &g8, uint8_t &b8 ) const override{
        r8 = ( color.color[0] << 3 ) + (color.color[0]>>2);
        g8 = ( color.color[1] << 2 ) + (color.color[1]>>4);
        b8 = ( color.color[2] << 3 ) + (color.color[2]>>2);
    };


};

#endif
