#ifndef __DEISPLAY_CONTROLLER_HPP__
#define __DEISPLAY_CONTROLLER_HPP__

#include "SSD1331.hpp"
#include "Canvas_SSD1331.hpp"
class DisplayController{

    private:
    unsigned char n_canvas;
    Canvas_SSD1331* *p_canvases;
    SSD1331 display;
    unsigned char wait_mode;
    bool is_rotated;

    public:
    void setup( int pin_DCCntl, int pin_RST, int pin_CS, Canvas_SSD1331 *canvas, unsigned char n_canvas = 2 );
    void rotate();
    
    public:
    //static void static_loop(void*);
    void loop();
    void off();
    void dim_mode();
    void on();
    

};
#endif //__DEISPLAY_CONTROLLER_HPP