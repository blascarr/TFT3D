#define MAX_NODESIZE 4
#define MAX_TRISIZE 2
#define NODESIZE 4
#define TRISIZE 2

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

#define  ST7735_CS_PIN    7      // <= /CS pin (chip-select, LOW to get attention of ST7735, HIGH and it ignores SPI bus)
#define ST7735_DC_PIN    6     // <= DC pin (1=data or 0=command indicator line) also called RS
#define ST7735_CHIPSET ST7735_INITR_BLACKTAB  

#include <PDQ_GFX.h>        // PDQ: Core graphics library
#include <PDQ_ST7735.h>      // PDQ: Hardware-specific driver library
#include <TFT3D.h>
#include "Vector.h"
#include "mesh.h"

PDQ_ST7735 tftdisplay;

TFT3D tft = TFT3D( tftdisplay );

float nodes[NODESIZE][1][3] = {
  {{(float)(14.0), (float)(14.0), (float)(0)}},
  {{(float)(14.0), (float)(-14.0), (float)(0)}},
  {{(float)(-14.0), (float)(-14.0), (float)(0)}},
  {{(float)(-14.0), (float)(14.0), (float)(0)}},
};

unsigned char faces[TRISIZE][1][3] = {
  {0, 1, 2},
  {0, 2, 3}
};


vector3D nodev [NODESIZE]={
  Matrix<1,3, float>  (nodes [0]),
  Matrix<1,3, float>  (nodes [1]),
  Matrix<1,3, float>  (nodes [2]),
  Matrix<1,3, float>  (nodes [3])
};

point3D facev [TRISIZE]={
  Matrix<1,3, uint8_t>  (faces [0]),
  Matrix<1,3, uint8_t>  (faces [1])
};

Vector< vector3D > nodelist ( nodev  );

Vector< point3D > facelist ( facev );

mesh model[6];

int r = 0;
boolean tick_once = true;
unsigned long auxtimer;

void setup() {

  tft.init();
  tftdisplay.fillScreen( YELLOW );
  nodelist.Assign( nodev, sizeof(nodes)/sizeof(nodes[0]) );
  facelist.Assign( facev, sizeof(faces)/sizeof(faces[0]) );

  //define colors for background
  int c[6] ={BLACK, GREEN, YELLOW, BLACK, GREEN, YELLOW};
  int marginx = 18;
  int sep = 25;
  for (int i = 0 ; i < 6 ; i++){
    model[i].setnodes ( &nodelist );
    model[i].setfaces ( &facelist );
    model[i].printdata();
    
    model[i].setdraw_type(1);
    model[i].setskip_tick(20);
    model[i].setframe_skip(50);

    model[i].setColors( c );
    model[i].set_tftsize( tftdisplay.width(), tftdisplay.height() );
    model[i].set_tftpos( tftdisplay.width()/2 , marginx + sep*i );
  }
  
}

void loop() {
   
  if( millis() - auxtimer > 10){
    auxtimer = millis();
    r+=5;
    
    for (int i = 0 ; i < 6 ; i++){
      model[i].update( 0 , 330 ,0,0,0,abs((r+10*i)%50) );
      model[i].draw( &tft );
    }
    
  };
}
