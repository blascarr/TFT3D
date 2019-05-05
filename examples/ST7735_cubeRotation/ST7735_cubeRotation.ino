#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include <TFT3D.h>
#include "mesh.h"

#define ST7735_CS_PIN     7     // <= /CS pin (chip-select, LOW to get attention of ST7735, HIGH and it ignores SPI bus)
#define ST7735_DC_PIN     6     // <= DC pin (1=data or 0=command indicator line) also called RS
#define ST7735_RST_PIN    5     // <= RST pin (optional)

Adafruit_ST7735 tftdisplay = Adafruit_ST7735(ST7735_CS_PIN, ST7735_DC_PIN, ST7735_RST_PIN);
TFT3D tft = TFT3D( tftdisplay );

//------------------------------------------------------------------//
//---------------------------CUBE-----------------------------------//
//------------------------------------------------------------------//

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
  tftdisplay.initR(INITR_BLACKTAB);

  tftdisplay.setRotation(1);
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
  model.update( abs(r%360 ), abs(r%360 ),0 );
  r++;
  model.draw( &tft );
}
