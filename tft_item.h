#ifdef _TFT3D_h
	#define _TFTITEM_h

	class tftitem{
		public:
			unsigned long auxtimer;
			bool _update = false;

			typedef void ( tftitem::*_f_tft )( TFT3D& _tft );
			_f_tft _f_draw;

			void update(  ){
				//Movement updated 
				_update = true;
			};

			void set_f_draw( _f_tft _f ){
				_f_draw = static_cast<void(tftitem::*)(TFT3D& _tft)>( _f );
			}

			virtual void draw( TFT3D& _tft, uint16_t timer = 0){
				if( millis() - auxtimer > timer){
					auxtimer = millis();
					if( _update ){
						(this->*_f_draw)( _tft );
						_update = false;
					}
					
				};
			};

	};

#endif