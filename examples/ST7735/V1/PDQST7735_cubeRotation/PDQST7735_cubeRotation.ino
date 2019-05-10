#define MAX_NODESIZE 8
#define MAX_TRISIZE 12

enum
{
  ST7735_INITB      = 0,        // 1.8" (128x160) ST7735B chipset (only one type)
  ST7735_INITR_GREENTAB   = 1,        // 1.8" (128x160) ST7735R chipset with green tab (same as ST7735_INITR_18GREENTAB)
  ST7735_INITR_REDTAB   = 2,        // 1.8" (128x160) ST7735R chipset with red tab (same as ST7735_INITR_18REDTAB)
  ST7735_INITR_BLACKTAB   = 3,        // 1.8" (128x160) ST7735S chipset with black tab (same as ST7735_INITR_18BLACKTAB)
  ST7735_INITR_144GREENTAB    = 4,        // 1.4" (128x128) ST7735R chipset with green tab
  ST7735_INITR_18GREENTAB   = ST7735_INITR_GREENTAB,  // 1.8" (128x160) ST7735R chipset with green tab
  ST7735_INITR_18REDTAB   = ST7735_INITR_REDTAB,    // 1.8" (128x160) ST7735R chipset with red tab
  ST7735_INITR_18BLACKTAB   = ST7735_INITR_BLACKTAB,  // 1.8" (128x160) ST7735S chipset with black tab
};

#include <SPI.h>        // must include this here (or else IDE can't find it)

#define ST7735_CS_PIN    7      // <= /CS pin (chip-select, LOW to get attention of ST7735, HIGH and it ignores SPI bus)
#define ST7735_DC_PIN    6     // <= DC pin (1=data or 0=command indicator line) also called RS
#define ST7735_CHIPSET ST7735_INITR_BLACKTAB  

#include <PDQ_GFX.h>        // PDQ: Core graphics library
#include <PDQ_ST7735.h>      // PDQ: Hardware-specific driver library
#include <TFT3D.h>
#include "mesh.h"

PDQ_ST7735 tftdisplay;

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
  model.update( abs(r%360 ), abs(r%360 ),0 );
  r++;
  model.draw( &tft );
}
