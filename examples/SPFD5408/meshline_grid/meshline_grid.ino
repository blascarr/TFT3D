#define MAX_NODESIZE 12
#define MAX_TRISIZE 6
#define DEBUG_AXIS

#include <SPI.h>

// *** SPFD5408 change -- Begin
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <TFT3D.h>
#include "mesh.h"
#include "meshline.h"

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define  BLACK   0x0001
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

float nodes[MAX_NODESIZE][3] = {
  {(float)(100.0), (float)(0.0), (float)(100.0)},
  {(float)(-100.0), (float)(0.0), (float)(100.0)},
  {(float)(100.0), (float)(0.0), (float)(80.0)},
  {(float)(-100.0), (float)(0.0), (float)(80.0)},
  {(float)(100.0), (float)(0.0), (float)(60.0)},
  {(float)(-100.0), (float)(0.0), (float)(60.0)},
  {(float)(100.0), (float)(0.0), (float)(40.0)},
  {(float)(-100.0), (float)(0.0), (float)(40.0)},
  {(float)(100.0), (float)(0.0), (float)(20.0)},
  {(float)(-100.0), (float)(0.0), (float)(20.0)},
  {(float)(100.0), (float)(0.0), (float)(0.0)},
  {(float)(-100.0), (float)(0.0), (float)(0.0)},
};


unsigned char faces[MAX_TRISIZE][2] = {
  {0, 1},
  {2, 3},
  {4, 5},
  {6, 7},
  {8, 9},
  {10, 11},
};

meshline grid ( nodes, faces ,sizeof(nodes)/sizeof(nodes[0]) ,sizeof(faces)/sizeof(faces[0]) );

int r;
unsigned long auxtimer;

Adafruit_TFTLCD tftdisplay(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TFT3D tft = TFT3D( tftdisplay );

void setup() {
  Serial.begin(9600);
  uint16_t identifier = tftdisplay.readID();
  Serial.println(identifier, HEX);
  tft.init();
  
  tftdisplay.begin(identifier); 
  tftdisplay.fillScreen(GREEN);
  tftdisplay.setRotation(1);
  tftdisplay.fillCircle(0,0, 30 , BLUE );
  grid.setdraw_type(1);
  grid.setskip_tick(10);
  grid.setframe_skip(50);

  //define colors for background
  int c[6] ={BLACK, YELLOW, YELLOW, BLACK, RED, GREEN};
  grid.setColors( c );
  grid.set_tftsize( tftdisplay.width(), tftdisplay.height() );
  
  Serial.print("NODESIZE: ");Serial.println( grid.nodesize );
  Serial.print("TRISIZE: ");Serial.println( grid.facesize);
  grid.rotateTo( 0, 30, 0 );
  grid.moveTo( 0, 0, 0 );
}

void loop() {
  if (Serial.available() ) {
     char c = Serial.read();

     if (c == 'X'){
        int n = Serial.readString().toInt();
        grid.move( n, 0, 0);
     }
     if (c == 'Y'){
        int n = Serial.readString().toInt();
        grid.move( 0, n, 0);
     }
     if (c == 'Z'){
        int n = Serial.readString().toInt();
        grid.move( 0, 0, n);
     }

     if (c == 'M'){
        int n = Serial.readString().toInt();
        grid.XYZmove( n, 0, 0);
     }
     if (c == 'N'){
        int n = Serial.readString().toInt();
        grid.XYZmove( 0, n, 0);
     }
     if (c == 'O'){
        int n = Serial.readString().toInt();
        grid.XYZmove( 0, 0, n);
     }
     //Serial << grid.m_world;
     //Serial.println();
  }
  grid.draw( &tft );
  grid.draw_axis( &tft , 40);
}
