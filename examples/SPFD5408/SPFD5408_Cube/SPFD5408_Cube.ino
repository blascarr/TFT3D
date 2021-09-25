#define MAX_NODESIZE 8
#define MAX_TRISIZE 12

#include <SPI.h>        // must include this here (or else IDE can't find it)

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <TFT3D.h>
#include "mesh.h"

Adafruit_TFTLCD tftdisplay(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TFT3D tft = TFT3D( tftdisplay );

float nodes[][3] = {
  {(float)(14.0), (float)(14.0), (float)(-14.0)},
  {(float)(14.0), (float)(-14.0), (float)(-14.0)},
  {(float)(-14.0), (float)(-14.0), (float)(-14.0)},
  {(float)(-14.0), (float)(14.0), (float)(-14.0)},
  {(float)(14.0), (float)(14.0), (float)(14.0)},
  {(float)(-14.0), (float)(14.0), (float)(14.0)},
  {(float)(-14.0), (float)(-14.0), (float)(14.0)},
  {(float)(14.0), (float)(-14.0), (float)(14.0)},
};

unsigned char faces[][3] = {
  {0, 1, 2},
  {2, 3, 0},
  {4, 5, 6},
  {6, 7, 4},
  {0, 4, 7},
  {7, 1, 0},
  {1, 7, 6},
  {6, 2, 1},
  {2, 6, 5},
  {5, 3, 2},
  {4, 0, 3},
  {3, 5, 4},
};

mesh model( nodes, faces ,sizeof(nodes)/sizeof(nodes[0]) ,sizeof(faces)/sizeof(faces[0]) );

int r;

void setup() {
  Serial.begin(9600);
  tft.init();
  tftdisplay.fillScreen( YELLOW );

  model.setdraw_type(1);
  model.setskip_tick(10);
  model.setframe_skip(50);

  //define colors for background
  int c[6] ={BLACK, GREEN, GREEN, BLACK, RED, GREEN};
  model.setColors( c );
  model.set_tftsize( tftdisplay.width(), tftdisplay.height() );

}

void loop() {
  model.rotateTo(  abs(r%360 ), abs(r%360 ),0 );
  //model.update( abs(r%360 ), abs(r%360 ),0 );
  r++;
  model.draw( &tft );
}
