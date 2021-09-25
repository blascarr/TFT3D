#ifdef _TFT3D_h
  
	#define _PIXELS_h
		#include "tft_item.h"
		class pixel: public tftitem {
			public: 
				int16_t x,y,x_old,y_old;
				uint16_t colour, c_bckg;
				int8_t mx=1;
				int8_t my=1;
				bool bounce=true; 
				uint8_t limit = 3;

			pixel::pixel(int16_t x= 0,int16_t y= 0,uint16_t c= 0): x(x),y(y),colour( c){
				//set_f_draw( &tftitem::drawpixel );
			};

			setXY( int16_t x= 0,int16_t y= 0 ){
				setX(x);setY(y);
			};

			setX( int16_t _x= 0 ){
				x  = _x;
			};

			setY(int16_t _y= 0 ){
				y  = _y;
			};

			move( int8_t dx, int8_t dy){
				x_old=x;
				y_old=y;
				x=x+mx*dx;
				y=y+my*dy;
			};

			setColour(uint16_t c){
				colour = c;
			}

			setbckgColour(TFT3D& _tft){
				c_bckg = _tft.readPixel(x, y);
			}

			draw( TFT3D& _tft){
				_tft._tft.drawPixel( x_old, y_old, c_bckg );
				c_bckg = _tft.readPixel(x, y);

				if (bounce){
					if( x >= _tft._tft.width()-limit ){
						mx=-1;
						x = _tft._tft.width()-limit;
					}
					if( x <= limit ){
						mx=1;
						x = limit;
					}
					if( y <= limit ){
						my=1;
						y=limit;
					}
					if( y >= _tft._tft.height()-limit ){
						my =-1;
						y =_tft._tft.height()-limit ;
					}
				}
				_tft._tft.drawPixel( x, y, colour );
			}

		};
#endif