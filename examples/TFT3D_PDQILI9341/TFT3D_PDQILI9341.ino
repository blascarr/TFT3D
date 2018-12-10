#define NODECOUNT 8
#define TRICOUNT 12


float nodes[NODECOUNT][3] = {
  {(float)(14.0), (float)(14.0), (float)(-14.0)},
  {(float)(14.0), (float)(-14.0), (float)(-14.0)},
  {(float)(-14.0), (float)(-14.0), (float)(-14.0)},
  {(float)(-14.0), (float)(14.0), (float)(-14.0)},
  {(float)(14.0), (float)(14.0), (float)(14.0)},
  {(float)(-14.0), (float)(14.0), (float)(14.0)},
  {(float)(-14.0), (float)(-14.0), (float)(14.0)},
  {(float)(14.0), (float)(-14.0), (float)(14.0)},
};

unsigned char faces[TRICOUNT][3] = {
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

#include <SPI.h>        // must include this here (or else IDE can't find it)

#define  ILI9341_CS_PIN    6      // <= /CS pin (chip-select, LOW to get attention of ILI9341, HIGH and it ignores SPI bus)
#define ILI9341_DC_PIN    7     // <= DC pin (1=data or 0=command indicator line) also called RS
#define ILI9341_RST_PIN 8 // <= RST pin (optional)

#include <PDQ_GFX.h>        // PDQ: Core graphics library
#include <PDQ_ILI9341.h>      // PDQ: Hardware-specific driver library
#include <TFT3D.h>
PDQ_ILI9341 tftdisplay;      // PDQ: create LCD object (using pins in "PDQ_ILI9341_config.h")

//Adafruit_ILI9341 tftdisplay = Adafruit_ILI9341(TFT_CS, TFT_DC );
TFT3D tft = TFT3D( tftdisplay );



mesh model( nodes, faces );
int rx,ry,rz,r;

void setup() {
  // put your setup code here, to run once:
  tft.init();
  tftdisplay.fillScreen( GREEN );
  tft.setMesh( &model );
  //next_tick = last_btn = millis();
  model.setdraw_type(1);
  model.setskip_tick(5);
  model.setframe_skip(100);
}

void loop() {
    model.update( r,r,0 );
  r++;
  if(r>360){
    r  = 0;
    }
  tft.draw(100);
}
