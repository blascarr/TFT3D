
#include <TFT3D.h>

    #define PIN_LOW(p, b)        digitalWrite(b, LOW)
    #define PIN_HIGH(p, b)       digitalWrite(b, HIGH)
    #define PIN_OUTPUT(p, b)     pinMode(b, OUTPUT)
    #define PIN_INPUT(p, b)      pinMode(b, INPUT_PULLUP)
    #define PIN_READ(p, b)       digitalRead(b)

    #define CD_COMMAND(CD_PIN) PIN_LOW(PORTB, CD_PIN);
    #define CS_ACTIVE(CS_PIN)  PIN_LOW(PORTB, CS_PIN);
    #define CS_IDLE( CS_PIN)    PIN_HIGH(PORTB, CS_PIN);

/*
static uint32_t readbits(int8_t bits)
    {
      uint32_t ret = 0;
      while (bits > 0) {
          ret <<= 8;
        ret |= SPI.transfer(0);
        bits -= 8;
      }
      return ret;
    }

    #define xchg8(x)     readbits(8)
    #define write8(x)     SPI.transfer(x)
    //#define write16(x)   { uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
    #define write18(x)   { write18_N(x, 1); }
    #define WriteCmd(x,CD)  { CD_COMMAND(CD); write8(x); }

    
    #if defined(ARDUINO) && ARDUINO >= 100
      #include "Arduino.h"
    #else
      #include "WProgram.h"
    #endif
*/

class TFTPixel:TFT3D(){
	//uint16_t readReg(uint16_t reg, uint8_t idx = 0);            //note that this reads pairs of data bytes
          /*int16_t  readGRAM(int16_t x, int16_t y, uint16_t *block, int16_t w, int16_t h){
            uint8_t r, g, b;
            int16_t n = w * h;    // we are NEVER going to read > 32k pixels at once
            _tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
            
            //Define Pins on object
            CS_ACTIVE(TFT_CS_PIN);
            WriteCmd( CMD_MEMORY_READ, TFT_DC_PIN );
            // needs 1 dummy read
            r = xchg8(0xFF);
            //DUMPS("DUMP READLOOP: ");
            while (n-- > 0) {
                r = xchg8(0xFF);
                g = xchg8(0xFF);
                b = xchg8(0xFF);
                //DUMPS("COLOR:");DUMPHEX( _tft.color565(r, g, b) );DUMPS("\t");
                *block++ = _tft.color565(r, g, b);
            }
            //DUMPPRINTLN();
            CS_IDLE(TFT_CS_PIN);
            _tft.setAddrWindow(0, 0, _tft.width() - 1, _tft.height() - 1);
            return 0;
          };*/

          //uint16_t readPixel(int16_t x, int16_t y) { uint16_t color; readGRAM(x, y, &color, 1, 1);DUMP("X:",x );DUMP("Y:",y );DUMPS("\tC: ");DUMPHEX( color);DUMPPRINTLN(); return color; }

          /*void  pushColors(uint16_t * block, int16_t n, bool first)
          {
                uint16_t color;
                CS_ACTIVE(TFT_CS_PIN);
                if (first) {
                    WriteCmd(CMD_MEMORY_WRITE, TFT_DC_PIN );
                }
                while (n-- > 0) {
                    color = *block++;
                    write16(color);
                }
                
                CS_IDLE(TFT_CS_PIN);
          }*/

          /*void pushColors(uint8_t * block, int16_t n, bool first)
          {
              uint16_t color;
              uint8_t h, l;
              CS_ACTIVE(TFT_CS_PIN);
              if (first) {
                  WriteCmd(CMD_MEMORY_WRITE, TFT_DC_PIN);
              }
              while (n-- > 0) {
                  h = (*block++);
                  l = (*block++);
                  color = (h << 8) | l;
                  write16(color);
              }
              CS_IDLE(TFT_CS_PIN);
          }*/
          
         /*void pushColors(const uint8_t * block, int16_t n, bool first, bool bigend)
          {
              uint16_t color;
            uint8_t h, l;
            CS_ACTIVE(TFT_CS_PIN);
              if (first) {
                  WriteCmd(CMD_MEMORY_WRITE, TFT_DC_PIN);
              }
              while (n-- > 0) {
                  l = pgm_read_byte(block++);
                  h = pgm_read_byte(block++);
                  color = (bigend) ? (l << 8 ) | h : (h << 8) | l;
              write16(color);
              }
              CS_IDLE(TFT_CS_PIN);
          }*/


          /*void vertScroll(int16_t top, int16_t scrollines, int16_t offset)
          {

              int16_t bfa = HEIGHT - top - scrollines;  // bottom fixed area
              int16_t vsp;
              vsp = top + offset;  // vertical start position
              if (offset < 0)
                  vsp += scrollines;          //keep in unsigned range
              CS_ACTIVE(TFT_CS_PIN);
              WriteCmd( 0x0033, TFT_DC_PIN );
              write16(top);        //TOP
              write16(scrollines); //VSA
              write16(bfa);        //BFA

              WriteCmd(0x0037, TFT_DC_PIN )
              write16(vsp);        //VLSP
              CS_IDLE(TFT_CS_PIN);
          }*/
};

