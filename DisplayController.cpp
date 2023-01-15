#include "DisplayController.hpp"
#include "debug_functions.hpp"
#include <Arduino.h>

void DisplayController::setup( int pin_DCCntl, int pin_RST, int pin_CS, Canvas_SSD1331 *canvas, unsigned char n_canvas ){
    if( n_canvas == 0 ){
        this->n_canvas = 1;
    }else{
        this->n_canvas = n_canvas;
    }
    this->p_canvases = new Canvas_SSD1331* [this->n_canvas];
    Canvas_SSD1331 *p = canvas;
    for( int n = 0; n < this->n_canvas; n++ ){
        this->p_canvases[n] = p;
        Serial.print("Canvas Pointer: ");
        Serial.println((int)(p));
        p++;
    }
    this->p_canvases[0]->clear();
    is_rotated = false;
    this->display.init( pin_DCCntl, pin_RST, pin_CS ); // onにはしない。
    this->display.send_frame_65K( (this->p_canvases[0]->get_pointer_to_data()) ); // 黒画像を送る。
    this->display.on();

}

void DisplayController::rotate(){
    if( this->is_rotated ){
        display.dir_normal();
        this->is_rotated = false;
    }else{
        display.rotate();
        this->is_rotated = true;
    }
}

// loop task
// 複数のCanvasをリングバッファとみなして順番に表示する。
// 読み込み可能になったらデータを送り、書き込み可能に変更
// 次のデータが読み込み可能になるまで何もしない。
void DisplayController::loop(){
    unsigned char d = 0;
    debug_println("DisplayController::Loop Start");
    debug_print("n_canvas = ");
    debug_println(n_canvas);
    while (1){
        //Serial.print("*");
        if( this->p_canvases[d]->is_readable() ){
            this->display.send_frame_65K( (this->p_canvases[d]->get_pointer_to_data()) );
            this->p_canvases[d]->set_writable();
            d = ( d + 1 ) % this->n_canvas;
        }
        delay(2);
    }
}


void DisplayController::dim_mode(){
    display.dim_mode();
}
void DisplayController::off(){
    display.sleep();
}
void DisplayController::on(){
    display.on();
}