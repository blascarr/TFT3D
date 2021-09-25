/*
  Design and created by Blascarr

  TFT3D
  Name    : Blascarr
  Description: TFT3D.h
  version : 1.0

	TFT3D is a generic library that supports differents TFT Screens based on Adafruit_GFX.

	This library is part of a educational course to learn C++ in practice in https://github.com/blascarr/TFTCourse or http://www.blascarr.com/ webpage.

	This Library gives some helpful classes to manipulate 3D Models and graphics visualization for TFT Screen. With TFT3D we extend some dependency issues for use a variety of TFTScreens with Adafruit TFT library . Nowadays is supported

    	ST7735 https://github.com/adafruit/Adafruit-ST7735-Library
    	SPFD5408 https://github.com/adafruit/TFTLCD-Library

	    ILI9341 https://github.com/adafruit/Adafruit_ILI9341

	    BasicLinearAlgebra is needed to integrate Matrix concepts and graphics terms.

	https://github.com/tomstewart89/BasicLinearAlgebra
  	
  	Blascarr invests time and resources providing this open source code like some other libraries, please
  	respect the job and support open-source software.
    
    Written by Adrian for Blascarr
*/

#ifndef _TFT3D_h
  #define _TFT3D_h  

    #include "BasicLinearAlgebra.h"
    using namespace BLA;
    #include "TFT_config.h"

    #define SERIALDEBUG Serial

    #ifdef TFTDEBUG
      #define  DUMP(s, v)  { SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v); }
      #define  DUMPHEX(v)  { SERIALDEBUG.print(v, HEX); }
      #define  DUMPV(v)  { SERIALDEBUG.print(v); }
      #define  DUMPS(s)    { SERIALDEBUG.print(F(s));}
      #define  DUMPPRINTLN() { SERIALDEBUG.println();}
      #define  DUMPTFT(s, v)  { SERIALDEBUG.print(F(s)); SERIALDEBUG.print(v); }
      #define  DUMPVTFT(v)  { SERIALDEBUG.print(v); }
      #define  DUMPSTFT(s)    { SERIALDEBUG.print(F(s));}
    #else
      #define  DUMP(s, v)
      #define  DUMPHEX(v)
      #define  DUMPV(v)
      #define  DUMPS(s)
      #define  DUMPPRINTLN() 
      #define  DUMPTFT(s, v)
      #define  DUMPVTFT(v)
      #define  DUMPSTFT(s)
    #endif

    #ifdef __SD_H__
      #define FILE_MAX_FILENAME_LEN 26
      #define FILE_MAX_FILEDIR_LEN 26
      #define MAX_FILE_LEN 100
      #define BUFFPIXEL 20

      long readNumber(String &data, char delimiter){

          int indexTo = data.indexOf( delimiter );

        String num = data.substring(0,indexTo);
        num.trim();
        data = data.substring(indexTo+1);
        //DUMP("data: ", data);
        return num.toInt();
      }
    #endif


  #if  defined(_ADAFRUIT_GFX_H ) || defined( _PDQ_GFX_H )
      
    #ifndef FOV
      #define FOV 64
    #endif  

      class TFT3D{
        public:
          
          uint8_t TFT_CS_PIN;
          uint8_t TFT_DC_PIN;

          enum class TFType
            {
              UNKNOWN,
              TFT_ST7735,
              TFT_ILI9341,
              TFT_SPFD5408,
              TFT_PDQST7735,
              TFT_PDQILI9341
          };

          TFType _tftype = TFType::UNKNOWN ;

          #ifdef _ADAFRUIT_ST7735H_

            TFT3D( Adafruit_ST7735& tft ): _tft(tft), _tftype(TFType::TFT_ST7735) { 
            }
          
          #endif

          #ifdef _PDQ_ST7735H_
            TFT3D( PDQ_ST7735& tft ): _tft(tft), _tftype(TFType::TFT_PDQST7735) {
                
            }
          #endif

          #ifdef _ADAFRUIT_ILI9341H_
            #define CMD_MEMORY_READ  0x2E
            #define CMD_MEMORY_WRITE 0x2C
            TFT3D( Adafruit_ILI9341& tft ): _tft(tft), _tftype(TFType::TFT_ILI9341) {
                
            }

            TFT3D( Adafruit_ILI9341& tft, uint8_t CS_PIN, uint8_t DC_PIN  ): _tft(tft), _tftype(TFType::TFT_ILI9341),TFT_CS_PIN(CS_PIN),TFT_DC_PIN(DC_PIN) {
                
            }
          #endif

          #ifdef _ADAFRUIT_TFTLCD_H_
            //TFT3D( TftSpfd5408& tft ): _tft(tft), _tftype(TFType::TFT_SPFD5408) {
                
            //}

            TFT3D( Adafruit_TFTLCD& tft ): _tft(tft), _tftype(TFType::TFT_SPFD5408) {
                
            }
          #endif

          #ifdef _PDQ_ILI9341H_
            #define CMD_MEMORY_READ  0x2E
            #define CMD_MEMORY_WRITE 0x2C
            TFT3D( PDQ_ILI9341& tft ): _tft(tft), _tftype(TFType::TFT_PDQILI9341) {
                
            }
            TFT3D( PDQ_ILI9341& tft, uint8_t CS_PIN, uint8_t DC_PIN  ): _tft(tft), _tftype(TFType::TFT_PDQILI9341),TFT_CS_PIN(CS_PIN),TFT_DC_PIN(DC_PIN) {
                
            }
          #endif

          //------------------- TFT Reference Definition ------------------------
          #ifdef _ADAFRUIT_ST7735H_ 
            Adafruit_ST7735& _tft;
          #endif

          #ifdef _PDQ_ST7735H_
            PDQ_ST7735& _tft;
          #endif

          #ifdef _ADAFRUIT_ILI9341H_
            Adafruit_ILI9341& _tft;
          #endif

          #ifdef _PDQ_ILI9341H_
            PDQ_ILI9341& _tft;
          #endif

          #ifdef _ADAFRUIT_TFTLCD_H_
            //TftSpfd5408& _tft;
            Adafruit_TFTLCD& _tft;
          #endif

          TFType getTFType(void){
            return _tftype;
          }

          void init();
          void update(void){};

          //----------------------------------------------------//
          //----------------------SD Methods--------------------//
          //----------------------------------------------------//

          #ifdef __SD_H__
            uint8_t TFT_SD_CS;
            uint16_t _nline=0;
            uint16_t _mode = FILE_READ;
            static const uint8_t maxFilenameLen= FILE_MAX_FILENAME_LEN;
            
            //char _fileName[FILE_MAX_FILENAME_LEN];
            //char _fileDir[FILE_MAX_FILEDIR_LEN];

            String _fileRoot;

            mutable File _file;
            
            void TFT3D::setRoot( String fileDir = "" );

            bool TFT3D::openSD();
            File TFT3D::openFile( String fileName );
            String TFT3D::readln(File &f);

            void TFT3D::readFile( String fileName, bool SerialDebug = true, bool TFTDebug = false);
            void TFT3D::loadImage(char *fileName, int x, int y);
            //void TFT3D::drawCSV(char *fileName, int x, int y);
          #endif

      };

      //---------------------------------------------------------------------------------------------//
      //----------------------------- TFT3D Initialization Methods ----------------------------------//
      
      void TFT3D::init() {
            #ifdef _ADAFRUIT_ILI9341H_
              if( _tftype == TFType::TFT_ILI9341 ){
                _tft.begin();
              }
            #endif

            #ifdef _ADAFRUIT_ST7735H_
              if( _tftype == TFType::TFT_ST7735 ){
                _tft.initR( TAB );
              }
            #endif
            #ifdef _ADAFRUIT_TFTLCD_H_
              /*if( _tftype == TFType::TFT_SPFD5408 ){
                _tft.begin( _tft.readID()); 
              }*/

              if( _tftype == TFType::TFT_SPFD5408 ){
                _tft.begin( TAB ); 
              }
            #endif
            #ifdef _PDQ_ST7735H_
              if( _tftype == TFType::TFT_PDQST7735 ){
                _tft.begin();
              }
            #endif

            #ifdef _PDQ_ILI9341H_
              if( _tftype == TFType::TFT_PDQILI9341 ){
                _tft.begin();
              }
            #endif
      }

    #ifdef __SD_H__
      #include "SDMesh.h"
    #endif
  #endif


#endif

