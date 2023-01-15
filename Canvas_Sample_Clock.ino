#include <Arduino.h>
#define ESP32

#include "ClockDrawer.hpp"
Drawer drawer;

// Canvas
#include "Canvas_SSD1331.hpp"
const unsigned char n_canvas = 2;
unsigned char current_canvas;
Canvas_SSD1331 canvas[n_canvas];

// WiFi
#include <WiFi.h>
#include "wifi_setting.h"  // This file must be modified.

#include "Timer.hpp"
Timer timer;

// Display
#include "DisplayController.hpp"
const int pin_DCCntl = 16;
const int pin_RST = 17;
const int pin_CS = 4;      
DisplayController display; 

int t0;
int count;
void setup() {

  pinMode(pin_CS,OUTPUT);
  digitalWrite(pin_CS,HIGH);
  

  WiFi.begin(SSID, WIFI_PASS);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(15);
    i++;
    if( i == 240 ){
        WiFi.disconnect();
  WiFi.begin(SSID, WIFI_PASS);
        i = 0;
    }
  }

  timer.sync_time();

  Serial.begin(115200);  

  drawer.init();

  Serial.println("Display.setup()");
  // 黒画像スタート
  display.setup( pin_DCCntl, pin_RST, pin_CS, canvas, n_canvas );

  // 描画ループ開始
  void (DisplayController::*fp)() = &DisplayController::loop;
  xTaskCreatePinnedToCore([](void *this_pointer){static_cast<DisplayController *>(this_pointer)->loop();}, "DSP", 8192, &display, 1, NULL, 0);

  Serial.println("setup() end");

  t0 = millis();
  count = 0;
}

void loop() {
  // ループ毎にCanvasに記載(displayが書き変わる)
  while( canvas[current_canvas].is_writable() == false ){
      delay(2);
  }
  int hour, min;
  float second;
  timer.get_hms(hour,min,second);
  //Serial.println(second);
  drawer.draw_clock( canvas[current_canvas], hour, min, second );
  current_canvas = ( current_canvas + 1 ) % n_canvas;

  if( millis() - t0 > 1000 ){
    t0 = millis();
    Serial.println(count);
    count = 0;
  }else{
    count++;
  }
}
