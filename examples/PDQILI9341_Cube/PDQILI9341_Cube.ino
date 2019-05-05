#define MAX_NODESIZE 8
#define MAX_TRISIZE 12

#include <SPI.h>        // must include this here (or else IDE can't find it)

#define ILI9341_CS_PIN    9      // <= /CS pin (chip-select, LOW to get attention of ILI9341, HIGH and it ignores SPI bus)
#define ILI9341_DC_PIN    8     // <= DC pin (1=data or 0=command indicator line) also called RS
#define ILI9341_RST_PIN 7 // <= RST pin (optional)

#include <PDQ_GFX.h>        // PDQ: Core graphics library
#include <PDQ_ILI9341.h>      // PDQ: Hardware-specific driver library
#include <TFT3D.h>
#include "mesh.h"

PDQ_ILI9341 tftdisplay;      // PDQ: create LCD object (using pins in "PDQ_ILI9341_config.h")

TFT3D tft = TFT3D( tftdisplay ,ILI9341_CS_PIN, ILI9341_DC_PIN );

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

  Serial.print("NODESIZE: ");Serial.println( model.NODECOUNT );
  Serial.print("TRISIZE: ");Serial.println(model.TRICOUNT);
  
  Serial.print("SIZENODES: ");Serial.println( sizeof(nodes));
  Serial.print("MNODESIZE: ");Serial.println( (sizeof(float)*3));
  
  Serial.print("MNODESIZE: ");Serial.println( sizeof(nodes)/sizeof(nodes[0]));
  Serial.print("MTRISIZE: ");Serial.println( sizeof(faces)/sizeof(faces[0]) );
}

void loop() {
  model.update( abs(r%360 ), abs(r%360 ),0 );
  r++;
  model.draw( &tft );
}
