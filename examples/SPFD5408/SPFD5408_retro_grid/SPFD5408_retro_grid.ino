#define MAX_NODESIZE 12
#define MAX_TRISIZE 6

#include <SPI.h>        // must include this here (or else IDE can't find it)
int m[2] = {3,3};

#define ILI9341_CS_PIN    9      // <= /CS pin (chip-select, LOW to get attention of ILI9341, HIGH and it ignores SPI bus)
#define ILI9341_DC_PIN    8     // <= DC pin (1=data or 0=command indicator line) also called RS
#define ILI9341_RST_PIN 7 // <= RST pin (optional)

#define DEBUG_AXIS

#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library    

#include <TFT3D.h>
//#include "Vector.h"
#include "mesh.h"
#include "meshline.h"

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
Adafruit_TFTLCD tftdisplay(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TFT3D tft = TFT3D( tftdisplay );

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

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(200);
  tftdisplay.begin(0x9341);
  tft.init();
  tftdisplay.setRotation(1);
  tftdisplay.fillScreen( YELLOW );
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
  grid.rotateTo( 30, 0, 30 );
  grid.moveTo( 0, 0, 0 );
  Serial << grid.m_world;
  Serial.println();
}

void loop() {
  if (Serial.available() ) {
     char c = Serial.read();

     if (c == 'X'){
        int n = Serial.readString().toInt();
        grid.moveTo( n, 0, 0);
     }
     if (c == 'Y'){
        int n = Serial.readString().toInt();
        grid.moveTo( 0, n, 0);
     }
     if (c == 'Z'){
        int n = Serial.readString().toInt();
        grid.moveTo( 0, 0, n);
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
  if( millis() - auxtimer > 100){
    auxtimer = millis();
    //grid.move( 0, 0, 1);
  };
  
  grid.draw( &tft );
  grid.draw_axis( &tft , 40);
}
