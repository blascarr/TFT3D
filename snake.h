#ifdef _TFT3D_h
  #define _SNAKE_h
	#include "tft_item.h"

	class snake: public tftitem{
		public:

			int16_t x,y,x_old,y_old;
			uint16_t colour, c_bckg;

			int16_t mx=1;
			int16_t my=1;

			uint8_t thick=1;

			bool bounce=true; 

			snake::snake(int16_t x = 0,int16_t y = 0,uint16_t c = 0): x(x),y(y),colour( c){

			};

			move( int8_t dx, int8_t dy){
				x_old=x;
				y_old=y;
				x=x+mx*dx;
				y=y+my*dy;
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

			setColour(uint16_t c){
				colour = c;
			}

			//void update( Matrix <4, 4, float> m );
			void draw( TFT3D& _tft, uint16_t timer = 10){

			};

	};

#endif