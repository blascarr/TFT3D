#ifndef _TFTCONFIG_h
  #define _TFTCONFIG_h 

	#ifdef _ADAFRUIT_ILI9341H_

		#define BLACK   0x0001
		#define BLUE    0x001F
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW  0xFFE0
		#define ROSE 	0xF81F
		#define WHITE   0xFFFF
	#endif	

	#ifdef _PDQ_ILI9341H_

		#define BLACK   0x0001
		#define BLUE    0x001F
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW  0xFFE0
		#define ROSE 	0xF81F
		#define WHITE   0xFFFF
	#endif	

	#ifdef _PDQ_ST7735H_

		#define BLACK   0x0001
		#define BLUE    0x001F
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW  0xFFE0
		#define ROSE 	0xF81F
		#define WHITE   0xFFFF
		#ifndef TAB 
			#define TAB 2
		#endif
	#endif	

	#ifdef _ADAFRUIT_TFTLCD_H_
		
		#define BLACK   0x0000
		#define BLUE    0x001F
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW  0xFFE0
		#define ROSE 	0xF81F
		#define WHITE   0xFFFF
		#ifndef TAB 
			#define TAB 0x9341
		#endif
	#endif

	#ifdef _ADAFRUIT_ST7735H_ 
		#define BLACK 	0x000001
		#define BLUE 	0x0000FF
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define WHITE 	0xFFFFFF
		#define YELLOW 	0x00FF00
		#define ROSE 	0x99AA66
		#define WHITE 	0xFFFFFF
		
	#endif
	
	//----------------------------------------//
	//--------------------LUT-----------------//
	//----------------------------------------//

	#define LUT(a) (float)(pgm_read_float(&lut[a]))// return value from LUT in PROGMEM

	const float lut[] PROGMEM = {         // 0 to 90 degrees fixed point COSINE look up table
		//16384, 16381, 16374, 16361, 16344, 16321, 16294, 16261, 16224, 16182, 16135, 16082, 16025, 15964, 15897, 15825, 15749, 15668, 15582, 15491, 15395, 15295, 15190, 15081, 14967, 14848, 14725, 14598, 14466, 14329, 14188, 14043, 13894, 13740, 13582, 13420, 13254, 13084, 12910, 12732, 12550, 12365, 12175, 11982, 11785, 11585, 11381, 11173, 10963, 10748, 10531, 10310, 10086, 9860, 9630, 9397, 9161, 8923, 8682, 8438, 8191, 7943, 7691, 7438, 7182, 6924, 6663, 6401, 6137, 5871, 5603, 5334, 5062, 4790, 4516, 4240, 3963, 3685, 3406, 3126, 2845, 2563, 2280, 1996, 1712, 1427, 1142, 857, 571, 285, 0
		1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9902, 0.9877, 0.9848, 0.9816, 0.9781, 0.9744, 0.9703, 0.9659, 0.9612, 0.9563, 0.9510, 0.9455, 0.9396, 0.9335, 0.9271, 0.9205, 0.9135, 0.9063, 0.8987, 0.8910, 0.8829, 0.8746, 0.8660, 0.8571, 0.8480, 0.8386, 0.8290, 0.8191, 0.8090, 0.7986, 0.7880, 0.7771, 0.7660, 0.7547, 0.7431, 0.7313, 0.7193, 0.7071, 0.6946, 0.6819, 0.6691, 0.6560, 0.6428, 0.6293, 0.6156, 0.6018, 0.5878, 0.5735, 0.5591, 0.5446, 0.5299, 0.5150, 0.4999, 0.4848, 0.4694, 0.4540, 0.4384, 0.4226, 0.4067, 0.3907, 0.3746, 0.3583, 0.3420, 0.3256, 0.3090, 0.2924, 0.2756, 0.2588, 0.2419, 0.2249, 0.2079, 0.1908, 0.1736, 0.1564, 0.1392, 0.1218, 0.1045, 0.0871, 0.0697, 0.0523, 0.0349, 0.0174, 0.0000
	};

	float sin_lut(unsigned int angle) {
		angle += 90;
		if (angle > 450) return LUT(0);
		if (angle > 360 && angle < 451) return -LUT(angle-360);
		if (angle > 270 && angle < 361) return -LUT(360-angle);
		if (angle > 180 && angle < 271) return  LUT(angle-180);
		return LUT(180-angle);
	}

	float cos_lut(unsigned int angle) {
		if (angle > 360) return LUT(0);
		if (angle > 270 && angle < 361) return  LUT(360-angle);
		if (angle > 180 && angle < 271) return -LUT(angle-180);
		if (angle > 90  && angle < 181) return -LUT(180-angle);
		return LUT(angle);
	}

	#ifndef _NOLUT_
		const float ang2rad =180/PI;
		Matrix<4,4 > trotx( float angle ){
			float arrayRot[4][4] = {{1,0,0,0},{0,cos_lut(angle),-sin_lut(angle),0},{0,sin_lut(angle),cos_lut(angle),0},{0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}

		Matrix<4,4 > troty( float angle ){	
			float arrayRot[4][4] = {{cos_lut(angle),0,sin_lut(angle),0},{0,1,0,0},{-sin_lut(angle),0,cos_lut(angle),0},{0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}

		Matrix<4,4 > trotz( float angle ){
			float arrayRot[4][4] = {{cos_lut(angle),-sin_lut(angle),0,0},{sin_lut(angle),cos_lut(angle),0},{0,0,1,0},{0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}

		Matrix<4,4 > transl( float px, float py, float pz ){
			float arrayRot[4][4] = { {1,0,0,px}, {0,1,0,py}, {0,0,1,pz}, {0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}
	#else

		const float ang2rad =180/PI;
		Matrix<4,4 > trotx( float angle ){
			float arrayRot[4][4] = {{1,0,0,0},{0,cos(angle/ang2rad),-sin(angle/ang2rad),0},{0,sin(angle/ang2rad),cos(angle/ang2rad),0},{0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}

		Matrix<4,4 > troty( float angle ){	
			float arrayRot[4][4] = {{cos(angle/ang2rad),0,sin(angle/ang2rad),0},{0,1,0,0},{-sin(angle/ang2rad),0,cos(angle/ang2rad),0},{0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}

		Matrix<4,4 > trotz( float angle ){
			float arrayRot[4][4] = {{cos(angle/ang2rad),-sin(angle/ang2rad),0,0},{sin(angle/ang2rad),cos(angle/ang2rad),0},{0,0,1,0},{0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}

		Matrix<4,4 > transl( float px, float py, float pz ){
			float arrayRot[4][4] = { {1,0,0,px}, {0,1,0,py}, {0,0,1,pz}, {0,0,0,1}};
			Matrix <4, 4 > m (arrayRot);
			return m;
		}
	#endif
#endif