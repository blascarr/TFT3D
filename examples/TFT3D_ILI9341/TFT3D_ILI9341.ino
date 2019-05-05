#define NODECOUNT 8
#define TRICOUNT 12
//#define TFTDEBUG

//#include <Vector.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TFT3D.h>
#include <mesh.h>

long bps = 9600;

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tftdisplay = Adafruit_ILI9341(TFT_CS, TFT_DC );
TFT3D tft = TFT3D( tftdisplay );


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

mesh model( nodes, faces );
int rx,ry,rz,r;

void setup() {
  //Serial.begin(bps);
  //Serial.println("TFT3D");
  tft.init();
  tftdisplay.fillScreen( GREEN );

  model.setdraw_type(1);
  model.setskip_tick(5);
  model.setframe_skip(100);
  
  model.set_tftsize( tftdisplay.width(), tftdisplay.height() );
  //model.set_tftpos( 120, 160  );
}

void loop() {
  
  model.update( abs(r%360),abs(r%360),0 );
  r++;
  model.draw( &tft ); // Draw each timer milliseconds

}
