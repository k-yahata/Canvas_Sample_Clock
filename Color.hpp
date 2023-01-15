#pragma once
#include <cstdint>
#include <iostream>

typedef uint8_t color_t;
typedef int16_t color_alpha_blend_t;

template<uint8_t N_COLOR>
class Color{
    public:
    static const uint8_t n_color = N_COLOR;
    uint8_t color[n_color];

    Color<N_COLOR>(){
        for( uint8_t c = 0; c < N_COLOR; c++ ){
            this->color[c] = 0;
        }
    }    

    Color( const Color<N_COLOR>& src){
        for( uint8_t c = 0; c < N_COLOR; c++ ){
            this->color[c] = src.color[c];
        }
    }    

    void print(){
        for( uint8_t c = 0; c < N_COLOR; c++ ){
            std::cout << "Color::print(): " << this->color[c] << std::endl;
        }
    }    
};

class ColorGray : public Color<1>{
    public:
    ColorGray( const color_t intensity = 0 ){
        this->color[0] = intensity;
    }
};

class ColorRGB : public Color<3>{
    public:
    ColorRGB( const color_t r = 0, const color_t g = 0, const color_t b = 0 ){
        this->color[0] = r;
        this->color[1] = g;
        this->color[2] = b;
    }
};
