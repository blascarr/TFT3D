
#ifdef _TFT3D_h
	#define _GAUGE_h

	class gauge{
		public:
			void update( Matrix <4, 4, float> m );
			void draw( TFT3D& _tft, uint16_t timer = 10);

	};

#endif